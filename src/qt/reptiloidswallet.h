// Copyright (c) 2018-2020 The Blocknet developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef REPTILOIDS_QT_REPTILOIDSWALLET_H
#define REPTILOIDS_QT_REPTILOIDSWALLET_H

#include <qt/reptiloidscreateproposal.h>
#include <qt/reptiloidsdashboard.h>
#include <qt/reptiloidsleftmenu.h>
#include <qt/reptiloidsproposals.h>
#include <qt/reptiloidssendfunds.h>
#include <qt/reptiloidstoolbar.h>
#include <qt/reptiloidstools.h>
#include <qt/reptiloidstransactionhistory.h>
#include <qt/reptiloidsvars.h>

#include <qt/clientmodel.h>
#include <qt/platformstyle.h>
#include <qt/walletmodel.h>

#include <QHash>
#include <QList>
#include <QProgressDialog>

class ReptiloidsWallet : public QFrame {
    Q_OBJECT

public:
    explicit ReptiloidsWallet(interfaces::Node & node, const PlatformStyle *platformStyle, QFrame *parent = nullptr);

    void setClientModel(ClientModel *c) { this->clientModel = c; }

    bool addWallet(WalletModel *w) { this->wallets[w->getWalletName()] = w; return true; };
    bool setCurrentWallet(const QString &name);
    bool setCurrentWallet(WalletModel *w) {
        if (!this->wallets.count(w->getWalletName()))
            this->wallets[w->getWalletName()] = w;
        return setCurrentWallet(w->getWalletName());
    };
    bool removeWallet(const QString &name) { this->wallets.remove(name); return true; };
    bool removeWallet(WalletModel *w) { return removeWallet(w->getWalletName()); };
    void removeAllWallets() { this->wallets.clear(); };
    bool handlePaymentRequest(const SendCoinsRecipient & recipient) { return false; }; // TODO Reptiloids Qt payment requests
    void showOutOfSyncWarning(bool fShow) { Q_EMIT requestedSyncWarningInfo(); };

    WalletModel* currentWalletModel() const { return walletModel; };

    void setProgress(int progress, const QString &msg, int maximum = 100);
    void updateStakingStatus(WalletModel *w);
    void setPeers(int peers);
    void setLock(bool lock, bool stakingOnly = false); // TODO Handle timed unlock

Q_SIGNALS:
    void encryptionStatusChanged(WalletModel::EncryptionStatus encryptStatus);
    void addressbook();
    void request();
    void tools();
    void handleRestart(QStringList args);
    void requestedSyncWarningInfo();
    /** Notify that a new transaction appeared */
    void incomingTransaction(const QString& date, int unit, const CAmount& amount, const QString& type,
            const QString& address, const QString& label, const QString& walletName);
    void progressClicked();


public Q_SLOTS:
    void setPage(ReptiloidsPage page);
    void gotoOverviewPage() { goToDashboard(); };
    void gotoHistoryPage() { goToHistory(); };
    void gotoReceiveCoinsPage() { }; // TODO Reptiloids Qt wire up receive coins page
    void gotoSendCoinsPage(QString addr = "") { onSendToAddress(addr); };
    void gotoSettingsPage() { setPage(ReptiloidsPage::SETTINGS); };
    void gotoSignMessageTab(QString addr);
    void gotoVerifyMessageTab(QString addr);
    void encryptWallet(bool status);
    void backupWallet();
    void changePassphrase();
    void unlockWallet();
    void showProgress(const QString &title, int nProgress);
    void usedSendingAddresses();
    void usedReceivingAddresses();
    void onLockRequest(bool locked, bool stakingOnly);

protected Q_SLOTS:
    void onSendFunds();
    void onSendToAddress(const QString &);
    void goToDashboard();
    void goToQuickSend();
    void goToHistory();
    void goToCreateProposal();
    void goToProposals();
    void balanceChanged(const interfaces::WalletBalances & balances);
    void displayUnitChanged(int unit);
    void processNewTransaction(const QModelIndex& parent, int start, int /*end*/);
    void onEncryptionStatus();

private:
    interfaces::Node &node;
    const PlatformStyle *platformStyle;
    QHash<QString, WalletModel*> wallets;
    ClientModel *clientModel;
    WalletModel *walletModel;
    ReptiloidsPage page;

    ReptiloidsLeftMenu *leftMenu;
    QFrame *contentBox;
    ReptiloidsToolBar *toolbar;
    ReptiloidsSendFunds *sendFunds = nullptr;
    ReptiloidsDashboard *dashboard = nullptr;
    ReptiloidsCreateProposal *createProposal = nullptr;
    ReptiloidsTools *btools = nullptr;
    QWidget *screen = nullptr;
    QProgressDialog *progressDialog = nullptr;
};

#endif // REPTILOIDS_QT_REPTILOIDSWALLET_H
