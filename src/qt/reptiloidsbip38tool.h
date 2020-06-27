// Copyright (c) 2018-2019 The Blocknet developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef REPTILOIDS_QT_REPTILOIDSBIP38TOOL_H
#define REPTILOIDS_QT_REPTILOIDSBIP38TOOL_H

#include <qt/reptiloidsaddressbtn.h>
#include <qt/reptiloidsformbtn.h>
#include <qt/reptiloidslineeditwithtitle.h>
#include <qt/reptiloidstoolspage.h>

#include <qt/walletmodel.h>

#include <QFrame>
#include <QLabel>
#include <QScrollArea>
#include <QVBoxLayout>

class ReptiloidsBIP38Tool : public ReptiloidsToolsPage {
    Q_OBJECT

public:
    explicit ReptiloidsBIP38Tool(QWidget *popup, int id, QFrame *parent = nullptr);
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
    ReptiloidsLineEditWithTitle *addressTi;
    ReptiloidsAddressBtn *addressBtn;
    ReptiloidsLineEditWithTitle *addressPasswordTi;
    ReptiloidsFormBtn *generateBtn;
    ReptiloidsLineEditWithTitle *keyTi;
    ReptiloidsLineEditWithTitle *keyPasswordTi;
    ReptiloidsFormBtn *decryptBtn;
};

#endif // REPTILOIDS_QT_REPTILOIDSBIP38TOOL_H
