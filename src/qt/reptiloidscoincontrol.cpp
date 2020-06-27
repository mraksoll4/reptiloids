// Copyright (c) 2018-2019 The Blocknet developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <qt/reptiloidscoincontrol.h>

#include <qt/reptiloidsguiutil.h>

#include <qt/addresstablemodel.h>
#include <qt/bitcoinunits.h>
#include <qt/optionsmodel.h>

#include <key_io.h>
#include <uint256.h>

#include <QApplication>
#include <QAbstractItemView>
#include <QClipboard>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QResizeEvent>
#include <QSettings>
#include <QSizePolicy>

/**
 * @brief Dialog encapsulates the coin control table. The default size is 960x580
 * @param parent
 */
ReptiloidsCoinControlDialog::ReptiloidsCoinControlDialog(WalletModel *w, QWidget *parent, Qt::WindowFlags f, bool standaloneMode) : 
    QDialog(parent, f), walletModel(w), standaloneMode(standaloneMode) 
{
    //this->setStyleSheet("border: 1px solid red;");
    this->setContentsMargins(QMargins());
    this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    this->setMinimumSize(BGU::spi(400), BGU::spi(250));
    this->setWindowTitle(tr("Coin Control"));

    content = new QFrame(this);
    content->setObjectName("coinControlDialog");
    content->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    auto *contentLayout = new QVBoxLayout;
    contentLayout->setContentsMargins(0, 0, 0, BGU::spi(20));
    content->setLayout(contentLayout);

    confirmBtn = new ReptiloidsFormBtn;
    confirmBtn->setText(tr("Confirm"));
    cancelBtn = new ReptiloidsFormBtn;
    cancelBtn->setObjectName("cancel");
    cancelBtn->setText(standaloneMode ? tr("Close") : tr("Cancel"));

    auto *btnBox = new QFrame;
    auto *btnBoxLayout = new QHBoxLayout;
    btnBoxLayout->setContentsMargins(QMargins());
    btnBoxLayout->setSpacing(BGU::spi(15));
    btnBox->setLayout(btnBoxLayout);
    btnBoxLayout->addStretch(1);
    btnBoxLayout->addWidget(cancelBtn, 0, Qt::AlignLeft);
    if (!standaloneMode)
        btnBoxLayout->addWidget(confirmBtn, 0, Qt::AlignLeft);
    btnBoxLayout->addStretch(1);

    // Manages the coin list
    cc = new ReptiloidsCoinControl;

    // Manages the selected coin details
    feePanel = new QFrame;
    feePanel->setObjectName("feePanel");
    feePanel->setContentsMargins(BGU::spi(20), 0, BGU::spi(20), 0);
    feePanel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
    feePanelLayout = new QGridLayout;
    feePanelLayout->setHorizontalSpacing(BGU::spi(30));
    feePanel->setLayout(feePanelLayout);
    feePanel->setHidden(true);

    // row 1
    quantityLbl = new QLabel(QString("%1:").arg(tr("Selected Inputs")));
    quantityVal = new QLabel;                                       quantityVal->setTextInteractionFlags(Qt::TextSelectableByMouse);
    amountLbl = new QLabel(QString("%1:").arg(tr("Amount")));
    amountVal = new QLabel;                                         amountVal->setTextInteractionFlags(Qt::TextSelectableByMouse);
    feeLbl = new QLabel(QString("%1:").arg(tr("Estimated Fee")));
    feeVal = new QLabel;                                            feeVal->setTextInteractionFlags(Qt::TextSelectableByMouse);
    afterFeeLbl = new QLabel(QString("%1:").arg(tr("After Fee")));
    afterFeeVal = new QLabel;                                       afterFeeVal->setTextInteractionFlags(Qt::TextSelectableByMouse);
    feePanelLayout->addWidget(quantityLbl, 0, 0, Qt::AlignRight);
    feePanelLayout->addWidget(quantityVal, 0, 1, Qt::AlignLeft);
    feePanelLayout->addWidget(amountLbl, 0, 2, Qt::AlignRight);
    feePanelLayout->addWidget(amountVal, 0, 3, Qt::AlignLeft);
    feePanelLayout->addWidget(feeLbl, 0, 4, Qt::AlignRight);
    feePanelLayout->addWidget(feeVal, 0, 5, Qt::AlignLeft);
    feePanelLayout->addWidget(afterFeeLbl, 0, 6, Qt::AlignRight);
    feePanelLayout->addWidget(afterFeeVal, 0, 7, Qt::AlignLeft);

    // row 2
    bytesLbl = new QLabel(QString("%1:").arg(tr("Bytes")));
    bytesVal = new QLabel;                                          bytesVal->setTextInteractionFlags(Qt::TextSelectableByMouse);
    priorityLbl = new QLabel(QString("%1:").arg(tr("Priority")));
    priorityVal = new QLabel;                                       priorityVal->setTextInteractionFlags(Qt::TextSelectableByMouse);
    dustLbl = new QLabel(QString("%1:").arg(tr("Dust")));
    dustVal = new QLabel;                                           dustVal->setTextInteractionFlags(Qt::TextSelectableByMouse);
    changeLbl = new QLabel(QString("%1:").arg(tr("Change")));
    changeVal = new QLabel;                                         changeVal->setTextInteractionFlags(Qt::TextSelectableByMouse);
    feePanelLayout->addWidget(bytesLbl, 1, 0, Qt::AlignRight);
    feePanelLayout->addWidget(bytesVal, 1, 1, Qt::AlignLeft);
    feePanelLayout->addWidget(priorityLbl, 1, 2, Qt::AlignRight);
    feePanelLayout->addWidget(priorityVal, 1, 3, Qt::AlignLeft);
    feePanelLayout->addWidget(dustLbl, 1, 4, Qt::AlignRight);
    feePanelLayout->addWidget(dustVal, 1, 5, Qt::AlignLeft);
    feePanelLayout->addWidget(changeLbl, 1, 6, Qt::AlignRight);
    feePanelLayout->addWidget(changeVal, 1, 7, Qt::AlignLeft);

    feePanelLayout->setRowMinimumHeight(0, BGU::spi(20));
    feePanelLayout->setRowMinimumHeight(1, BGU::spi(20));

    contentLayout->addWidget(cc, 1);
    contentLayout->addSpacing(5);
    contentLayout->addWidget(feePanel);
    contentLayout->addWidget(btnBox);

    this->resize(BGU::spi(1010), BGU::spi(680));

    connect(confirmBtn, &QPushButton::clicked, this, [this]() {
        Q_EMIT accept();
    });
    connect(cancelBtn, &QPushButton::clicked, this, [this]() {
        cc->setData(std::make_shared<ReptiloidsCoinControl::Model>());
        Q_EMIT reject();
    });
    connect(cc, &ReptiloidsCoinControl::tableUpdated, this, &ReptiloidsCoinControlDialog::updateLabels);

    updateLabels();
}

void ReptiloidsCoinControlDialog::resizeEvent(QResizeEvent *evt) {
    QDialog::resizeEvent(evt);
    content->resize(evt->size().width(), evt->size().height());
}

void ReptiloidsCoinControlDialog::showEvent(QShowEvent *event) {
    QDialog::showEvent(event);
    updateLabels();
}

void ReptiloidsCoinControlDialog::updateLabels() {
    // TODO Reptiloids Qt handle fee info
    feePanel->setHidden(true);
//    auto displayUnit = walletModel->getOptionsModel()->getDisplayUnit();
//    int64_t totalSelectedAmount = 0;
//    QVector<WalletModel::CoinInput> inputs;
//
//    if (cc->getData() != nullptr) {
//        for (auto &utxo : cc->getData()->data) {
//            if (utxo->unlocked && utxo->checked) {
//                totalSelectedAmount += utxo->camount;
//                inputs.push_back({ uint256S(utxo->transaction.toStdString()), utxo->vout, utxo->address, utxo->camount });
//            }
//        }
//    }
//
//    if (totalSelectedAmount > 0) {
//        auto res = walletModel->wallet().getFeeInfo(inputs, payAmount);
//        quantityVal->setText(QString::number(res.quantity));
//        amountVal->setText(BitcoinUnits::formatWithUnit(displayUnit, res.amount));
//        feeVal->setText(BitcoinUnits::formatWithUnit(displayUnit, res.fee));
//        afterFeeVal->setText(BitcoinUnits::formatWithUnit(displayUnit, res.afterFee));
//        bytesVal->setText(QString::number(res.bytes));
//        priorityVal->setText(cc->getPriorityLabel(res.priority));
//        dustVal->setText(res.dust ? tr("yes") : tr("no"));
//        changeVal->setText(BitcoinUnits::formatWithUnit(displayUnit, res.change));
//    }
//
//    feePanel->setHidden(totalSelectedAmount == 0);
}

void ReptiloidsCoinControlDialog::populateUnspentTransactions(const QVector<ReptiloidsSimpleUTXO> & txSelectedUtxos) {
    int displayUnit = walletModel->getOptionsModel()->getDisplayUnit();
    QVector<ReptiloidsCoinControl::UTXO*> utxos;

    auto mapCoins = walletModel->wallet().listCoins();
    for (auto & item : mapCoins) {
        CAmount nSum = 0;
        double dPrioritySum = 0;
        int nChildren = 0;
        int nInputSum = 0;
        const auto sWalletAddress = EncodeDestination(item.first);

        for (auto & tup : item.second) {
            const auto & out = std::get<0>(tup);
            const auto & walletTx = std::get<1>(tup);
            int nInputSize = 0;
            nSum += walletTx.txout.nValue;
            nChildren++;

            auto *utxo = new ReptiloidsCoinControl::UTXO;
            utxo->checked = false;

            // address
            CTxDestination outputAddress;
            QString sAddress = "";
            if (ExtractDestination(walletTx.txout.scriptPubKey, outputAddress)) {
                sAddress = QString::fromStdString(EncodeDestination(outputAddress));
                utxo->address = sAddress;
                CPubKey pubkey;
                CKeyID *keyid = boost::get<CKeyID>(&outputAddress);
                if (keyid && walletModel->wallet().getPubKey(*keyid, pubkey) && !pubkey.IsCompressed())
                    nInputSize = 29; // 29 = 180 - 151 (public key is 180 bytes, priority free area is 151 bytes)
            }

            // label
            if (!(sAddress.toStdString() == sWalletAddress)) { // if change
                utxo->label = tr("(change)");
            } else {
                QString sLabel = walletModel->getAddressTableModel()->labelForAddress(sAddress);
                if (sLabel.isEmpty())
                    sLabel = tr("(no label)");
                utxo->label = sLabel;
            }

            // amount
            utxo->amount = BitcoinUnits::format(displayUnit, walletTx.txout.nValue);
            utxo->camount = walletTx.txout.nValue;

            // date
            utxo->date = QDateTime::fromTime_t(static_cast<uint>(walletTx.time));

            // confirmations
            utxo->confirmations = walletTx.depth_in_main_chain;

            // priority
            double dPriority = ((double)walletTx.txout.nValue / (nInputSize + 78)) * (walletTx.depth_in_main_chain + 1); // 78 = 2 * 34 + 10
            utxo->priority = dPriority;
            dPrioritySum += (double)walletTx.txout.nValue * (walletTx.depth_in_main_chain + 1);
            nInputSum += nInputSize;

            // transaction hash & vout
            uint256 txhash = out.hash;
            utxo->transaction = QString::fromStdString(txhash.GetHex());
            utxo->vout = static_cast<unsigned int>(out.n);

            // locked coins
            utxo->locked = walletModel->wallet().isLockedCoin(out);
            utxo->unlocked = !utxo->locked;

            // selected coins
            for (auto &outpt : txSelectedUtxos) {
                if (outpt.hash == txhash && outpt.vout == static_cast<uint>(out.n) && !utxo->locked) {
                    utxo->checked = true;
                    break;
                }
            }

            utxos.push_back(utxo);
        }
    }

    auto ccData = std::make_shared<ReptiloidsCoinControl::Model>();
    ccData->freeThreshold = COIN * 576 / 250; // TODO Reptiloids Qt handle free threshold
    ccData->data = utxos;
    getCC()->setData(ccData);
    
    if (standaloneMode) // only process utxo state changes in standalone mode
        connect(getCC(), &ReptiloidsCoinControl::tableUpdated, this, &ReptiloidsCoinControlDialog::updateUTXOState);
}

void ReptiloidsCoinControlDialog::updateUTXOState() {
    for (auto *data : getCC()->getData()->data) {
        if (data->locked) {
            COutPoint utxo(uint256S(data->transaction.toStdString()), data->vout);
            walletModel->wallet().lockCoin(utxo);
        }
        if (data->unlocked) {
            COutPoint utxo(uint256S(data->transaction.toStdString()), data->vout);
            walletModel->wallet().unlockCoin(utxo);
        }
    }
}

/**
 * @brief Manages and displays the coin control input list.
 * @param parent
 */
ReptiloidsCoinControl::ReptiloidsCoinControl(QWidget *parent) : QFrame(parent), layout(new QVBoxLayout),
                                                           table(new QTableWidget), contextMenu(new QMenu) {
    // this->setStyleSheet("border: 1px solid red");
    this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    layout->setContentsMargins(QMargins());
    this->setLayout(layout);

    // table
    table->setContentsMargins(QMargins());
    table->setColumnCount(COLUMN_TXVOUT + 1);
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    table->setSelectionBehavior(QAbstractItemView::SelectRows);
    table->setAlternatingRowColors(true);
    table->setColumnWidth(COLUMN_PADDING, BGU::spi(10));
    table->setColumnWidth(COLUMN_CHECKBOX, BGU::spi(30));
    table->setShowGrid(false);
    table->setFocusPolicy(Qt::NoFocus);
    table->setContextMenuPolicy(Qt::CustomContextMenu);
    table->setColumnHidden(COLUMN_TXHASH, true);
    table->setColumnHidden(COLUMN_TXVOUT, true);
    table->verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    table->verticalHeader()->setDefaultSectionSize(BGU::spi(60));
    table->verticalHeader()->setVisible(false);
    table->horizontalHeader()->setDefaultAlignment(Qt::AlignLeft);
    table->horizontalHeader()->setSortIndicatorShown(true);
    table->horizontalHeader()->setSectionsClickable(true);
    table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    table->horizontalHeader()->setSectionResizeMode(COLUMN_PADDING, QHeaderView::Fixed);
    table->horizontalHeader()->setSectionResizeMode(COLUMN_CHECKBOX, QHeaderView::Fixed);
    table->horizontalHeader()->setSectionResizeMode(COLUMN_AMOUNT, QHeaderView::ResizeToContents);
    table->horizontalHeader()->setSectionResizeMode(COLUMN_ADDRESS, QHeaderView::ResizeToContents);
    table->setHorizontalHeaderLabels({ "", "", tr("Amount"), tr("Label"), tr("Address"), tr("Date"), tr("Confirmations"), tr("Priority"), "" });

    // context menu actions
    selectCoins = new QAction(tr("Select coins"), this);
    deselectCoins = new QAction(tr("Deselect coins"), this);
    selectCoins->setEnabled(false);
    deselectCoins->setEnabled(false);
    auto *selectAllCoins = new QAction(tr("Select all coins"), this);
    auto *deselectAllCoins = new QAction(tr("Deselect all coins"), this);
    auto *copyAmountAction = new QAction(tr("Copy amount"), this);
    auto *copyLabelAction = new QAction(tr("Copy label"), this);
    auto *copyAddressAction = new QAction(tr("Copy address"), this);
    auto *copyTransactionAction = new QAction(tr("Copy transaction ID"), this);
    auto *lockAction = new QAction(tr("Lock unspent"), this);
    auto *unlockAction = new QAction(tr("Unlock unspent"), this);

    // context menu
    contextMenu->addAction(selectCoins);
    contextMenu->addAction(deselectCoins);
    contextMenu->addSeparator();
    contextMenu->addAction(selectAllCoins);
    contextMenu->addAction(deselectAllCoins);
    contextMenu->addSeparator();
    contextMenu->addAction(copyAddressAction);
    contextMenu->addAction(copyLabelAction);
    contextMenu->addAction(copyAmountAction);
    contextMenu->addAction(copyTransactionAction);
    contextMenu->addSeparator();
    contextMenu->addAction(lockAction);
    contextMenu->addAction(unlockAction);

    layout->addWidget(table);

    // Restore sorting preferences
    QSettings s;
    if (s.contains("nCoinControlSortColumn") && s.contains("nCoinControlSortOrder"))
        table->horizontalHeader()->setSortIndicator(s.value("nCoinControlSortColumn").toInt(),
                static_cast<Qt::SortOrder>(s.value("nCoinControlSortOrder").toInt()));

    connect(table, &QTableWidget::customContextMenuRequested, this, &ReptiloidsCoinControl::showContextMenu);
    connect(table->horizontalHeader(), &QHeaderView::sortIndicatorChanged, this, [this](int column, Qt::SortOrder order) {
        QSettings settings;
        settings.setValue("nCoinControlSortOrder", table->horizontalHeader()->sortIndicatorOrder());
        settings.setValue("nCoinControlSortColumn", table->horizontalHeader()->sortIndicatorSection());
    });

    connect(selectCoins, &QAction::triggered, this, [this]() {
        auto *select = table->selectionModel();
        if (select->hasSelection()) {
            unwatch();
            auto idxs = select->selectedRows(COLUMN_CHECKBOX);
            for (auto &idx : idxs) {
                auto *item = table->item(idx.row(), idx.column());
                if (item) {
                    UTXO *utxo = nullptr;
                    if (utxoForHash(getTransactionHash(item), getVOut(item), utxo) && utxo != nullptr && utxo->isValid()) {
                        if (!utxo->locked) { // do not select locked coins
                            utxo->checked = true;
                            item->setCheckState(Qt::Checked);
                        }
                    }
                }
            }
            watch();
            Q_EMIT tableUpdated();
        }
    });
    connect(deselectCoins, &QAction::triggered, this, [this]() {
        auto *select = table->selectionModel();
        if (select->hasSelection()) {
            unwatch();
            auto idxs = select->selectedRows(COLUMN_CHECKBOX);
            for (auto &idx : idxs) {
                auto *item = table->item(idx.row(), idx.column());
                if (item) {
                    UTXO *utxo = nullptr;
                    if (utxoForHash(getTransactionHash(item), getVOut(item), utxo) && utxo != nullptr && utxo->isValid()) {
                        if (!utxo->locked) { // do not modify locked coins
                            utxo->checked = false;
                            item->setCheckState(Qt::Unchecked);
                        }
                    }
                }
            }
            watch();
            Q_EMIT tableUpdated();
        }
    });

    connect(selectAllCoins, &QAction::triggered, this, [this]() {
        unwatch();
        for (int row = 0; row < table->rowCount(); ++row) {
            auto *item = table->item(row, COLUMN_CHECKBOX);
            if (item) {
                UTXO *utxo = nullptr;
                if (utxoForHash(getTransactionHash(item), getVOut(item), utxo) && utxo != nullptr && utxo->isValid()) {
                    if (!utxo->locked) { // do not select locked coins
                        utxo->checked = true;
                        item->setCheckState(Qt::Checked);
                    }
                }
            }
        }
        watch();
        Q_EMIT tableUpdated();
    });
    connect(deselectAllCoins, &QAction::triggered, this, [this]() {
        unwatch();
        for (int row = 0; row < table->rowCount(); ++row) {
            auto *item = table->item(row, COLUMN_CHECKBOX);
            if (item) {
                UTXO *utxo = nullptr;
                if (utxoForHash(getTransactionHash(item), getVOut(item), utxo) && utxo != nullptr && utxo->isValid()) {
                    if (!utxo->locked) { // do not select locked coins
                        utxo->checked = false;
                        item->setCheckState(Qt::Unchecked);
                    }
                }
            }
        }
        watch();
        Q_EMIT tableUpdated();
    });

    connect(copyAmountAction, &QAction::triggered, this, [this]() {
        if (contextItem) {
            UTXO *utxo = nullptr;
            if (utxoForHash(getTransactionHash(contextItem), getVOut(contextItem), utxo) && utxo != nullptr && utxo->isValid())
                setClipboard(utxo->amount);
        }
    });
    connect(copyLabelAction, &QAction::triggered, this, [this]() {
        if (contextItem) {
            UTXO *utxo = nullptr;
            if (utxoForHash(getTransactionHash(contextItem), getVOut(contextItem), utxo) && utxo != nullptr && utxo->isValid())
                setClipboard(utxo->label);
        }
    });
    connect(copyAddressAction, &QAction::triggered, this, [this]() {
        if (contextItem) {
            UTXO *utxo = nullptr;
            if (utxoForHash(getTransactionHash(contextItem), getVOut(contextItem), utxo) && utxo != nullptr && utxo->isValid())
                setClipboard(utxo->address);
        }
    });
    connect(copyTransactionAction, &QAction::triggered, this, [this]() {
        if (contextItem) {
            UTXO *utxo = nullptr;
            if (utxoForHash(getTransactionHash(contextItem), getVOut(contextItem), utxo) && utxo != nullptr && utxo->isValid())
                setClipboard(utxo->transaction);
        }
    });
    connect(lockAction, &QAction::triggered, this, [this]() {
        auto *select = table->selectionModel();
        if (select->hasSelection()) {
            unwatch();
            auto idxs = select->selectedRows(COLUMN_CHECKBOX);
            for (auto &idx : idxs) {
                auto *item = table->item(idx.row(), COLUMN_TXHASH);
                if (item) {
                    UTXO *utxo = nullptr;
                    if (utxoForHash(getTransactionHash(item), getVOut(item), utxo) && utxo != nullptr && utxo->isValid()) {
                        utxo->locked = true;
                        utxo->unlocked = !utxo->locked;
                        utxo->checked = false;
                        auto *cbItem = new QTableWidgetItem;
                        cbItem->setIcon(QIcon(":/icons/lock_closed"));
                        table->setItem(idx.row(), COLUMN_CHECKBOX, cbItem);
                    }
                }
            }
            watch();
            Q_EMIT tableUpdated();
        }
    });
    connect(unlockAction, &QAction::triggered, this, [this]() {
        auto *select = table->selectionModel();
        if (select->hasSelection()) {
            unwatch();
            auto idxs = select->selectedRows(COLUMN_CHECKBOX);
            for (auto &idx : idxs) {
                auto *item = table->item(idx.row(), COLUMN_TXHASH);
                if (item) {
                    UTXO *utxo = nullptr;
                    if (utxoForHash(getTransactionHash(item), getVOut(item), utxo) && utxo->isValid()) {
                        utxo->locked = false;
                        utxo->unlocked = !utxo->locked;
                        utxo->checked = false;
                        auto *cbItem = new QTableWidgetItem;
                        cbItem->setCheckState(Qt::Unchecked);
                        table->setItem(idx.row(), COLUMN_CHECKBOX, cbItem);
                    }
                }
            }
            watch();
            Q_EMIT tableUpdated();
        }
    });
}

void ReptiloidsCoinControl::setData(ModelPtr dataModel) {
    this->dataModel = dataModel;

    unwatch();
    table->clearContents();
    table->setRowCount(dataModel->data.count());
    table->setSortingEnabled(false);

    for (int i = 0; i < dataModel->data.count(); ++i) {
        auto *d = dataModel->data[i];

        // checkbox
        auto *cbItem = new QTableWidgetItem;
        if (d->locked)
            cbItem->setIcon(QIcon(":/icons/lock_closed"));
        else cbItem->setCheckState(d->checked ? Qt::Checked : Qt::Unchecked);
        table->setItem(i, COLUMN_CHECKBOX, cbItem);

        // amount
        auto *amountItem = new ReptiloidsCoinControl::NumberItem;
        amountItem->setData(Qt::DisplayRole, d->amount);
        table->setItem(i, COLUMN_AMOUNT, amountItem);

        // label
        auto *labelItem = new QTableWidgetItem;
        labelItem->setText(d->label);
        table->setItem(i, COLUMN_LABEL, labelItem);

        // address
        auto *addressItem = new QTableWidgetItem;
        addressItem->setText(d->address);
        table->setItem(i, COLUMN_ADDRESS, addressItem);

        // date
        auto *dateItem = new QTableWidgetItem;
        auto localDate = d->date.toLocalTime();
        dateItem->setData(Qt::DisplayRole, d->date);
        table->setItem(i, COLUMN_DATE, dateItem);

        // confirmations
        auto *confItem = new ReptiloidsCoinControl::NumberItem;
        confItem->setData(Qt::DisplayRole, QString::number(d->confirmations));
        table->setItem(i, COLUMN_CONFIRMATIONS, confItem);

        // priority
        auto *priorityItem = new ReptiloidsCoinControl::PriorityItem;
        priorityItem->setData(PriorityItem::PriorityRole, d->priority);
        priorityItem->setData(Qt::DisplayRole, getPriorityLabel(d->priority));
        table->setItem(i, COLUMN_PRIORITY, priorityItem);

        // txhash
        auto *txhashItem = new QTableWidgetItem;
        txhashItem->setData(Qt::DisplayRole, d->transaction);
        table->setItem(i, COLUMN_TXHASH, txhashItem);

        // tx vout
        auto *txvoutItem = new QTableWidgetItem;
        txvoutItem->setData(Qt::DisplayRole, d->vout);
        table->setItem(i, COLUMN_TXVOUT, txvoutItem);
    }

    table->setSortingEnabled(true);
    watch();
}

ReptiloidsCoinControl::ModelPtr ReptiloidsCoinControl::getData() {
    return dataModel;
}

void ReptiloidsCoinControl::sizeTo(const int minimumHeight, const int maximumHeight) {
    int h = dataModel ? dataModel->data.count() * 60 : minimumHeight;
    if (h > maximumHeight)
        h = maximumHeight;
    table->setFixedHeight(h);
}

void ReptiloidsCoinControl::showContextMenu(QPoint pt) {
    auto *select = table->selectionModel();
    selectCoins->setEnabled(select->hasSelection());
    deselectCoins->setEnabled(select->hasSelection());
    auto *item = table->itemAt(pt);
    if (!item) {
        contextItem = nullptr;
        return;
    }
    contextItem = item;
    contextMenu->exec(QCursor::pos());
}

void ReptiloidsCoinControl::setClipboard(const QString &str) {
    QApplication::clipboard()->setText(str, QClipboard::Clipboard);
}

/**
 * @brief  Returns the string representation of the priority.
 * @param  dPriority
 * @return
 */
QString ReptiloidsCoinControl::getPriorityLabel(double dPriority) {
    double dPriorityMedium = dataModel->mempoolPriority;

    if (dPriorityMedium <= 0)
        dPriorityMedium = dataModel->freeThreshold;

    if (dPriority / 1000000 > dPriorityMedium)
        return tr("highest");
    else if (dPriority / 100000 > dPriorityMedium)
        return tr("higher");
    else if (dPriority / 10000 > dPriorityMedium)
        return tr("high");
    else if (dPriority / 1000 > dPriorityMedium)
        return tr("medium-high");
    else if (dPriority > dPriorityMedium)
        return tr("medium");
    else if (dPriority * 10 > dPriorityMedium)
        return tr("low-medium");
    else if (dPriority * 100 > dPriorityMedium)
        return tr("low");
    else if (dPriority * 1000 > dPriorityMedium)
        return tr("lower");
    else
        return tr("lowest");
}

void ReptiloidsCoinControl::unwatch() {
    table->setEnabled(false);
    disconnect(table, &QTableWidget::itemChanged, this, &ReptiloidsCoinControl::onItemChanged);
}

void ReptiloidsCoinControl::watch() {
    table->setEnabled(true);
    connect(table, &QTableWidget::itemChanged, this, &ReptiloidsCoinControl::onItemChanged);
}

bool ReptiloidsCoinControl::utxoForHash(const QString transaction, const uint vout, UTXO *&utxo) {
    for (auto *item : dataModel->data) {
        if (transaction == item->transaction && vout == item->vout) {
            utxo = item;
            return true;
        }
    }
    return false;
}

QString ReptiloidsCoinControl::getTransactionHash(QTableWidgetItem *item) {
    return table->item(item->row(), COLUMN_TXHASH)->data(Qt::DisplayRole).toString();
}

uint ReptiloidsCoinControl::getVOut(QTableWidgetItem *item) {
    return table->item(item->row(), COLUMN_TXVOUT)->data(Qt::DisplayRole).toUInt();
}

void ReptiloidsCoinControl::onItemChanged(QTableWidgetItem *item) {
    UTXO *utxo = nullptr;
    if (utxoForHash(getTransactionHash(item), getVOut(item), utxo) && utxo != nullptr && utxo->isValid()) {
        utxo->checked = item->checkState() == Qt::Checked;
        Q_EMIT tableUpdated();
    }
}
