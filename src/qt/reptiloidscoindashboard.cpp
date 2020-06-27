// Copyright (c) 2018-2020 The Blocknet developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <qt/reptiloidscoindashboard.h>

#include <qt/reptiloidscoinhdiv.h>
#include <qt/reptiloidscoiniconbtn.h>
#include <qt/reptiloidscoinguiutil.h>
#include <qt/reptiloidscoinvars.h>

#include <qt/bitcoinunits.h>
#include <qt/transactionrecord.h>
#include <qt/transactiontablemodel.h>

#include <QAbstractItemView>
#include <QDateTime>
#include <QFont>
#include <QHeaderView>

ReptiloidsCoinDashboard::ReptiloidsCoinDashboard(QFrame *parent) : QFrame(parent), layout(new QVBoxLayout),
                                                       walletModel(nullptr),
                                                       displayUnit(0), walletBalance(0),
                                                       unconfirmedBalance(0), immatureBalance(0) {
//    this->setStyleSheet("border: 1px solid red");
    this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    layout->setContentsMargins(BGU::spi(46), BGU::spi(10), BGU::spi(50), 0);
    this->setLayout(layout);

    titleLbl = new QLabel(tr("Dashboard"));
    titleLbl->setObjectName("h4");

    auto *balanceGrid = new QFrame;
    auto *balanceGridLayout = new QGridLayout;
    balanceGridLayout->setContentsMargins(QMargins());
    balanceGrid->setLayout(balanceGridLayout);

    auto *balanceBox = new QFrame;
    auto *balanceBoxLayout = new QVBoxLayout;
    balanceBoxLayout->setContentsMargins(QMargins());
    balanceBox->setLayout(balanceBoxLayout);
    balanceLbl = new QLabel(tr("Available Balance"));
    balanceLbl->setObjectName("balanceLbl");
    balanceValueLbl = new QLabel;
    balanceValueLbl->setObjectName("h1");

    auto *pendingBox = new QFrame;
    auto *pendingLayout = new QHBoxLayout;
    pendingLayout->setContentsMargins(QMargins());
    pendingBox->setLayout(pendingLayout);
    pendingLbl = new QLabel(tr("Pending:"));
    pendingLbl->setObjectName("pendingLbl");
    pendingValueLbl = new QLabel;
    pendingValueLbl->setObjectName("pendingValueLbl");
    pendingLayout->addWidget(pendingLbl);
    pendingLayout->addWidget(pendingValueLbl, 0, Qt::AlignLeft);
    pendingLayout->addStretch(1);

    auto *immatureBox = new QFrame;
    auto *immatureLayout = new QHBoxLayout;
    immatureLayout->setContentsMargins(QMargins());
    immatureBox->setLayout(immatureLayout);
    immatureLbl = new QLabel(tr("Immature:"));
    immatureLbl->setObjectName("immatureLbl");
    immatureValueLbl = new QLabel;
    immatureValueLbl->setObjectName("immatureValueLbl");
    immatureLayout->addWidget(immatureLbl);
    immatureLayout->addWidget(immatureValueLbl, 0, Qt::AlignLeft);
    immatureLayout->addStretch(1);

    auto *totalBox = new QFrame;
    auto *totalLayout = new QHBoxLayout;
    totalLayout->setContentsMargins(QMargins());
    totalBox->setLayout(totalLayout);
    totalLbl = new QLabel(tr("Total:"));
    totalLbl->setObjectName("totalLbl");
    totalValueLbl = new QLabel;
    totalValueLbl->setObjectName("totalValueLbl");
    totalLayout->addWidget(totalLbl);
    totalLayout->addWidget(totalValueLbl, 0, Qt::AlignLeft);
    totalLayout->addStretch(1);

    balanceBoxLayout->addWidget(balanceValueLbl);
    balanceBoxLayout->addWidget(pendingBox);
    balanceBoxLayout->addWidget(immatureBox);
    balanceBoxLayout->addWidget(totalBox);

    auto *quickSend = new ReptiloidsCoinIconBtn(tr("Quick Send"), ":/redesign/QuickActions/QuickSendIcon.png");

    balanceGridLayout->addWidget(balanceBox, 0, 0, Qt::AlignLeft);
    balanceGridLayout->addWidget(quickSend, 0, 1, Qt::AlignRight | Qt::AlignVCenter);
    balanceGridLayout->setColumnStretch(1, 1);

    auto *hdiv = new ReptiloidsCoinHDiv;

    auto *recentBox = new QFrame;
    auto *recentLayout = new QHBoxLayout;
    recentLayout->setContentsMargins(QMargins());
    recentLayout->setSpacing(0);
    recentBox->setLayout(recentLayout);
    recentTxsLbl = new QLabel(tr("Recent Transactions"));
    recentTxsLbl->setObjectName("recentTransactionsLbl");
    viewAll = new QPushButton;
    viewAll->setObjectName("linkBtn");
    viewAll->setText(tr("View All"));
    viewAll->setCursor(Qt::PointingHandCursor);
    recentLayout->addWidget(recentTxsLbl, 0, Qt::AlignBottom);
    recentLayout->addWidget(viewAll, 0, Qt::AlignRight | Qt::AlignBottom);

    recentTransactions = new QFrame;
    recentTransactions->setObjectName("content");
    recentTransactions->setContentsMargins(QMargins());
    recentTransactions->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    auto *recentTransactionsGridLayout = new QVBoxLayout;
    recentTransactionsGridLayout->setContentsMargins(QMargins());
    recentTransactions->setLayout(recentTransactionsGridLayout);
    transactionsTbl = new ReptiloidsCoinDashboardTable;
    transactionsTbl->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    transactionsTbl->setEditTriggers(QAbstractItemView::NoEditTriggers);
    transactionsTbl->setSelectionBehavior(QAbstractItemView::SelectRows);
    transactionsTbl->setSelectionMode(QAbstractItemView::NoSelection);
    transactionsTbl->setAlternatingRowColors(true);
    transactionsTbl->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
    transactionsTbl->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    transactionsTbl->setShowGrid(false);
    transactionsTbl->setFocusPolicy(Qt::NoFocus);
    transactionsTbl->setContextMenuPolicy(Qt::CustomContextMenu);
    transactionsTbl->setSortingEnabled(true);
    transactionsTbl->verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    transactionsTbl->verticalHeader()->setDefaultSectionSize(BGU::spi(60));
    transactionsTbl->verticalHeader()->setVisible(false);
    transactionsTbl->horizontalHeader()->setDefaultAlignment(Qt::AlignLeft);
    transactionsTbl->horizontalHeader()->setSortIndicatorShown(false);
    transactionsTbl->horizontalHeader()->setSectionsClickable(false);
    transactionsTbl->horizontalHeader()->setVisible(false);
    recentTransactionsGridLayout->addWidget(transactionsTbl, 1);

    layout->addWidget(titleLbl, 0, Qt::AlignTop | Qt::AlignLeft);
    layout->addWidget(balanceLbl);
    layout->addWidget(balanceGrid);
    layout->addSpacing(BGU::spi(15));
    layout->addWidget(hdiv);
    layout->addSpacing(BGU::spi(15));
    layout->addWidget(recentBox);
    layout->addWidget(recentTransactions, 1);
    layout->addSpacing(BGU::spi(20));

    connect(quickSend, &ReptiloidsCoinIconBtn::clicked, this, &ReptiloidsCoinDashboard::onQuickSend);
    connect(viewAll, &QPushButton::clicked, this, &ReptiloidsCoinDashboard::onViewAll);
}

void ReptiloidsCoinDashboard::setWalletModel(WalletModel *w) {
    if (walletModel == w) {
        displayUnit = walletModel->getOptionsModel()->getDisplayUnit();
        balanceChanged(walletModel->wallet().getBalances());
        return;
    }

    walletEvents(false);

    walletModel = w;
    if (!walletModel || !walletModel->getOptionsModel())
        return;

    displayUnit = walletModel->getOptionsModel()->getDisplayUnit();
    balanceChanged(walletModel->wallet().getBalances());

    transactionsTbl->setWalletModel(walletModel);
    transactionsTbl->horizontalHeader()->setSectionResizeMode(ReptiloidsCoinDashboardFilterProxy::DashboardStatus, QHeaderView::Fixed);
    transactionsTbl->horizontalHeader()->setSectionResizeMode(ReptiloidsCoinDashboardFilterProxy::DashboardDate, QHeaderView::Fixed);
    transactionsTbl->horizontalHeader()->setSectionResizeMode(ReptiloidsCoinDashboardFilterProxy::DashboardTime, QHeaderView::Fixed);
    transactionsTbl->horizontalHeader()->setSectionResizeMode(ReptiloidsCoinDashboardFilterProxy::DashboardType, QHeaderView::ResizeToContents);
    transactionsTbl->horizontalHeader()->setSectionResizeMode(ReptiloidsCoinDashboardFilterProxy::DashboardAmount, QHeaderView::ResizeToContents);
    transactionsTbl->horizontalHeader()->setSectionResizeMode(ReptiloidsCoinDashboardFilterProxy::DashboardToAddress, QHeaderView::Stretch);
    transactionsTbl->setColumnWidth(ReptiloidsCoinDashboardFilterProxy::DashboardStatus, BGU::spi(3));
    transactionsTbl->setColumnWidth(ReptiloidsCoinDashboardFilterProxy::DashboardDate, BGU::spi(60));
    transactionsTbl->setColumnWidth(ReptiloidsCoinDashboardFilterProxy::DashboardTime, BGU::spi(72));

    // Watch for wallet changes
    walletEvents(true);
}

void ReptiloidsCoinDashboard::showEvent(QShowEvent *event) {
    QWidget::showEvent(event);
    walletEvents(true);
    transactionsTbl->enter();
}

void ReptiloidsCoinDashboard::hideEvent(QHideEvent *event) {
    QWidget::hideEvent(event);
    walletEvents(false);
    transactionsTbl->leave();
}

void ReptiloidsCoinDashboard::balanceChanged(const interfaces::WalletBalances & balances) {
    walletBalance = balances.balance;
    unconfirmedBalance = balances.unconfirmed_balance;
    immatureBalance = balances.immature_balance;
    updateBalance();
}

void ReptiloidsCoinDashboard::displayUnitChanged(int unit) {
    displayUnit = unit;
    updateBalance();
}

void ReptiloidsCoinDashboard::updateBalance() {
    balanceValueLbl->setText(BitcoinUnits::formatWithUnit(displayUnit, walletBalance, false, BitcoinUnits::separatorAlways));
    pendingValueLbl->setText(BitcoinUnits::formatWithUnit(displayUnit, unconfirmedBalance, false, BitcoinUnits::separatorAlways));
    immatureValueLbl->setText(BitcoinUnits::formatWithUnit(displayUnit, immatureBalance, false, BitcoinUnits::separatorAlways));
    totalValueLbl->setText(BitcoinUnits::formatWithUnit(displayUnit, walletBalance + unconfirmedBalance + immatureBalance, false, BitcoinUnits::separatorAlways));
}

void ReptiloidsCoinDashboard::walletEvents(const bool on) {
    if (walletModel && on) {
        connect(walletModel->getOptionsModel(), &OptionsModel::displayUnitChanged, this, &ReptiloidsCoinDashboard::displayUnitChanged);
        displayUnitChanged(walletModel->getOptionsModel()->getDisplayUnit());
    } else if (walletModel) {
        disconnect(walletModel->getOptionsModel(), &OptionsModel::displayUnitChanged, this, &ReptiloidsCoinDashboard::displayUnitChanged);
    }
}

ReptiloidsCoinDashboardTable::ReptiloidsCoinDashboardTable(QWidget *parent) : QTableView(parent),
                                                                  walletModel(nullptr) {
}

void ReptiloidsCoinDashboardTable::setWalletModel(WalletModel *w) {
    if (walletModel == w)
        return;
    walletModel = w;

    if (walletModel == nullptr) {
        setModel(nullptr);
        return;
    }

    this->setItemDelegateForColumn(ReptiloidsCoinDashboardFilterProxy::DashboardStatus, new ReptiloidsCoinDashboardCellItem(this));
    this->setItemDelegateForColumn(ReptiloidsCoinDashboardFilterProxy::DashboardDate, new ReptiloidsCoinDashboardCellItem(this));

    // Set up transaction list
    auto *filter = new ReptiloidsCoinDashboardFilterProxy(walletModel->getOptionsModel(), this);
    filter->setSourceModel(walletModel->getTransactionTableModel());
    filter->setLimit(30);
    filter->setDynamicSortFilter(true);
    filter->setSortRole(Qt::EditRole);
    filter->setFilterRole(Qt::EditRole);
    filter->sort(ReptiloidsCoinDashboardFilterProxy::DashboardDate, Qt::DescendingOrder);
    setModel(filter);
}

void ReptiloidsCoinDashboardTable::leave() {
    this->blockSignals(true);
    model()->blockSignals(true);
}
void ReptiloidsCoinDashboardTable::enter() {
    this->blockSignals(false);
    model()->blockSignals(false);
    setModel(model());
}

ReptiloidsCoinDashboardFilterProxy::ReptiloidsCoinDashboardFilterProxy(OptionsModel *o, QObject *parent) : QSortFilterProxyModel(parent),
                                                                                               optionsModel(o),
                                                                                               limitRows(-1) { }


void ReptiloidsCoinDashboardFilterProxy::setLimit(int limit) {
    this->limitRows = limit;
}

bool ReptiloidsCoinDashboardFilterProxy::filterAcceptsRow(int sourceRow, const QModelIndex& sourceParent) const {
    QModelIndex index = sourceModel()->index(sourceRow, 0, sourceParent);

    auto involvesWatchAddress = index.data(TransactionTableModel::WatchonlyRole).toBool();
    if (involvesWatchAddress)
        return false;

    auto status = index.data(TransactionTableModel::StatusRole).toInt();
    if (status == TransactionStatus::Conflicted)
        return false;

    return true;
}

bool ReptiloidsCoinDashboardFilterProxy::lessThan(const QModelIndex &left, const QModelIndex &right) const {
    if (left.column() == ReptiloidsCoinDashboardFilterProxy::DashboardDate) {
        QVariant leftData = sourceModel()->index(left.row(), TransactionTableModel::Date).data(Qt::EditRole);
        QVariant rightData = sourceModel()->index(right.row(), TransactionTableModel::Date).data(Qt::EditRole);
        return leftData.toLongLong() < rightData.toLongLong();
    }
    return QSortFilterProxyModel::lessThan(left, right);
}

int ReptiloidsCoinDashboardFilterProxy::columnCount(const QModelIndex &) const {
    return ReptiloidsCoinDashboardFilterProxy::DashboardAmount + 1;
}

int ReptiloidsCoinDashboardFilterProxy::rowCount(const QModelIndex& parent) const {
    if (limitRows != -1)
        return std::min(QSortFilterProxyModel::rowCount(parent), limitRows);
    else
        return QSortFilterProxyModel::rowCount(parent);
}

QVariant ReptiloidsCoinDashboardFilterProxy::data(const QModelIndex &index, int role) const {
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
            case DashboardStatus:
                return static_cast<int>(rec->status.status);
            case DashboardDate:
                return model->formatTxDate(rec);
            case DashboardTime:
                return QDateTime::fromTime_t(static_cast<uint>(rec->time)).toString("h:mmap");
            case DashboardToAddress: {
                auto addr = model->formatTxToAddress(rec, false);
                if (addr.isEmpty())
                    addr = tr("n/a");
                return addr;
            }
            case DashboardType:
                return model->formatTxType(rec);
            case DashboardAmount: {
                auto amt = static_cast<CAmount>(rec->credit + rec->debit);
                auto str = BitcoinUnits::floorWithUnit(optionsModel->getDisplayUnit(), amt, 2, true, BitcoinUnits::separatorNever);
                if (!rec->status.countsForBalance)
                    str = QString("[%1]").arg(str);
                return str;
            }
        }
        break;
    case Qt::EditRole: // Edit role is used for sorting, so return the unformatted values
    case Qt::UserRole:
        switch (index.column()) {
            case DashboardStatus:
                return QString::fromStdString(rec->status.sortKey);
            case DashboardDate: {
                return rec->time;
            }
            case DashboardTime:
                return rec->time;
            case DashboardType:
                return model->formatTxType(rec);
            case DashboardToAddress:
                return model->formatTxToAddress(rec, true);
            case DashboardAmount:
                return static_cast<qint64>(rec->credit + rec->debit);
            }
        break;
    case Qt::DecorationRole:
        switch (index.column()) {
            case DashboardStatus:
                return model->txStatusDecoration(rec);
            default:
                return QVariant();
        }
        break;
    case Qt::ToolTipRole:
        return model->formatTooltip(rec);
    case Qt::TextAlignmentRole:
        switch (index.column()) {
            case DashboardStatus:
                return static_cast<int>(Qt::AlignLeft | Qt::AlignVCenter);
            case DashboardDate:
            case DashboardTime:
                return static_cast<int>(Qt::AlignCenter | Qt::AlignVCenter);
            case DashboardType:
            case DashboardToAddress:
            case DashboardAmount:
            default:
                return static_cast<int>(Qt::AlignLeft | Qt::AlignVCenter);
        }
        break;
    case Qt::ForegroundRole:
        if (index.column() == DashboardAmount) {
            if ((rec->credit + rec->debit) < 0)
                return QColor(0xFB, 0x7F, 0x70);
            else if ((rec->credit + rec->debit) > 0)
                return QColor(0x4B, 0xF5, 0xC6);
        }
        return QColor("white");
    }
    return QSortFilterProxyModel::data(index, role);
}

ReptiloidsCoinDashboardCellItem::ReptiloidsCoinDashboardCellItem(QObject *parent) : QStyledItemDelegate(parent) { }

void ReptiloidsCoinDashboardCellItem::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const {
    painter->save();

    switch (index.column()) {
        case ReptiloidsCoinDashboardFilterProxy::DashboardStatus: {
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
            int pad = BGU::spi(2);
            QRect r(option.rect.x(), option.rect.y()+pad/2, BGU::spi(3), option.rect.height()-pad);
            painter->fillRect(r, color);
            break;
        }
        case ReptiloidsCoinDashboardFilterProxy::DashboardDate: {
            auto date = QDateTime::fromTime_t(index.data(Qt::EditRole).toULongLong());
            auto month = date.toString("MMM").toUpper();
            auto dt = date.toString("dd");
            painter->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
            // Draw the month
            painter->save();
            painter->setPen(QColor("white"));
            painter->setFont(QFont("Roboto", 13, 25));
            painter->drawText(QRect(option.rect.x(), option.rect.y() + BGU::spi(5), option.rect.width(), option.rect.height()*0.4), month, Qt::AlignCenter | Qt::AlignVCenter);
            painter->restore();
            // Draw the date
            painter->save();
            painter->setFont(QFont("Roboto", 21, 25));
            painter->setPen(QColor("white"));
            painter->drawText(QRect(option.rect.x(), option.rect.y() + BGU::spi(20), option.rect.width(), option.rect.height()*0.6), dt, Qt::AlignCenter | Qt::AlignVCenter);
            painter->restore();
            break;
        }
    }

    painter->restore();
}

QSize ReptiloidsCoinDashboardCellItem::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const {
    switch (index.column()) {
        case ReptiloidsCoinDashboardFilterProxy::DashboardStatus:
            return {BGU::spi(3), option.rect.height()};
        case ReptiloidsCoinDashboardFilterProxy::DashboardDate:
            return {BGU::spi(60), option.rect.height()};
    }
    return QStyledItemDelegate::sizeHint(option, index);
}
