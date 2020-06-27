// Copyright (c) 2018-2019 The Blocknet developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef REPTILOIDSCOIN_QT_REPTILOIDSCOINWALLETREPAIR_H
#define REPTILOIDSCOIN_QT_REPTILOIDSCOINWALLETREPAIR_H

#include <qt/reptiloidscoinformbtn.h>
#include <qt/reptiloidscointoolspage.h>

#include <QFrame>
#include <QHBoxLayout>
#include <QLabel>
#include <QScrollArea>
#include <QTextEdit>
#include <QVBoxLayout>

class ReptiloidsCoinWalletRepair : public ReptiloidsCoinToolsPage {
    Q_OBJECT

public:
    explicit ReptiloidsCoinWalletRepair(QWidget *popup, int id, QFrame *parent = nullptr);
    void setWalletModel(WalletModel *w);

public Q_SLOTS:
    void walletSalvage();
    void walletRescan();
    void walletZaptxes1();
    void walletZaptxes2();
    void walletUpgrade();
    void walletReindex();

Q_SIGNALS:
    /** Get restart command-line parameters and handle restart */
    void handleRestart(QStringList args);

private:
    QScrollArea *scrollArea;
    QFrame *content;
    QVBoxLayout *contentLayout;
    QVBoxLayout *layout;
    QLabel *titleLbl;
    QWidget *popupWidget;
    QLabel *descriptionTxt;
    QFrame *walletFrame;
    QHBoxLayout *walletLayout;
    QFrame *salvageFrame;
    QVBoxLayout *salvageLayout;
    QLabel *salvageTitleLbl;
    QLabel *salvageDescLbl;
    ReptiloidsCoinFormBtn *salvageWalletBtn;
    QFrame *rescanFrame;
    QHBoxLayout *rescanLayout;
    QFrame *blockchainFrame;
    QVBoxLayout *blockchainLayout;
    QLabel *rescanTitleLbl;
    QLabel *rescanDescLbl;
    ReptiloidsCoinFormBtn *rescanBlockchainBtn;
    QFrame *transaction1Frame;
    QHBoxLayout *transaction1Layout;
    QFrame *recoverFrame;
    QVBoxLayout *recoverLayout;
    QLabel *transaction1TitleLbl;
    QLabel *transaction1DescLbl;
    ReptiloidsCoinFormBtn *transaction1Btn;
    QFrame *transaction2Frame;
    QHBoxLayout *transaction2Layout;
    QFrame *recover2Frame;
    QVBoxLayout *recover2Layout;
    QLabel *transaction2TitleLbl;
    QLabel *transaction2DescLbl;
    ReptiloidsCoinFormBtn *transaction2Btn;
    QFrame *upgradeFrame;
    QHBoxLayout *upgradeLayout;
    QFrame *formatFrame;
    QVBoxLayout *formatLayout;
    QLabel *formatTitleLbl;
    QLabel *formatDescLbl;
    ReptiloidsCoinFormBtn *upgradeBtn;
    QFrame *rebuildFrame;
    QHBoxLayout *rebuildLayout;
    QFrame *indexFrame;
    QVBoxLayout *indexLayout;
    QLabel *indexTitleLbl;
    QLabel *indexDescLbl;
    ReptiloidsCoinFormBtn *rebuildBtn;

    /** Build parameter list for restart */
    void buildParameterlist(QString arg);
    /** Display QMessageBox */
    bool displayConfirmationBox(const QString & title, const QString & msg);
};

#endif // REPTILOIDSCOIN_QT_REPTILOIDSCOINWALLETREPAIR_H
