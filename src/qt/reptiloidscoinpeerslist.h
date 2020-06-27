// Copyright (c) 2018-2019 The Blocknet developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef REPTILOIDSCOIN_QT_REPTILOIDSCOINPEERSLIST_H
#define REPTILOIDSCOIN_QT_REPTILOIDSCOINPEERSLIST_H

#include <qt/reptiloidscoinpeerdetails.h>
#include <qt/reptiloidscointoolspage.h>

#include <qt/clientmodel.h>

#include <QFrame>
#include <QHideEvent>
#include <QLabel>
#include <QShowEvent>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QVBoxLayout>

class ReptiloidsCoinPeersList : public ReptiloidsCoinToolsPage {
    Q_OBJECT
protected:

public:
    explicit ReptiloidsCoinPeersList(QWidget *, int id, QFrame *parent = nullptr);
    void setClientModel(ClientModel *c);

protected:
    void showEvent(QShowEvent *event) override;
    void hideEvent(QHideEvent *event) override;

private Q_SLOTS:
    void displayPeerDetails(const QItemSelection &, const QItemSelection &);

private:
    QVBoxLayout *layout;
    QLabel *titleLbl;
    QTableView *table;
    ClientModel *clientModel = nullptr;
    ReptiloidsCoinPeerDetails *peerDetails = nullptr;

    enum {
        COLUMN_ADDRESS,
        COLUMN_VERSION,
        COLUMN_PING_TIME,
        COLUMN_BAN_SCORE,
        COLUMN_LATEST_BLOCK
    };
};

#endif // REPTILOIDSCOIN_QT_REPTILOIDSCOINPEERSLIST_H
