// Copyright (c) 2018-2019 The Blocknet developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <qt/reptiloidscointransactionhistory.h>

#include <qt/reptiloidscoinformbtn.h>
#include <qt/reptiloidscoinguiutil.h>
#include <qt/reptiloidscoinsendfundsutil.h>
#include <qt/reptiloidscoinvars.h>

#include <qt/bitcoinunits.h>
#include <qt/csvmodelwriter.h>
#include <qt/guiutil.h>
#include <qt/transactiondescdialog.h>
#include <qt/transactionrecord.h>
#include <qt/transactiontablemodel.h>

#include <QAbstractItemView>
#include <QDateTime>
#include <QHeaderView>
#include <QKeyEvent>
#include <QSettings>

ReptiloidsCoinTransactionHistory::ReptiloidsCoinTransactionHistory(WalletModel *w, QWidget *parent) : QFrame(parent), walletModel(w),
                                                                                          layout(new QVBoxLayout) {
//    this->setStyleSheet("border: 1px solid red");
    this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    this->setLayout(layout);
    layout->setContentsMargins(BGU::spi(45), BGU::spi(10), BGU::spi(45), BGU::spi(30));

    displayUnit = walletModel->getOptionsModel()->getDisplayUnit();
    auto displayUnitName = BitcoinUnits::longName(displayUnit);

    auto *titleBox = new QFrame;
    titleBox->setContentsMargins(QMargins());
    auto *titleBoxLayout = new QHBoxLayout;
    titleBox->setLayout(titleBoxLayout);
    titleBoxLayout->setContentsMargins(QMargins());
    titleLbl = new QLabel(tr("Transaction History"));
    titleLbl->setObjectName("h4");
    auto *exportBtn = new ReptiloidsCoinFormBtn;
    exportBtn->setText(tr("Export History"));
    titleBoxLayout->addWidget(titleLbl, 0, Qt::AlignTop | Qt::AlignLeft);
    titleBoxLayout->addStretch(1);
    titleBoxLayout->addWidget(exportBtn, 0, Qt::AlignRight);

    auto *subtitleLbl = new QLabel;
    subtitleLbl->setObjectName("h2");

    auto *searchBox = new QFrame;
    searchBox->setContentsMargins(QMargins());
    searchBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    auto *searchBoxLayout = new QHBoxLayout;
    searchBoxLayout->setContentsMargins(QMargins());
    searchBox->setLayout(searchBoxLayout);

    dateCb = new ReptiloidsCoinDropdown;
    dateCb->addItem(tr("All"),        All);
    dateCb->addItem(tr("Today"),      Today);
    dateCb->addItem(tr("This week"),  ThisWeek);
    dateCb->addItem(tr("This month"), ThisMonth);
    dateCb->addItem(tr("Last month"), LastMonth);
    dateCb->addItem(tr("This year"),  ThisYear);
    dateCb->addItem(tr("Range..."),   Range);

    typeCb = new ReptiloidsCoinDropdown;
    typeCb->addItem(tr("All"),            ReptiloidsCoinTransactionHistoryFilterProxy::ALL_TYPES);
    typeCb->addItem(tr("Most Common"),    ReptiloidsCoinTransactionHistoryFilterProxy::COMMON_TYPES);
    typeCb->addItem(tr("Received"),       ReptiloidsCoinTransactionHistoryFilterProxy::TYPE(TransactionRecord::RecvWithAddress) | ReptiloidsCoinTransactionHistoryFilterProxy::TYPE(TransactionRecord::RecvFromOther));
    typeCb->addItem(tr("Sent"),           ReptiloidsCoinTransactionHistoryFilterProxy::TYPE(TransactionRecord::SendToAddress) | ReptiloidsCoinTransactionHistoryFilterProxy::TYPE(TransactionRecord::SendToOther));
    typeCb->addItem(tr("To yourself"),    ReptiloidsCoinTransactionHistoryFilterProxy::TYPE(TransactionRecord::SendToSelf));
    typeCb->addItem(tr("Staked"),         ReptiloidsCoinTransactionHistoryFilterProxy::TYPE(TransactionRecord::Generated));
    typeCb->addItem(tr("Other"),          ReptiloidsCoinTransactionHistoryFilterProxy::TYPE(TransactionRecord::Other));

    addressTi = new ReptiloidsCoinLineEdit;
    addressTi->setParent(this);
    addressTi->setPlaceholderText(tr("Enter address or label to search"));

    amountTi = new ReptiloidsCoinLineEdit;
    amountTi->setFixedWidth(BGU::spi(120));
    amountTi->setParent(this);
    amountTi->setPlaceholderText(tr("Min amount"));
    amountTi->setValidator(new ReptiloidsCoinNumberValidator(0, REPTILOIDSCOINGUI_FUNDS_MAX, BitcoinUnits::decimals(displayUnit)));
    amountTi->setMaxLength(REPTILOIDSCOINGUI_MAXCHARS);

    searchBoxLayout->addWidget(dateCb);
    searchBoxLayout->addWidget(typeCb);
    searchBoxLayout->addWidget(addressTi);
    searchBoxLayout->addWidget(amountTi);

    dateRangeWidget = new QFrame();
    dateRangeWidget->setObjectName("dateRange");
    dateRangeWidget->setContentsMargins(1, 1, 1, 1);
    auto *dateRangeWidgetLayout = new QHBoxLayout(dateRangeWidget);
    dateRangeWidgetLayout->setContentsMargins(QMargins());
    dateRangeWidgetLayout->addWidget(new QLabel(tr("Date from")));
    dateRangeWidget->setVisible(false); // Hide by default

    dateFrom = new QDateTimeEdit;
    dateFrom->setFixedSize(BGU::spi(120), BGU::spi(30));
    dateFrom->setDisplayFormat("MM/dd/yy");
    dateFrom->setCalendarPopup(true);
    dateFrom->setMinimumWidth(BGU::spi(100));
    dateFrom->setDate(QDate::currentDate().addDays(-7));
    dateRangeWidgetLayout->addWidget(dateFrom);
    dateRangeWidgetLayout->addWidget(new QLabel(tr("to")));

    dateTo = new QDateTimeEdit;
    dateTo->setFixedSize(BGU::spi(120), BGU::spi(30));
    dateTo->setDisplayFormat("MM/dd/yy");
    dateTo->setCalendarPopup(true);
    dateTo->setMinimumWidth(BGU::spi(100));
    dateTo->setDate(QDate::currentDate());
    dateRangeWidgetLayout->addWidget(dateTo);
    dateRangeWidgetLayout->addStretch();

    transactionsTbl = new ReptiloidsCoinTransactionHistoryTable(this);
    transactionsTbl->setObjectName("transactionHistory");
    transactionsTbl->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    transactionsTbl->setEditTriggers(QAbstractItemView::NoEditTriggers);
    transactionsTbl->setSelectionBehavior(QAbstractItemView::SelectRows);
    transactionsTbl->setSelectionMode(QAbstractItemView::ExtendedSelection);
    transactionsTbl->setAlternatingRowColors(true);
    transactionsTbl->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    transactionsTbl->setShowGrid(false);
    transactionsTbl->setContextMenuPolicy(Qt::CustomContextMenu);
    transactionsTbl->setSortingEnabled(true);
    transactionsTbl->verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    transactionsTbl->verticalHeader()->setDefaultSectionSize(BGU::spi(35));
    transactionsTbl->verticalHeader()->setVisible(false);
    transactionsTbl->horizontalHeader()->setDefaultAlignment(Qt::AlignLeft);
    transactionsTbl->horizontalHeader()->setSortIndicatorShown(true);
    transactionsTbl->horizontalHeader()->setSectionsClickable(true);

    // Displays total amount selected
    totalSelectedLbl = new QLabel;
    totalSelectedLbl->setObjectName("h6");
    totalSelectedLbl->hide();

    layout->addWidget(titleBox);
    layout->addSpacing(BGU::spi(25));
    layout->addWidget(searchBox);
    layout->addWidget(dateRangeWidget);
    layout->addWidget(transactionsTbl);
    layout->addWidget(totalSelectedLbl, 0, Qt::AlignRight);

    // Actions
    auto *copyAddressAction = new QAction(tr("Copy address"), this);
    auto *copyLabelAction = new QAction(tr("Copy label"), this);
    auto *copyAmountAction = new QAction(tr("Copy amount"), this);
    auto *copyTxIDAction = new QAction(tr("Copy transaction ID"), this);
    auto *showDetailsAction = new QAction(tr("Show transaction details"), this);

    contextMenu = new QMenu;
    contextMenu->addAction(copyAddressAction);
    contextMenu->addAction(copyLabelAction);
    contextMenu->addAction(copyAmountAction);
    contextMenu->addAction(copyTxIDAction);
    contextMenu->addAction(showDetailsAction);

    // Set transaction data and adjust section sizes
    transactionsTbl->setWalletModel(walletModel);
    transactionsTbl->horizontalHeader()->setSectionResizeMode(ReptiloidsCoinTransactionHistoryFilterProxy::HistoryStatus, QHeaderView::Fixed);
    transactionsTbl->horizontalHeader()->setSectionResizeMode(ReptiloidsCoinTransactionHistoryFilterProxy::HistoryDate, QHeaderView::Fixed);
    transactionsTbl->horizontalHeader()->setSectionResizeMode(ReptiloidsCoinTransactionHistoryFilterProxy::HistoryTime, QHeaderView::Fixed);
    transactionsTbl->horizontalHeader()->setSectionResizeMode(ReptiloidsCoinTransactionHistoryFilterProxy::HistoryToAddress, QHeaderView::Stretch);
    transactionsTbl->horizontalHeader()->setSectionResizeMode(ReptiloidsCoinTransactionHistoryFilterProxy::HistoryType, QHeaderView::ResizeToContents);
    transactionsTbl->horizontalHeader()->setSectionResizeMode(ReptiloidsCoinTransactionHistoryFilterProxy::HistoryAmount, QHeaderView::ResizeToContents);
    transactionsTbl->setColumnWidth(ReptiloidsCoinTransactionHistoryFilterProxy::HistoryStatus, BGU::spi(3));
    transactionsTbl->setColumnWidth(ReptiloidsCoinTransactionHistoryFilterProxy::HistoryDate, BGU::spi(105));
    transactionsTbl->setColumnWidth(ReptiloidsCoinTransactionHistoryFilterProxy::HistoryTime, BGU::spi(72));

    // Restore from last visit
    QSettings settings;
    typeCb->setCurrentIndex(settings.value("transactionType").toInt());
    dateCb->setCurrentIndex(settings.value("transactionDate").toInt());
    dateChanged(settings.value("transactionDate").toInt());
    typeChanged(settings.value("transactionType").toInt());

    connect(copyAddressAction, &QAction::triggered, this, &ReptiloidsCoinTransactionHistory::copyAddress);
    connect(copyLabelAction, &QAction::triggered, this, &ReptiloidsCoinTransactionHistory::copyLabel);
    connect(copyAmountAction, &QAction::triggered, this, &ReptiloidsCoinTransactionHistory::copyAmount);
    connect(copyTxIDAction, &QAction::triggered, this, &ReptiloidsCoinTransactionHistory::copyTxID);
    connect(showDetailsAction, &QAction::triggered, this, &ReptiloidsCoinTransactionHistory::showDetails);
    connect(dateFrom, &QDateTimeEdit::dateChanged, this, &ReptiloidsCoinTransactionHistory::dateRangeChanged);
    connect(dateTo, &QDateTimeEdit::dateChanged, this, &ReptiloidsCoinTransactionHistory::dateRangeChanged);
    connect(exportBtn, &ReptiloidsCoinFormBtn::clicked, this, &ReptiloidsCoinTransactionHistory::exportClicked);
    connect(transactionsTbl->selectionModel(), &QItemSelectionModel::selectionChanged, this,
            &ReptiloidsCoinTransactionHistory::displayTotalSelected);
}

void ReptiloidsCoinTransactionHistory::showEvent(QShowEvent *event) {
    QWidget::showEvent(event);
    connect(walletModel->getOptionsModel(), &OptionsModel::displayUnitChanged, this, &ReptiloidsCoinTransactionHistory::onDisplayUnit);
    connect(addressTi, &ReptiloidsCoinLineEdit::textChanged, this, &ReptiloidsCoinTransactionHistory::addressChanged);
    connect(amountTi, &ReptiloidsCoinLineEdit::textChanged, this, &ReptiloidsCoinTransactionHistory::amountChanged);
    connect(dateCb, static_cast<void(ReptiloidsCoinDropdown::*)(int)>(&ReptiloidsCoinDropdown::activated), this, &ReptiloidsCoinTransactionHistory::dateChanged);
    connect(typeCb, static_cast<void(ReptiloidsCoinDropdown::*)(int)>(&ReptiloidsCoinDropdown::activated), this, &ReptiloidsCoinTransactionHistory::typeChanged);
    connect(transactionsTbl, &ReptiloidsCoinTransactionHistoryTable::doubleClicked, this, &ReptiloidsCoinTransactionHistory::showDetails);
    connect(transactionsTbl, &ReptiloidsCoinTransactionHistoryTable::customContextMenuRequested, this, &ReptiloidsCoinTransactionHistory::contextualMenu);
    onDisplayUnit(walletModel->getOptionsModel()->getDisplayUnit());
}

void ReptiloidsCoinTransactionHistory::hideEvent(QHideEvent *event) {
    QWidget::hideEvent(event);
    disconnect(walletModel->getOptionsModel(), &OptionsModel::displayUnitChanged, this, &ReptiloidsCoinTransactionHistory::onDisplayUnit);
    disconnect(addressTi, &ReptiloidsCoinLineEdit::textChanged, this, &ReptiloidsCoinTransactionHistory::addressChanged);
    disconnect(amountTi, &ReptiloidsCoinLineEdit::textChanged, this, &ReptiloidsCoinTransactionHistory::amountChanged);
    connect(dateCb, static_cast<void(ReptiloidsCoinDropdown::*)(int)>(&ReptiloidsCoinDropdown::activated), this, &ReptiloidsCoinTransactionHistory::dateChanged);
    connect(typeCb, static_cast<void(ReptiloidsCoinDropdown::*)(int)>(&ReptiloidsCoinDropdown::activated), this, &ReptiloidsCoinTransactionHistory::typeChanged);
    disconnect(transactionsTbl, &ReptiloidsCoinTransactionHistoryTable::doubleClicked, this, &ReptiloidsCoinTransactionHistory::showDetails);
    disconnect(transactionsTbl, &ReptiloidsCoinTransactionHistoryTable::customContextMenuRequested, this, &ReptiloidsCoinTransactionHistory::contextualMenu);
}

void ReptiloidsCoinTransactionHistory::onDisplayUnit(int unit) {
    displayUnit = unit;
    amountTi->setValidator(new ReptiloidsCoinNumberValidator(0, REPTILOIDSCOINGUI_FUNDS_MAX, BitcoinUnits::decimals(displayUnit)));
}

void ReptiloidsCoinTransactionHistory::addressChanged(const QString &prefix) {
    transactionsTbl->setAddressPrefix(prefix);
}

void ReptiloidsCoinTransactionHistory::amountChanged(const QString &amount) {
    CAmount amount_parsed = 0;
    QString newAmount = amount;
    newAmount.replace(QString(","), QString("."));

    if (BitcoinUnits::parse(displayUnit, newAmount, &amount_parsed)) {
        transactionsTbl->setMinAmount(amount_parsed);
    } else {
        transactionsTbl->setMinAmount(0);
    }
}

void ReptiloidsCoinTransactionHistory::dateChanged(int idx) {
    QDate current = QDate::currentDate();
    dateRangeWidget->setVisible(false);
    switch (dateCb->itemData(idx).toInt()) {
        case All:
            transactionsTbl->setDateRange(
                    ReptiloidsCoinTransactionHistoryFilterProxy::MIN_DATE,
                    ReptiloidsCoinTransactionHistoryFilterProxy::MAX_DATE);
            break;
        case Today:
            transactionsTbl->setDateRange(
                    QDateTime(current),
                    ReptiloidsCoinTransactionHistoryFilterProxy::MAX_DATE);
            break;
        case ThisWeek: {
            // Find last Monday
            QDate startOfWeek = current.addDays(-(current.dayOfWeek() - 1));
            transactionsTbl->setDateRange(
                    QDateTime(startOfWeek),
                    ReptiloidsCoinTransactionHistoryFilterProxy::MAX_DATE);

        } break;
        case ThisMonth:
            transactionsTbl->setDateRange(
                    QDateTime(QDate(current.year(), current.month(), 1)),
                    ReptiloidsCoinTransactionHistoryFilterProxy::MAX_DATE);
            break;
        case LastMonth:
            transactionsTbl->setDateRange(
                    QDateTime(QDate(current.year(), current.month() - 1, 1)),
                    QDateTime(QDate(current.year(), current.month(), 1)));
            break;
        case ThisYear:
            transactionsTbl->setDateRange(
                    QDateTime(QDate(current.year(), 1, 1)),
                    ReptiloidsCoinTransactionHistoryFilterProxy::MAX_DATE);
            break;
        case Range:
            dateRangeWidget->setVisible(true);
            dateRangeChanged();
            break;
    }
    if (dateCb->itemData(idx).toInt() != Range) {
        QSettings settings;
        settings.setValue("transactionDate", idx);
    }
}

void ReptiloidsCoinTransactionHistory::typeChanged(int idx) {
    transactionsTbl->setTypeFilter(static_cast<quint32>(typeCb->itemData(idx).toInt()));
    QSettings settings;
    settings.setValue("transactionType", idx);
}

void ReptiloidsCoinTransactionHistory::dateRangeChanged() {
    transactionsTbl->setDateRange(QDateTime(dateFrom->date()),
                                  QDateTime(dateTo->date()).addDays(1));
}

void ReptiloidsCoinTransactionHistory::contextualMenu(const QPoint& point) {
    auto index = transactionsTbl->indexAt(point);
    if (index.isValid())
        contextMenu->exec(QCursor::pos());
}

void ReptiloidsCoinTransactionHistory::copyAddress() {
    GUIUtil::copyEntryData(transactionsTbl, 0, TransactionTableModel::AddressRole);
}

void ReptiloidsCoinTransactionHistory::copyLabel() {
    GUIUtil::copyEntryData(transactionsTbl, 0, TransactionTableModel::LabelRole);
}

void ReptiloidsCoinTransactionHistory::copyAmount() {
    GUIUtil::copyEntryData(transactionsTbl, 0, TransactionTableModel::FormattedAmountRole);
}

void ReptiloidsCoinTransactionHistory::copyTxID() {
    GUIUtil::copyEntryData(transactionsTbl, 0, TransactionTableModel::TxHashRole);
}

void ReptiloidsCoinTransactionHistory::showDetails() {
    auto selection = transactionsTbl->selectionModel()->selectedRows();
    if (!selection.isEmpty()) {
        TransactionDescDialog dlg(selection.at(0));
        dlg.setStyleSheet(GUIUtil::loadStyleSheet());
        dlg.exec();
    }
}

void ReptiloidsCoinTransactionHistory::exportClicked() {
    // CSV is currently the only supported format
    QString filename = GUIUtil::getSaveFileName(this, tr("Export Transaction History"), QString(),
                                                tr("Comma separated file (*.csv)"), NULL);

    if (filename.isNull())
        return;

    CSVModelWriter writer(filename);

    // name, column, role
    writer.setModel(transactionsTbl->model());
    writer.addColumn(tr("Confirmed"), 0, TransactionTableModel::ConfirmedRole);
//    if (model && model->haveWatchOnly())
//        writer.addColumn(tr("Watch-only"), TransactionTableModel::Watchonly);
    writer.addColumn(tr("Date"), 0, TransactionTableModel::DateRole);
    writer.addColumn(tr("Type"), ReptiloidsCoinTransactionHistoryFilterProxy::HistoryType, Qt::DisplayRole);
    writer.addColumn(tr("Label"), 0, TransactionTableModel::LabelRole);
    writer.addColumn(tr("Address"), 0, TransactionTableModel::AddressRole);
    writer.addColumn(BitcoinUnits::getAmountColumnTitle(displayUnit), 0, TransactionTableModel::FormattedAmountRole);
    writer.addColumn(tr("ID"), 0, TransactionTableModel::TxHashRole);

    if (!writer.write())
        QMessageBox::warning(this->parentWidget(), tr("Export Failed"), tr("There was an error trying to save the transaction history to %1.").arg(filename), QMessageBox::Ok);
    else
        QMessageBox::warning(this->parentWidget(), tr("Export Successful"), tr("The transaction history was successfully saved to %1.").arg(filename), QMessageBox::Ok);
}

void ReptiloidsCoinTransactionHistory::displayTotalSelected(const QItemSelection &, const QItemSelection &) {
    if (!transactionsTbl->selectionModel()) {
        totalSelectedLbl->clear();
        totalSelectedLbl->hide();
        return;
    }

    QModelIndexList selection = transactionsTbl->selectionModel()->selectedRows();

    qint64 amount = 0;
    for (QModelIndex & index : selection)
        amount += index.data(TransactionTableModel::AmountRole).toLongLong();

    QString strAmount(BitcoinUnits::formatWithUnit(displayUnit, amount, true, BitcoinUnits::separatorAlways));
    if (amount < 0)
        strAmount = "<span style='color:red;'>" + strAmount + "</span>";

    totalSelectedLbl->setText(strAmount);
    totalSelectedLbl->show();
}

ReptiloidsCoinTransactionHistoryTable::ReptiloidsCoinTransactionHistoryTable(QWidget *parent) : QTableView(parent),
                                                                                    walletModel(nullptr) {
}

void ReptiloidsCoinTransactionHistoryTable::setWalletModel(WalletModel *w) {
    if (walletModel == w)
        return;
    walletModel = w;

    if (walletModel == nullptr) {
        setModel(nullptr);
        return;
    }

    this->setItemDelegateForColumn(ReptiloidsCoinTransactionHistoryFilterProxy::HistoryStatus, new ReptiloidsCoinTransactionHistoryCellItem(this));

    // Set up transaction list
    auto *filter = new ReptiloidsCoinTransactionHistoryFilterProxy(walletModel->getOptionsModel(), this);
    filter->setSourceModel(walletModel->getTransactionTableModel());
    filter->setDynamicSortFilter(true);
    filter->setSortRole(Qt::EditRole);
    filter->setFilterRole(Qt::EditRole);
    filter->setSortCaseSensitivity(Qt::CaseInsensitive);
    filter->setFilterCaseSensitivity(Qt::CaseInsensitive);
    filter->sort(ReptiloidsCoinTransactionHistoryFilterProxy::HistoryDate, Qt::DescendingOrder);
    setModel(filter);
}

void ReptiloidsCoinTransactionHistoryTable::leave() {
    this->blockSignals(true);
    model()->blockSignals(true);
}
void ReptiloidsCoinTransactionHistoryTable::enter() {
    this->blockSignals(false);
    model()->blockSignals(false);
}

void ReptiloidsCoinTransactionHistoryTable::setAddressPrefix(const QString &prefix) {
    auto *m = dynamic_cast<ReptiloidsCoinTransactionHistoryFilterProxy*>(this->model());
    m->setAddressPrefix(prefix);
}

void ReptiloidsCoinTransactionHistoryTable::setMinAmount(const CAmount &minimum) {
    auto *m = dynamic_cast<ReptiloidsCoinTransactionHistoryFilterProxy*>(this->model());
    m->setMinAmount(minimum);
}

void ReptiloidsCoinTransactionHistoryTable::setTypeFilter(quint32 types) {
    auto *m = dynamic_cast<ReptiloidsCoinTransactionHistoryFilterProxy*>(this->model());
    m->setTypeFilter(types);
}

void ReptiloidsCoinTransactionHistoryTable::setDateRange(const QDateTime &from, const QDateTime &to) {
    auto *m = dynamic_cast<ReptiloidsCoinTransactionHistoryFilterProxy*>(this->model());
    m->setDateRange(from, to);
}

const QDateTime ReptiloidsCoinTransactionHistoryFilterProxy::MIN_DATE = QDateTime::fromTime_t(0);
const QDateTime ReptiloidsCoinTransactionHistoryFilterProxy::MAX_DATE = QDateTime::fromTime_t(0xFFFFFFFF);
ReptiloidsCoinTransactionHistoryFilterProxy::ReptiloidsCoinTransactionHistoryFilterProxy(OptionsModel *o, QObject *parent) : QSortFilterProxyModel(parent),
                                                                                                                 optionsModel(o),
                                                                                                                 limitRows(-1),
                                                                                                                 addrPrefix(QString()),
                                                                                                                 minAmount(0),
                                                                                                                 typeFilter(COMMON_TYPES),
                                                                                                                 dateFrom(MIN_DATE),
                                                                                                                 dateTo(MAX_DATE) { }

void ReptiloidsCoinTransactionHistoryFilterProxy::setLimit(int limit) {
    this->limitRows = limit;
}

void ReptiloidsCoinTransactionHistoryFilterProxy::setAddressPrefix(const QString &prefix) {
    this->addrPrefix = prefix;
    invalidateFilter();
}

void ReptiloidsCoinTransactionHistoryFilterProxy::setMinAmount(const CAmount &minimum) {
    this->minAmount = minimum;
    invalidateFilter();
}

void ReptiloidsCoinTransactionHistoryFilterProxy::setTypeFilter(quint32 types) {
    this->typeFilter = types;
    invalidateFilter();
}

void ReptiloidsCoinTransactionHistoryFilterProxy::setDateRange(const QDateTime &from, const QDateTime &to) {
    this->dateFrom = from;
    this->dateTo = to;
    invalidateFilter();
}

bool ReptiloidsCoinTransactionHistoryFilterProxy::filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const {
    QModelIndex index = sourceModel()->index(sourceRow, 0, sourceParent);

    auto type = index.data(TransactionTableModel::TypeRole).toInt();
    if (!(TYPE(type) & typeFilter))
        return false;

    QDateTime datetime = index.data(TransactionTableModel::DateRole).toDateTime();
    QString address = index.data(TransactionTableModel::AddressRole).toString();
    QString label = index.data(TransactionTableModel::LabelRole).toString();
    qint64 amount = llabs(index.data(TransactionTableModel::AmountRole).toLongLong());
    int status = index.data(TransactionTableModel::StatusRole).toInt();

    if (status == TransactionStatus::Conflicted)
        return false;
    if (datetime < dateFrom || datetime > dateTo)
        return false;
    if (!address.contains(addrPrefix, Qt::CaseInsensitive) && !label.contains(addrPrefix, Qt::CaseInsensitive))
        return false;
    if (amount < minAmount)
        return false;

    return true;
}

bool ReptiloidsCoinTransactionHistoryFilterProxy::lessThan(const QModelIndex &left, const QModelIndex &right) const {
    switch (left.column()) {
        case HistoryDate: {
            auto l1 = sourceModel()->index(left.row(), TransactionTableModel::Date).data(Qt::EditRole);
            auto r1 = sourceModel()->index(right.row(), TransactionTableModel::Date).data(Qt::EditRole);
            return l1.toLongLong() < r1.toLongLong();
        }
        case HistoryToAddress: {
            auto l2 = sourceModel()->index(left.row(), TransactionTableModel::ToAddress).data(Qt::DisplayRole);
            auto r2 = sourceModel()->index(right.row(), TransactionTableModel::ToAddress).data(Qt::DisplayRole);
            return l2.toString() < r2.toString();
        }
        case HistoryType: {
            auto l3 = sourceModel()->index(left.row(), TransactionTableModel::Type).data(Qt::DisplayRole);
            auto r3 = sourceModel()->index(right.row(), TransactionTableModel::Type).data(Qt::DisplayRole);
            return l3.toString() < r3.toString();
        }
        case HistoryAmount: {
            auto l4 = sourceModel()->index(left.row(), TransactionTableModel::Amount).data(Qt::EditRole);
            auto r4 = sourceModel()->index(right.row(), TransactionTableModel::Amount).data(Qt::EditRole);
            return l4.toLongLong() < r4.toLongLong();
        }
    }
    return QSortFilterProxyModel::lessThan(left, right);
}

int ReptiloidsCoinTransactionHistoryFilterProxy::columnCount(const QModelIndex &) const {
    return ReptiloidsCoinTransactionHistoryFilterProxy::HistoryAmount + 1;
}

int ReptiloidsCoinTransactionHistoryFilterProxy::rowCount(const QModelIndex& parent) const {
    if (limitRows != -1)
        return std::min(QSortFilterProxyModel::rowCount(parent), limitRows);
    else
        return QSortFilterProxyModel::rowCount(parent);
}

QVariant ReptiloidsCoinTransactionHistoryFilterProxy::headerData(int section, Qt::Orientation orientation, int role) const {
    if (orientation == Qt::Horizontal) {
        if (role == Qt::DisplayRole) {
            switch (section) {
                case HistoryStatus:
                    return tr("");
                case HistoryDate:
                    return tr("Date");
                case HistoryTime:
                    return tr("");
                case HistoryToAddress:
                    return tr("Address");
                case HistoryType:
                    return tr("Type");
                case HistoryAmount:
                    return tr("Amount");
                default:
                    return tr("");
            }
        } else if (role == Qt::TextAlignmentRole) {
            return Qt::AlignLeft;
        }
    }
    return QSortFilterProxyModel::headerData(section, orientation, role);
}

QVariant ReptiloidsCoinTransactionHistoryFilterProxy::data(const QModelIndex &index, int role) const {
    if (!index.isValid())
        return QVariant();

    auto *model = dynamic_cast<TransactionTableModel*>(sourceModel());
    QModelIndex sourceIndex = mapToSource(index);
    auto *rec = static_cast<TransactionRecord*>(model->index(sourceIndex.row(), 0).internalPointer());
    if (!rec)
        return QVariant();

    switch (role) {
        case Qt::DisplayRole:
            switch (index.column()) {
                case HistoryStatus:
                    return static_cast<int>(rec->status.status);
                case HistoryDate:
                    return model->formatTxDate(rec);
                case HistoryTime:
                    return QDateTime::fromTime_t(static_cast<uint>(rec->time)).toString("h:mmap");
                case HistoryToAddress: {
                    auto addr = model->formatTxToAddress(rec, false);
                    if (addr.isEmpty())
                        addr = tr("n/a");
                    return addr;
                }
                case HistoryType:
                    return model->formatTxType(rec);
                case HistoryAmount: {
                    auto amt = static_cast<CAmount>(rec->credit + rec->debit);
                    auto str = BitcoinUnits::floorWithUnit(optionsModel->getDisplayUnit(), amt, 3, true, BitcoinUnits::separatorNever);
                    if (!rec->status.countsForBalance)
                        str = QString("[%1]").arg(str);
                    return str;
                }
            }
            break;
        case Qt::EditRole: // Edit role is used for sorting, so return the unformatted values
        case Qt::UserRole:
            switch (index.column()) {
                case HistoryStatus:
                    return QString::fromStdString(rec->status.sortKey);
                case HistoryDate: {
                    return rec->time;
                }
                case HistoryTime:
                    return rec->time;
                case HistoryType:
                    return model->formatTxType(rec);
                case HistoryToAddress:
                    return model->formatTxToAddress(rec, true);
                case HistoryAmount:
                    return static_cast<qint64>(rec->credit + rec->debit);
            }
            break;
        case Qt::DecorationRole:
            switch (index.column()) {
                case HistoryStatus:
                    return model->txStatusDecoration(rec);
                default:
                    return QVariant();
            }
            break;
        case Qt::ToolTipRole:
            return model->formatTooltip(rec);
        case Qt::TextAlignmentRole:
            switch (index.column()) {
                case HistoryStatus:
                    return static_cast<int>(Qt::AlignLeft | Qt::AlignVCenter);
                case HistoryDate:
                case HistoryTime:
                    return static_cast<int>(Qt::AlignCenter | Qt::AlignVCenter);
                case HistoryType:
                case HistoryToAddress:
                case HistoryAmount:
                default:
                    return static_cast<int>(Qt::AlignLeft | Qt::AlignVCenter);
            }
            break;
        case Qt::ForegroundRole:
            if (index.column() == HistoryAmount) {
                if ((rec->credit + rec->debit) < 0)
                    return QColor(0xFB, 0x7F, 0x70);
                else if ((rec->credit + rec->debit) > 0)
                    return QColor(0x4B, 0xF5, 0xC6);
            }
            return QColor("white");
    }
    return QSortFilterProxyModel::data(index, role);
}

ReptiloidsCoinTransactionHistoryCellItem::ReptiloidsCoinTransactionHistoryCellItem(QObject *parent) : QStyledItemDelegate(parent) { }

void ReptiloidsCoinTransactionHistoryCellItem::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const {
    if (index.column() == ReptiloidsCoinTransactionHistoryFilterProxy::HistoryStatus) {
        painter->save();
        QColor color;
        auto status = static_cast<TransactionStatus::Status>(index.data(Qt::DisplayRole).toInt());
        switch (status) {
            case TransactionStatus::Status::Confirmed:
            case TransactionStatus::Status::Confirming:
                color.setRgb(0x4B, 0xF5, 0xC6);
                break;
            case TransactionStatus::Status::OpenUntilDate:
            case TransactionStatus::Status::OpenUntilBlock:
            case TransactionStatus::Status::Unconfirmed:
            case TransactionStatus::Status::Immature:
                color.setRgb(0xF8, 0xBF, 0x1C);
                break;
            case TransactionStatus::Status::Conflicted:
            case TransactionStatus::Status::NotAccepted:
            default:
                color.setRgb(0xFB, 0x7F, 0x70);
                break;
        }
        // Draw the status indicator, leave some room on top and bottom
        int pad = 2;
        QRect r(option.rect.x(), option.rect.y()+pad/2, 3, option.rect.height()-pad);
        painter->fillRect(r, color);
        painter->restore();
    }
}

QSize ReptiloidsCoinTransactionHistoryCellItem::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const {
    if (index.column() == ReptiloidsCoinTransactionHistoryFilterProxy::HistoryStatus)
        return {BGU::spi(3), option.rect.height()};
    return QStyledItemDelegate::sizeHint(option, index);
}