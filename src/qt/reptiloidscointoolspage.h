// Copyright (c) 2018-2019 The Blocknet developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef REPTILOIDSCOIN_QT_REPTILOIDSCOINTOOLSPAGE_H
#define REPTILOIDSCOIN_QT_REPTILOIDSCOINTOOLSPAGE_H

#include <qt/reptiloidscointabbar.h>

#include <qt/clientmodel.h>
#include <qt/walletmodel.h>

#include <QFrame>

class ReptiloidsCoinToolsPage : public QFrame {
    Q_OBJECT
public:
    explicit ReptiloidsCoinToolsPage(int id, QFrame *parent = nullptr) {}

protected:
    WalletModel *walletModel = nullptr;
    int pageID{0};
};

#endif // REPTILOIDSCOIN_QT_REPTILOIDSCOINTOOLSPAGE_H
