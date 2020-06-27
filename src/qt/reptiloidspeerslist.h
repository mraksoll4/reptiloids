// Copyright (c) 2018-2019 The Blocknet developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef REPTILOIDS_QT_REPTILOIDSPEERSLIST_H
#define REPTILOIDS_QT_REPTILOIDSPEERSLIST_H

#include <qt/reptiloidspeerdetails.h>
#include <qt/reptiloidstoolspage.h>

#include <qt/clientmodel.h>

#include <QFrame>
#include <QHideEvent>
#include <QLabel>
#include <QShowEvent>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QVBoxLayout>

class ReptiloidsPeersList : public ReptiloidsToolsPage {
    Q_OBJECT
protected:

public:
    explicit ReptiloidsPeersList(QWidget *, int id, QFrame *parent = nullptr);
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
    ReptiloidsPeerDetails *peerDetails = nullptr;

    enum {
        COLUMN_ADDRESS,
        COLUMN_VERSION,
        COLUMN_PING_TIME,
        COLUMN_BAN_SCORE,
        COLUMN_LATEST_BLOCK
    };
};

#endif // REPTILOIDS_QT_REPTILOIDSPEERSLIST_H
