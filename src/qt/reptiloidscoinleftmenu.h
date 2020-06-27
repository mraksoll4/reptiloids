// Copyright (c) 2018-2019 The Blocknet developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef REPTILOIDSCOIN_QT_REPTILOIDSCOINLEFTMENU_H
#define REPTILOIDSCOIN_QT_REPTILOIDSCOINLEFTMENU_H

#include <qt/reptiloidscoiniconlabel.h>
#include <qt/reptiloidscoinvars.h>

#include <amount.h>

#include <QAbstractButton>
#include <QFrame>
#include <QLabel>
#include <QWidget>

class ReptiloidsCoinLeftMenu : public QFrame
{
    Q_OBJECT

public:
    explicit ReptiloidsCoinLeftMenu(QFrame *parent = nullptr);
    void setBalance(CAmount balance, int unit);
    void selectMenu(ReptiloidsCoinPage menuType);

Q_SIGNALS:
    void menuChanged(ReptiloidsCoinPage menuType);

public Q_SLOTS:

private Q_SLOTS:
    void onMenuClicked(bool);

private:
    QVBoxLayout *layout;
    QLabel *logo;
    QLabel *balanceLbl;
    QLabel *balanceAmountLbl;

    QButtonGroup *group;
    ReptiloidsCoinIconLabel *dashboard;
    ReptiloidsCoinIconLabel *addressBook;
    ReptiloidsCoinIconLabel *sendFunds;
    ReptiloidsCoinIconLabel *requestFunds;
    ReptiloidsCoinIconLabel *transactionHistory;
    ReptiloidsCoinIconLabel *snodes;
    ReptiloidsCoinIconLabel *proposals;
    ReptiloidsCoinIconLabel *announcements;
    ReptiloidsCoinIconLabel *settings;
    ReptiloidsCoinIconLabel *tools;
    QList<QAbstractButton*> btns;

    QLabel *versionLbl;
    int selected;
};

#endif // REPTILOIDSCOIN_QT_REPTILOIDSCOINLEFTMENU_H
