// Copyright (c) 2018-2019 The Blocknet developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef REPTILOIDS_QT_REPTILOIDSLEFTMENU_H
#define REPTILOIDS_QT_REPTILOIDSLEFTMENU_H

#include <qt/reptiloidsiconlabel.h>
#include <qt/reptiloidsvars.h>

#include <amount.h>

#include <QAbstractButton>
#include <QFrame>
#include <QLabel>
#include <QWidget>

class ReptiloidsLeftMenu : public QFrame
{
    Q_OBJECT

public:
    explicit ReptiloidsLeftMenu(QFrame *parent = nullptr);
    void setBalance(CAmount balance, int unit);
    void selectMenu(ReptiloidsPage menuType);

Q_SIGNALS:
    void menuChanged(ReptiloidsPage menuType);

public Q_SLOTS:

private Q_SLOTS:
    void onMenuClicked(bool);

private:
    QVBoxLayout *layout;
    QLabel *logo;
    QLabel *balanceLbl;
    QLabel *balanceAmountLbl;

    QButtonGroup *group;
    ReptiloidsIconLabel *dashboard;
    ReptiloidsIconLabel *addressBook;
    ReptiloidsIconLabel *sendFunds;
    ReptiloidsIconLabel *requestFunds;
    ReptiloidsIconLabel *transactionHistory;
    ReptiloidsIconLabel *snodes;
    ReptiloidsIconLabel *proposals;
    ReptiloidsIconLabel *announcements;
    ReptiloidsIconLabel *settings;
    ReptiloidsIconLabel *tools;
    QList<QAbstractButton*> btns;

    QLabel *versionLbl;
    int selected;
};

#endif // REPTILOIDS_QT_REPTILOIDSLEFTMENU_H
