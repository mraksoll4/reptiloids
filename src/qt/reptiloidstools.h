// Copyright (c) 2018-2019 The Blocknet developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef REPTILOIDSCOIN_QT_REPTILOIDSCOINTOOLS_H
#define REPTILOIDSCOIN_QT_REPTILOIDSCOINTOOLS_H

#include <qt/reptiloidscointabbar.h>
#include <qt/reptiloidscointoolspage.h>

#include <qt/clientmodel.h>
#include <qt/walletmodel.h>

#include <QFocusEvent>
#include <QFrame>
#include <QSet>
#include <QVBoxLayout>

class ReptiloidsCoinDebugConsole;
class ReptiloidsCoinPeersList;
class ReptiloidsCoinBIP38Tool;
class ReptiloidsCoinWalletRepair;

class ReptiloidsCoinTools : public QFrame {
    Q_OBJECT
public:
    explicit ReptiloidsCoinTools(interfaces::Node & node, const PlatformStyle *platformStyle, QFrame *parent = nullptr);
    void setModels(ClientModel *c, WalletModel *w);

Q_SIGNALS:
    /** Get restart command-line parameters and handle restart */
    void handleRestart(QStringList args);

protected:
    void focusInEvent(QFocusEvent *evt);

private Q_SLOTS:
    void tabChanged(int tab);

private:
    WalletModel *walletModel;
    ClientModel *clientModel;
    QVector<ReptiloidsCoinToolsPage*> pages;

    QVBoxLayout *layout;
    QLabel *titleLbl;
    ReptiloidsCoinDebugConsole *debugConsole;
    ReptiloidsCoinPeersList *networkMonitor;
    ReptiloidsCoinPeersList *peersList;
    ReptiloidsCoinBIP38Tool *bip38Tool;
    ReptiloidsCoinWalletRepair *walletRepair;
    ReptiloidsCoinPeersList *multisend;
    ReptiloidsCoinTabBar *tabBar;
    ReptiloidsCoinToolsPage *screen = nullptr;
};

#endif // REPTILOIDSCOIN_QT_REPTILOIDSCOINTOOLS_H
