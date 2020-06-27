// Copyright (c) 2018-2019 The Blocknet developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <qt/reptiloidsaddressbook.h>

#include <qt/reptiloidsaddressedit.h>
#include <qt/reptiloidsavatar.h>
#include <qt/reptiloidsguiutil.h>
#include <qt/reptiloidsiconbtn.h>
#include <qt/reptiloidslabelbtn.h>

#include <qt/addresstablemodel.h>
#include <qt/guiutil.h>

#include <QHeaderView>
#include <QSettings>
#include <QTimer>

ReptiloidsAddressBookDialog::ReptiloidsAddressBookDialog(WalletModel *model, Qt::WindowFlags f, int filter, QWidget *parent) : QDialog(parent, f) {
    this->setModal(true);
    if (filter == ReptiloidsAddressBook::FILTER_DEFAULT)
        this->setMinimumSize(BGU::spi(520), BGU::spi(650));
    else this->setMinimumSize(BGU::spi(700), BGU::spi(650));
    this->setStyleSheet(GUIUtil::loadStyleSheet());
    this->setContentsMargins(QMargins());
    this->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    this->setWindowTitle(tr("Address Book"));

    auto *dialogLayout = new QVBoxLayout;
    dialogLayout->setContentsMargins(QMargins());
    this->setLayout(dialogLayout);

    auto *doneBtn = new ReptiloidsFormBtn;
    doneBtn->setText(tr("Done"));

    bool slimMode = filter == ReptiloidsAddressBook::FILTER_DEFAULT; // only slim if default filter
    form = new ReptiloidsAddressBook(slimMode, filter, this);
    form->setWalletModel(model);

    dialogLayout->addWidget(form, 1);
    dialogLayout->addWidget(doneBtn, 0, Qt::AlignCenter);
    dialogLayout->addSpacing(BGU::spi(20));

    connect(form, &ReptiloidsAddressBook::send, this, [this](const QString &address) {
        Q_EMIT send(address);
        if (ssMode)
            accept();
    });
    connect(doneBtn, &ReptiloidsFormBtn::clicked, this, [this]() {
        accept();
    });
}

void ReptiloidsAddressBookDialog::resizeEvent(QResizeEvent *evt) {
    QDialog::resizeEvent(evt);
}

ReptiloidsAddressBook::ReptiloidsAddressBook(bool slimMode, int filter, QWidget *parent) : QFrame(parent),
                                                                                       slimMode(slimMode),
                                                                                       layout(new QVBoxLayout) {
    this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    if (slimMode || filter != FILTER_DEFAULT)
        layout->setContentsMargins(0, BGU::spi(10), 0, 0);
    else layout->setContentsMargins(BGU::spi(46), BGU::spi(10), BGU::spi(50), 0);
    this->setLayout(layout);

    titleLbl = new QLabel(tr("Address Book"));
    titleLbl->setObjectName("h4");
    titleLbl->setFixedHeight(BGU::spi(26));
    if (slimMode || filter != FILTER_DEFAULT)
        titleLbl->hide();

    auto *topBox = new QFrame;
    topBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::MinimumExpanding);
    auto *topBoxLayout = new QHBoxLayout;
    topBoxLayout->setContentsMargins(QMargins());
    topBoxLayout->setSizeConstraint(QLayout::SetMaximumSize);
    topBox->setLayout(topBoxLayout);

    auto *addAddressBtn = new ReptiloidsIconBtn(":/redesign/QuickActions/AddressButtonIcon.png");

    addButtonLbl = new QLabel(tr("New Address"));
    addButtonLbl->setObjectName("h4");

    filterLbl = new QLabel(tr("Filter by:"));
    filterLbl->setObjectName("title");

    addressDropdown = new ReptiloidsDropdown;
    addressDropdown->addItem(tr("All Addresses"), FILTER_ALL);
    addressDropdown->addItem(tr("Contacts"),      FILTER_SENDING);
    addressDropdown->addItem(tr("My Addresses"),  FILTER_RECEIVING);

    table = new QTableWidget;
    table->setContentsMargins(QMargins());
    table->setColumnCount(COLUMN_DELETE + 1);
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    table->setSelectionBehavior(QAbstractItemView::SelectRows);
    table->setSelectionMode(QAbstractItemView::SingleSelection);
    table->setFocusPolicy(Qt::NoFocus);
    table->setAlternatingRowColors(true);
    table->setColumnWidth(COLUMN_ACTION, BGU::spi(50));
    table->setColumnWidth(COLUMN_AVATAR, BGU::spi(50));
    table->setColumnWidth(COLUMN_COPY, BGU::spi(65));
    table->setColumnWidth(COLUMN_EDIT, BGU::spi(65));
    table->setColumnWidth(COLUMN_DELETE, BGU::spi(65));
    table->setShowGrid(false);
    table->setFocusPolicy(Qt::NoFocus);
    table->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
    table->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    table->setContextMenuPolicy(Qt::CustomContextMenu);
    table->verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    table->verticalHeader()->setDefaultSectionSize(BGU::spi(78));
    table->verticalHeader()->setVisible(false);
    table->horizontalHeader()->setDefaultAlignment(Qt::AlignLeft);
    table->horizontalHeader()->setSortIndicatorShown(true);
    table->horizontalHeader()->setSectionsClickable(true);
    table->horizontalHeader()->setSectionResizeMode(COLUMN_ACTION, QHeaderView::Fixed);
    table->horizontalHeader()->setSectionResizeMode(COLUMN_AVATAR, QHeaderView::Fixed);
    table->horizontalHeader()->setSectionResizeMode(COLUMN_ALIAS, QHeaderView::ResizeToContents);
    table->horizontalHeader()->setSectionResizeMode(COLUMN_ADDRESS, QHeaderView::ResizeToContents);
    table->horizontalHeader()->setSectionResizeMode(COLUMN_COPY, QHeaderView::Fixed);
    table->horizontalHeader()->setSectionResizeMode(COLUMN_EDIT, QHeaderView::Fixed);
    table->horizontalHeader()->setSectionResizeMode(COLUMN_DELETE, QHeaderView::Fixed);
    table->horizontalHeader()->setStretchLastSection(true);
    table->setHorizontalHeaderLabels({ "", "", tr("Alias"), tr("Address"), "", "", "" });

    // If in slim mode, hide all columns except add, alias, and address
    if (slimMode) {
        table->setColumnHidden(COLUMN_COPY, true);
        table->setColumnHidden(COLUMN_EDIT, true);
        table->setColumnHidden(COLUMN_DELETE, true);
    }

    if (!slimMode) {
        if (filter != FILTER_DEFAULT)
            topBoxLayout->addSpacing(BGU::spi(30));
        topBoxLayout->addWidget(addAddressBtn, 0, Qt::AlignLeft);
        topBoxLayout->addWidget(addButtonLbl, 0, Qt::AlignLeft | Qt::AlignVCenter);
        topBoxLayout->addStretch(1);
    }
    topBoxLayout->addWidget(filterLbl, 0, Qt::AlignRight);
    topBoxLayout->addWidget(addressDropdown, 0, Qt::AlignRight);
    if (filter != FILTER_DEFAULT)
        topBoxLayout->addSpacing(BGU::spi(30));

    layout->addWidget(titleLbl);
    layout->addSpacing(BGU::spi(10));
    if (!slimMode)
        layout->addWidget(topBox);
    else layout->addWidget(topBox, 0, Qt::AlignCenter);
    layout->addSpacing(BGU::spi(15));
    layout->addWidget(table, 1);
    layout->addSpacing(BGU::spi(20));

    // Set the filter dropdown state
    if (filter == FILTER_DEFAULT) {
        int ddIndex = 0;
        QSettings settings;
        if (!slimMode)
            ddIndex = settings.value("reptiloidsAddressBookFilter").toInt();
        else ddIndex = settings.value("reptiloidsAddressBookFilterSlim").toInt();
        addressDropdown->setCurrentIndex(ddIndex);
    }
    else {
        addressDropdown->setCurrentIndex(ddIndexForType(filter));
    }
    filteredOption = addressDropdown->currentData().toInt();

    connect(addAddressBtn, &ReptiloidsIconBtn::clicked, this, &ReptiloidsAddressBook::onAddAddress);
    connect(addressDropdown, &ReptiloidsDropdown::valueChanged, this, &ReptiloidsAddressBook::onFilter);
    connect(table, &QTableWidget::cellDoubleClicked, this, &ReptiloidsAddressBook::onDoubleClick);
}

void ReptiloidsAddressBook::setWalletModel(WalletModel *w) {
    if (walletModel == w)
        return;

    walletModel = w;
    if (!walletModel || !walletModel->getOptionsModel())
        return;

    initialize();

    connect(walletModel->getAddressTableModel(), &AddressTableModel::rowsInserted, this,
        [this](const QModelIndex &, int, int) {
            initialize();
        });
    connect(walletModel->getAddressTableModel(), &AddressTableModel::rowsRemoved, this,
        [this](const QModelIndex &, int, int) {
            initialize();
        });
    connect(walletModel->getAddressTableModel(), &AddressTableModel::dataChanged, this,
        [this](const QModelIndex &, const QModelIndex &, const QVector<int> &) {
            initialize();
        });
}

void ReptiloidsAddressBook::initialize() {
    if (!walletModel)
        return;

    dataModel.clear();

    AddressTableModel *addressTableModel = walletModel->getAddressTableModel();
    int rowCount = addressTableModel->rowCount(QModelIndex());

    for (int row=0; row<rowCount; row++) {
        auto index = addressTableModel->index(row, 0, QModelIndex());
        auto *rec = static_cast<AddressTableEntry*>(index.internalPointer());
        if (!rec)
            continue;
        QString alias = rec->label;
        QString address = rec->address;
        int type = rec->type;
        Address a = {
            alias,
            address,
            type
        };
        dataModel << a;
    }

    // Sort on alias descending
    std::sort(dataModel.begin(), dataModel.end(), [](const Address &a, const Address &b) {
        return a.alias > b.alias;
    });

    this->setData(filtered(dataModel, filteredOption));
}

void ReptiloidsAddressBook::unwatch() {
    table->setEnabled(false);
}

void ReptiloidsAddressBook::watch() {
    table->setEnabled(true);
}

QVector<ReptiloidsAddressBook::Address> ReptiloidsAddressBook::filtered(const QVector<Address> &data, int filter) {
    QVector<Address> r;
    for (auto &d : data) {
        switch (filter) {
            case AddressTableEntry::Sending:
                if (d.type == AddressTableEntry::Sending)
                    r.push_back(d);
                break;
            case AddressTableEntry::Receiving:
                if (d.type == AddressTableEntry::Receiving)
                    r.push_back(d);
                break;
            case FILTER_ALL: // must be 2 or greater
            default:
                r.push_back(d);
                break;
        }
    }
    return r;
}

void ReptiloidsAddressBook::setData(const QVector<Address> &data) {
    this->filteredData = data;

    unwatch();
    table->clearContents();
    table->setRowCount(this->filteredData.count());
    table->setSortingEnabled(false);

    for (int i = 0; i < this->filteredData.count(); ++i) {
        auto &d = this->filteredData[i];

        // action item
        auto *actionItem = new QTableWidgetItem;
        actionItem->setToolTip(!slimMode ? tr("Send funds to this address") : tr("Add this address"));
        auto *widget = new QWidget();
        widget->setContentsMargins(QMargins());
        auto *boxLayout = new QVBoxLayout;
        boxLayout->setContentsMargins(QMargins());
        boxLayout->setSpacing(0);
        widget->setLayout(boxLayout);

        auto *button = new ReptiloidsActionBtn;
        button->setID(d.address);
        boxLayout->addWidget(button, 0, Qt::AlignCenter);
        connect(button, &ReptiloidsActionBtn::clicked, this, &ReptiloidsAddressBook::onAddressAction);

        table->setCellWidget(i, COLUMN_ACTION, widget);
        table->setItem(i, COLUMN_ACTION, actionItem);

        // avatar
        auto *avatarItem = new QTableWidgetItem;
        auto *avatarWidget = new QWidget();
        avatarWidget->setContentsMargins(QMargins());
        auto *avatarLayout = new QVBoxLayout;
        avatarLayout->setContentsMargins(QMargins());
        avatarLayout->setSpacing(0);
        avatarWidget->setLayout(avatarLayout);

        auto *avatar = d.type == AddressTableEntry::Sending ? new ReptiloidsAvatar(d.address)
                                                            : new ReptiloidsAvatarBlue(d.address);
        avatarLayout->addWidget(avatar, 0, Qt::AlignCenter);
        table->setCellWidget(i, COLUMN_AVATAR, avatarWidget);
        table->setItem(i, COLUMN_AVATAR, avatarItem);

        // alias
        auto *aliasItem = new QTableWidgetItem;
        aliasItem->setData(Qt::DisplayRole, d.alias);
        table->setItem(i, COLUMN_ALIAS, aliasItem);

        // address
        auto *addressItem = new QTableWidgetItem;
        addressItem->setData(Qt::DisplayRole, d.address);
        table->setItem(i, COLUMN_ADDRESS, addressItem);

        // copy item
        auto *copyItem = new QTableWidgetItem;
        copyItem->setToolTip(tr("Copy address"));
        auto *copyWidget = new QWidget();
        copyWidget->setContentsMargins(QMargins());
        auto *copyLayout = new QVBoxLayout;
        copyLayout->setContentsMargins(QMargins());
        copyLayout->setSpacing(0);
        copyWidget->setLayout(copyLayout);

        auto *copyButton = new ReptiloidsLabelBtn;
        copyButton->setText(tr("Copy"));
        copyButton->setID(d.address);
        copyLayout->addWidget(copyButton, 0, Qt::AlignLeft);
        copyLayout->addSpacing(BGU::spi(6));
        connect(copyButton, &ReptiloidsLabelBtn::clicked, this, &ReptiloidsAddressBook::onCopyAddress);

        table->setCellWidget(i, COLUMN_COPY, copyWidget);
        table->setItem(i, COLUMN_COPY, copyItem);

        // edit item
        auto *editItem = new QTableWidgetItem;
        editItem->setToolTip(tr("Edit address alias"));
        auto *editWidget = new QWidget();
        editWidget->setContentsMargins(QMargins());
        auto *editLayout = new QVBoxLayout;
        editLayout->setContentsMargins(QMargins());
        editLayout->setSpacing(0);
        editWidget->setLayout(editLayout);

        auto *editButton = new ReptiloidsLabelBtn;
        editButton->setText(tr("Edit"));
        editButton->setID(d.address);
        editLayout->addWidget(editButton, 0, Qt::AlignLeft);
        editLayout->addSpacing(BGU::spi(6));
        connect(editButton, &ReptiloidsLabelBtn::clicked, this, &ReptiloidsAddressBook::onEditAddress);

        table->setCellWidget(i, COLUMN_EDIT, editWidget);
        table->setItem(i, COLUMN_EDIT, editItem);

        // delete item
        auto *deleteItem = new QTableWidgetItem;
        deleteItem->setToolTip(tr("Delete this address"));
        auto *deleteWidget = new QWidget();
        deleteWidget->setContentsMargins(QMargins());
        auto *deleteLayout = new QVBoxLayout;
        deleteLayout->setContentsMargins(QMargins());
        deleteLayout->setSpacing(0);
        deleteWidget->setLayout(deleteLayout);

        // Can only delete sending addresses
        if (d.type == AddressTableEntry::Sending) {
            auto *deleteButton = new ReptiloidsLabelBtn;
            deleteButton->setText(tr("Delete"));
            deleteButton->setID(d.address);
            deleteLayout->addWidget(deleteButton, 0, Qt::AlignLeft);
            deleteLayout->addSpacing(BGU::spi(6));
            connect(deleteButton, &ReptiloidsLabelBtn::clicked, this, &ReptiloidsAddressBook::onDeleteAddress);
        }

        table->setCellWidget(i, COLUMN_DELETE, deleteWidget);
        table->setItem(i, COLUMN_DELETE, deleteItem);
    }

    table->setSortingEnabled(true);
    watch();
}

/**
 * @brief Filters the data model based on the current filter dropdown filter flag.
 */
void ReptiloidsAddressBook::onFilter() {
    filteredOption = addressDropdown->currentData().toInt();
    setData(filtered(dataModel, filteredOption));
    QSettings settings;
    if (!slimMode)
        settings.setValue("reptiloidsAddressBookFilter", addressDropdown->currentIndex());
    else settings.setValue("reptiloidsAddressBookFilterSlim", addressDropdown->currentIndex());
}

void ReptiloidsAddressBook::onCopyAddress() {
    auto *btn = qobject_cast<ReptiloidsLabelBtn*>(sender());
    auto address = btn->getID();
    GUIUtil::setClipboard(address);
}

void ReptiloidsAddressBook::onAddAddress() {
    ReptiloidsAddressAddDialog dlg(walletModel->getAddressTableModel(), walletModel, Qt::WindowSystemMenuHint | Qt::WindowTitleHint);
    dlg.setStyleSheet(GUIUtil::loadStyleSheet());
    connect(&dlg, &QDialog::accepted, this, [this, &dlg]() {
        // If the user added a new private key, ask them if they want to perform a wallet rescan
        if (!dlg.form->getKey().isEmpty()) {
            QMessageBox::StandardButton retval = QMessageBox::question(this->parentWidget(), tr("Rescan the wallet"),
                                      tr("You imported a new wallet address. Would you like to rescan the blockchain to add coin associated with this address? If you don't rescan, you may not see all your coin.\n\nThis may take several minutes."),
                                      QMessageBox::Yes | QMessageBox::No,
                                      QMessageBox::No);

            if (retval != QMessageBox::Yes)
                return;

            walletModel->showProgress(tr("Rescanning..."), 0);
            // TODO Reptiloids Qt wallet rescan after add private key
//            QTimer::singleShot(1000, [this]() {
//                walletModel->RescanFromTime();
//            });
        }

    });
    dlg.exec();
}

void ReptiloidsAddressBook::onEditAddress() {
    auto *btn = qobject_cast<ReptiloidsLabelBtn*>(sender());
    Address data;
    data.address = btn->getID();
    // Remove address from data model
    auto rows = walletModel->getAddressTableModel()->rowCount(QModelIndex());
    for (int row = rows - 1; row >= 0; --row) {
        auto index = walletModel->getAddressTableModel()->index(row, 0, QModelIndex());
        auto *rec = static_cast<AddressTableEntry*>(index.internalPointer());
        if (rec && data.address == rec->address) {
            data.alias = rec->label;
            data.type = rec->type;
            break;
        }
    }
    ReptiloidsAddressEditDialog dlg(walletModel->getAddressTableModel(), walletModel, Qt::WindowSystemMenuHint | Qt::WindowTitleHint);
    dlg.setData(data.address, data.alias, data.type, QString());
    dlg.exec();
}

void ReptiloidsAddressBook::onDoubleClick(int row, int col) {
    if (row >= filteredData.size()) // check index
        return;
    auto data = filteredData[row];
    if (!slimMode) {
        ReptiloidsAddressEditDialog dlg(walletModel->getAddressTableModel(), walletModel, Qt::WindowSystemMenuHint | Qt::WindowTitleHint);
        dlg.setData(data.address, data.alias, data.type, QString());
        dlg.exec();
    } else {
        Q_EMIT send(data.address);
    }
}

void ReptiloidsAddressBook::onDeleteAddress() {
    auto *btn = qobject_cast<ReptiloidsLabelBtn*>(sender());
    auto address = btn->getID();
    QMessageBox::StandardButton retval = QMessageBox::question(this, tr("Delete Address"),
                                                               QString("%1\n\n%2").arg(tr("Are you sure you want to delete this address?"), address),
                                                               QMessageBox::Yes | QMessageBox::No,
                                                               QMessageBox::No);

    if (retval != QMessageBox::Yes)
        return;

    // Remove address from data model
    auto rows = walletModel->getAddressTableModel()->rowCount(QModelIndex());
    for (int row = rows - 1; row >= 0; --row) {
        auto index = walletModel->getAddressTableModel()->index(row, AddressTableModel::Address, QModelIndex());
        if (address == index.data(Qt::EditRole).toString()) {
            walletModel->getAddressTableModel()->removeRows(index.row(), 1, index.parent());
            break;
        }
    }
}

void ReptiloidsAddressBook::onAddressAction() {
    auto *btn = qobject_cast<ReptiloidsActionBtn*>(sender());
    auto address = btn->getID();
    Q_EMIT send(address);
}

int ReptiloidsAddressBook::ddIndexForType(int type) {
    switch (type) {
        case FILTER_SENDING:
            return 1;
        case FILTER_RECEIVING:
            return 2;
        case FILTER_ALL:
        default:
            return 0;
    }
}
