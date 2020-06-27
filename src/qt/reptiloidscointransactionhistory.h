// Copyright (c) 2018-2019 The Blocknet developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef REPTILOIDSCOIN_QT_REPTILOIDSCOINTRANSACTIONHISTORY_H
#define REPTILOIDSCOIN_QT_REPTILOIDSCOINTRANSACTIONHISTORY_H

#include <qt/reptiloidscoindropdown.h>
#include <qt/reptiloidscoinlineedit.h>

#include <qt/optionsmodel.h>
#include <qt/transactionrecord.h>
#include <qt/transactiontablemodel.h>
#include <qt/walletmodel.h>

#include <QDateTime>
#include <QDateTimeEdit>
#include <QKeyEvent>
#include <QFrame>
#include <QLabel>
#include <QMenu>
#include <QModelIndex>
#include <QPainter>
#include <QPushButton>
#include <QScrollArea>
#include <QSortFilterProxyModel>
#include <QTableView>
#include <QVariant>
#include <QVBoxLayout>
#include <QVector>

class ReptiloidsCoinTransactionHistoryTable;

class ReptiloidsCoinTransactionHistory : public QFrame {
    Q_OBJECT

public:
    explicit ReptiloidsCoinTransactionHistory(WalletModel *w, QWidget *parent = nullptr);

    // Date ranges for filter
    enum {
        All,
        Today,
        ThisWeek,
        ThisMonth,
        LastMonth,
        ThisYear,
        Range
    };

Q_SIGNALS:

public Q_SLOTS:

protected:
    void showEvent(QShowEvent *event) override;
    void hideEvent(QHideEvent *event) override;

private Q_SLOTS:
    void onDisplayUnit(int);
    void addressChanged(const QString &prefix);
    void amountChanged(const QString &amount);
    void dateChanged(int idx);
    void typeChanged(int idx);
    void dateRangeChanged();
    void contextualMenu(const QPoint& point);
    void copyAddress();
    void copyLabel();
    void copyAmount();
    void copyTxID();
    void showDetails();
    void exportClicked();
    void displayTotalSelected(const QItemSelection &, const QItemSelection &);

private:
    WalletModel *walletModel;
    int displayUnit;
    QVBoxLayout *layout;
    QLabel *titleLbl;
    ReptiloidsCoinTransactionHistoryTable *transactionsTbl;
    ReptiloidsCoinDropdown *dateCb;
    ReptiloidsCoinDropdown *typeCb;
    ReptiloidsCoinLineEdit *addressTi;
    ReptiloidsCoinLineEdit *amountTi;
    QMenu *contextMenu;
    QFrame *dateRangeWidget;
    QDateTimeEdit *dateFrom;
    QDateTimeEdit *dateTo;
    QLabel *totalSelectedLbl;
};

class ReptiloidsCoinTransactionHistoryTable : public QTableView {
    Q_OBJECT

public:
    explicit ReptiloidsCoinTransactionHistoryTable(QWidget *parent = nullptr);
    void setWalletModel(WalletModel *w);
    void leave();
    void enter();
    void setAddressPrefix(const QString &prefix);
    void setMinAmount(const CAmount &minimum);
    void setTypeFilter(quint32 types);
    void setDateRange(const QDateTime &from, const QDateTime &to);

private:
    WalletModel *walletModel;
};

class ReptiloidsCoinTransactionHistoryFilterProxy : public QSortFilterProxyModel {
    Q_OBJECT

public:
    explicit ReptiloidsCoinTransactionHistoryFilterProxy(OptionsModel *o, QObject *parent = nullptr);

    QVariant data(const QModelIndex &index, int role) const override;
    int rowCount(const QModelIndex &parent) const override;
    int columnCount(const QModelIndex &) const override;

    static const QDateTime MIN_DATE; // Earliest date that can be represented (far in the past)
    static const QDateTime MAX_DATE; // Last date that can be represented (far in the future)
    static const quint32 ALL_TYPES = 0xFFFFFFFF; // Type filter bit field (all types)
    static const quint32 COMMON_TYPES = 4479; // Type filter bit field (all types but Obfuscation-SPAM)
    static quint32 TYPE(int type) { return 1 << type; }

    enum ColumnIndex {
        HistoryStatus = 0,
        HistoryDate = 1,
        HistoryTime = 2,
        HistoryToAddress = 3,
        HistoryType = 4,
        HistoryAmount = 5
    };

    void setLimit(int limit); // Set maximum number of rows returned, -1 if unlimited.
    void setAddressPrefix(const QString &prefix);
    void setMinAmount(const CAmount &minimum);
    void setTypeFilter(quint32 types);
    void setDateRange(const QDateTime &from, const QDateTime &to);

protected:
    bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const override;
    bool lessThan(const QModelIndex &left, const QModelIndex &right) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;

private:
    OptionsModel *optionsModel;
    int limitRows;
    QString addrPrefix;
    CAmount minAmount;
    quint32 typeFilter;
    QDateTime dateFrom;
    QDateTime dateTo;
};

#include <QStyledItemDelegate>

/**
 * @brief Responsible for drawing the custom table columns (Status, Date, Amount).
 * @param o
 * @param parent
 */
class ReptiloidsCoinTransactionHistoryCellItem : public QStyledItemDelegate {
    Q_OBJECT

public:
    explicit ReptiloidsCoinTransactionHistoryCellItem(QObject *parent = nullptr);

protected:
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const override;
};

#endif // REPTILOIDSCOIN_QT_REPTILOIDSCOINTRANSACTIONHISTORY_H