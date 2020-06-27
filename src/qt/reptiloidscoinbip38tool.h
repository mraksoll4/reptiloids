// Copyright (c) 2018-2019 The Blocknet developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef REPTILOIDSCOIN_QT_REPTILOIDSCOINBIP38TOOL_H
#define REPTILOIDSCOIN_QT_REPTILOIDSCOINBIP38TOOL_H

#include <qt/reptiloidscoinaddressbtn.h>
#include <qt/reptiloidscoinformbtn.h>
#include <qt/reptiloidscoinlineeditwithtitle.h>
#include <qt/reptiloidscointoolspage.h>

#include <qt/walletmodel.h>

#include <QFrame>
#include <QLabel>
#include <QScrollArea>
#include <QVBoxLayout>

class ReptiloidsCoinBIP38Tool : public ReptiloidsCoinToolsPage {
    Q_OBJECT

public:
    explicit ReptiloidsCoinBIP38Tool(QWidget *popup, int id, QFrame *parent = nullptr);
    void setWalletModel(WalletModel *w);

private Q_SLOTS:
    void onGenerateEncryptedKey();
    void onDecryptKey();

private:
    QScrollArea *scrollArea;
    QFrame *content;
    QVBoxLayout *contentLayout;
    QVBoxLayout *layout;
    QLabel *titleLbl;
    QWidget *popupWidget;
    ReptiloidsCoinLineEditWithTitle *addressTi;
    ReptiloidsCoinAddressBtn *addressBtn;
    ReptiloidsCoinLineEditWithTitle *addressPasswordTi;
    ReptiloidsCoinFormBtn *generateBtn;
    ReptiloidsCoinLineEditWithTitle *keyTi;
    ReptiloidsCoinLineEditWithTitle *keyPasswordTi;
    ReptiloidsCoinFormBtn *decryptBtn;
};

#endif // REPTILOIDSCOIN_QT_REPTILOIDSCOINBIP38TOOL_H
