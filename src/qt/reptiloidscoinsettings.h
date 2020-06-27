// Copyright (c) 2018-2019 The Blocknet developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef REPTILOIDSCOIN_QT_REPTILOIDSCOINSETTINGS_H
#define REPTILOIDSCOIN_QT_REPTILOIDSCOINSETTINGS_H

#include <qt/reptiloidscoindropdown.h>
#include <qt/reptiloidscoinformbtn.h>
#include <qt/reptiloidscoinlabelbtn.h>
#include <qt/reptiloidscoinlineeditwithtitle.h>

#include <qt/walletmodel.h>

#include <QCheckBox>
#include <QDataWidgetMapper>
#include <QFrame>
#include <QLabel>
#include <QScrollArea>
#include <QSpinBox>
#include <QVBoxLayout>
#include <QWidget>

class ReptiloidsCoinSettings : public QFrame 
{
    Q_OBJECT

public:
    explicit ReptiloidsCoinSettings(interfaces::Node & node, QWidget *parent = nullptr);
    void setWalletModel(WalletModel *w);
    void backupWallet();

private Q_SLOTS:
    void onResetSettingsToDefault();

private:
    interfaces::Node &node;
    QVBoxLayout *layout;
    QDataWidgetMapper *mapper;
    QScrollArea *scrollArea;
    QFrame *content;
    WalletModel *walletModel;
    QVBoxLayout *contentLayout;
    QLabel *titleLbl;
    ReptiloidsCoinLabelBtn *aboutCoreLblBtn;
    ReptiloidsCoinLabelBtn *aboutQtLblBtn;
    QLabel *generalLbl;
    QCheckBox *startWalletOnLoginCb;
    QLabel *sizeDbCacheLbl;
    QSpinBox *dbCacheSb;
    QLabel *verificationThreadsLbl;
    QSpinBox *threadsSb;
    QLabel *walletLbl;
    QCheckBox *spendChangeCb;
    ReptiloidsCoinFormBtn *backupBtn;
    QLabel *networkLbl;
    QCheckBox *upnpCb;
    QCheckBox *allowIncomingCb;
    QCheckBox *connectSocks5Cb;
    ReptiloidsCoinLineEditWithTitle *proxyTi;
    ReptiloidsCoinLineEditWithTitle *portTi;
    QLabel *displayLbl;
    QLabel *languageLbl;
    ReptiloidsCoinDropdown *languageDropdown;
    QLabel *contributeLbl;
    ReptiloidsCoinLabelBtn *contributeLblBtn;
    QLabel *unitsLbl;
    ReptiloidsCoinDropdown *unitsDropdown;
    QLabel *decimalLbl;
    ReptiloidsCoinDropdown *decimalDropdown;
    ReptiloidsCoinLineEditWithTitle *thirdPartyUrlTi;
    ReptiloidsCoinFormBtn *saveBtn;
    ReptiloidsCoinFormBtn *resetBtn;
};

#endif // REPTILOIDSCOIN_QT_REPTILOIDSCOINSETTINGS_H
