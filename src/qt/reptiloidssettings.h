// Copyright (c) 2018-2019 The Blocknet developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef REPTILOIDS_QT_REPTILOIDSSETTINGS_H
#define REPTILOIDS_QT_REPTILOIDSSETTINGS_H

#include <qt/reptiloidsdropdown.h>
#include <qt/reptiloidsformbtn.h>
#include <qt/reptiloidslabelbtn.h>
#include <qt/reptiloidslineeditwithtitle.h>

#include <qt/walletmodel.h>

#include <QCheckBox>
#include <QDataWidgetMapper>
#include <QFrame>
#include <QLabel>
#include <QScrollArea>
#include <QSpinBox>
#include <QVBoxLayout>
#include <QWidget>

class ReptiloidsSettings : public QFrame 
{
    Q_OBJECT

public:
    explicit ReptiloidsSettings(interfaces::Node & node, QWidget *parent = nullptr);
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
    ReptiloidsLabelBtn *aboutCoreLblBtn;
    ReptiloidsLabelBtn *aboutQtLblBtn;
    QLabel *generalLbl;
    QCheckBox *startWalletOnLoginCb;
    QLabel *sizeDbCacheLbl;
    QSpinBox *dbCacheSb;
    QLabel *verificationThreadsLbl;
    QSpinBox *threadsSb;
    QLabel *walletLbl;
    QCheckBox *spendChangeCb;
    ReptiloidsFormBtn *backupBtn;
    QLabel *networkLbl;
    QCheckBox *upnpCb;
    QCheckBox *allowIncomingCb;
    QCheckBox *connectSocks5Cb;
    ReptiloidsLineEditWithTitle *proxyTi;
    ReptiloidsLineEditWithTitle *portTi;
    QLabel *displayLbl;
    QLabel *languageLbl;
    ReptiloidsDropdown *languageDropdown;
    QLabel *contributeLbl;
    ReptiloidsLabelBtn *contributeLblBtn;
    QLabel *unitsLbl;
    ReptiloidsDropdown *unitsDropdown;
    QLabel *decimalLbl;
    ReptiloidsDropdown *decimalDropdown;
    ReptiloidsLineEditWithTitle *thirdPartyUrlTi;
    ReptiloidsFormBtn *saveBtn;
    ReptiloidsFormBtn *resetBtn;
};

#endif // REPTILOIDS_QT_REPTILOIDSSETTINGS_H
