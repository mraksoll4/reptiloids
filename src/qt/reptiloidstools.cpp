// Copyright (c) 2018-2019 The Blocknet developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <qt/reptiloidstools.h>

#include <qt/reptiloidsbip38tool.h>
#include <qt/reptiloidsdebugconsole.h>
#include <qt/reptiloidsguiutil.h>
#include <qt/reptiloidspeerslist.h>
#include <qt/reptiloidswalletrepair.h>

#include <QEvent>
#include <QList>
#include <QMessageBox>
#include <QSettings>
#include <QTimer>

enum BToolsTabs {
    DEBUG_CONSOLE = 1,
    NETWORK_MONITOR,
    PEERS_LIST,
    BIP38_TOOL,
    WALLET_REPAIR,
    MULTISEND,
};

ReptiloidsTools::ReptiloidsTools(interfaces::Node & node, const PlatformStyle *platformStyle, QFrame *parent)
                            : QFrame(parent), layout(new QVBoxLayout)
{
    this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    layout->setContentsMargins(BGU::spi(46), BGU::spi(10), BGU::spi(50), 0);
    this->setLayout(layout);

    titleLbl = new QLabel(tr("Tools"));
    titleLbl->setObjectName("h4");
    titleLbl->setFixedHeight(BGU::spi(26));

    debugConsole = new ReptiloidsDebugConsole(node, platformStyle, this, DEBUG_CONSOLE);
//    networkMonitor = new ReptiloidsPeersList(this, NETWORK_MONITOR);
    peersList = new ReptiloidsPeersList(this, PEERS_LIST);
//    bip38Tool = new ReptiloidsBIP38Tool(this, BIP38_TOOL);
    walletRepair = new ReptiloidsWalletRepair(this, WALLET_REPAIR);
//    multisend = new ReptiloidsPeersList(this, MULTISEND);
    pages = { debugConsole, /*networkMonitor,*/ peersList, /*bip38Tool,*/ walletRepair/*, multisend*/ };

    tabBar = new ReptiloidsTabBar;
    tabBar->setParent(this);
    tabBar->addTab(tr("Debug Console"), DEBUG_CONSOLE);
//    tabBar->addTab(tr("Network Monitor"), NETWORK_MONITOR); // TODO Network monitor
    tabBar->addTab(tr("Peers List"), PEERS_LIST);
//    tabBar->addTab(tr("BIP38 Tool"), BIP38_TOOL);
    tabBar->addTab(tr("Wallet Repair"), WALLET_REPAIR);
//    tabBar->addTab(tr("Multisend"), MULTISEND); // TODO Multisend
    tabBar->show();

    connect(tabBar, &ReptiloidsTabBar::tabChanged, this, &ReptiloidsTools::tabChanged);
    connect(walletRepair, &ReptiloidsWalletRepair::handleRestart, this, [this](QStringList args) {
        Q_EMIT handleRestart(args);
    });

    layout->addWidget(titleLbl);
    layout->addWidget(tabBar);

    QSettings settings;
    tabChanged(settings.value("nToolsTab", DEBUG_CONSOLE).toInt());
}

void ReptiloidsTools::setModels(ClientModel *c, WalletModel *w) {
    if (clientModel == c && walletModel == w)
        return;
    clientModel = c;
    walletModel = w;

    debugConsole->setModels(clientModel, walletModel);
//    networkMonitor->setWalletModel(walletModel);
    peersList->setClientModel(clientModel);
//    bip38Tool->setWalletModel(walletModel);
    walletRepair->setWalletModel(walletModel);
//    multisend->setWalletModel(walletModel);
}

void ReptiloidsTools::focusInEvent(QFocusEvent *evt) {
    QWidget::focusInEvent(evt);
    if (screen)
        screen->setFocus();
}

void ReptiloidsTools::tabChanged(int tab) {
    tabBar->showTab(tab);
    QSettings settings;
    settings.setValue("nToolsTab", tab);

    if (screen) {
        layout->removeWidget(screen);
        screen->hide();
    }

    switch(tab) {
        case DEBUG_CONSOLE:
            screen = debugConsole;
            break;
//        case NETWORK_MONITOR:
//            screen = networkMonitor;
//            break;
        case PEERS_LIST:
            screen = peersList;
            break;
//        case BIP38_TOOL:
//            screen = bip38Tool;
//            break;
        case WALLET_REPAIR:
            screen = walletRepair;
            break;
//        case MULTISEND:
//            screen = multisend;
//            break;
        default:
            return;
    }

    layout->addWidget(screen);

    screen->show();
    screen->setFocus();
}

