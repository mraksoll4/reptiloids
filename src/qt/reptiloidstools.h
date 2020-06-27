// Copyright (c) 2018-2019 The Blocknet developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef REPTILOIDS_QT_REPTILOIDSTOOLS_H
#define REPTILOIDS_QT_REPTILOIDSTOOLS_H

#include <qt/reptiloidstabbar.h>
#include <qt/reptiloidstoolspage.h>

#include <qt/clientmodel.h>
#include <qt/walletmodel.h>

#include <QFocusEvent>
#include <QFrame>
#include <QSet>
#include <QVBoxLayout>

class ReptiloidsDebugConsole;
class ReptiloidsPeersList;
class ReptiloidsBIP38Tool;
class ReptiloidsWalletRepair;

class ReptiloidsTools : public QFrame {
    Q_OBJECT
public:
    explicit ReptiloidsTools(interfaces::Node & node, const PlatformStyle *platformStyle, QFrame *parent = nullptr);
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
    QVector<ReptiloidsToolsPage*> pages;

    QVBoxLayout *layout;
    QLabel *titleLbl;
    ReptiloidsDebugConsole *debugConsole;
    ReptiloidsPeersList *networkMonitor;
    ReptiloidsPeersList *peersList;
    ReptiloidsBIP38Tool *bip38Tool;
    ReptiloidsWalletRepair *walletRepair;
    ReptiloidsPeersList *multisend;
    ReptiloidsTabBar *tabBar;
    ReptiloidsToolsPage *screen = nullptr;
};

#endif // REPTILOIDS_QT_REPTILOIDSTOOLS_H
