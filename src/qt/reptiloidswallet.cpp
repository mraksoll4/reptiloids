// Copyright (c) 2018-2020 The Blocknet developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <qt/reptiloidswallet.h>

#include <qt/reptiloidsaddressbook.h>
#include <qt/reptiloidsfontmgr.h>
#include <qt/reptiloidsquicksend.h>
#include <qt/reptiloidsservicenodes.h>
#include <qt/reptiloidssettings.h>

#include <qt/askpassphrasedialog.h>
#include <qt/optionsmodel.h>
#include <qt/platformstyle.h>
#include <qt/receivecoinsdialog.h>
#include <qt/signverifymessagedialog.h>
#include <qt/transactionfilterproxy.h>
#include <qt/transactiontablemodel.h>
#include <qt/transactionview.h>

#include <util/system.h>

#include <QSettings>

ReptiloidsWallet::ReptiloidsWallet(interfaces::Node & node, const PlatformStyle *platformStyle, QFrame *parent)
                              : QFrame(parent), node(node), platformStyle(platformStyle)
{
    ReptiloidsFontMgr::setup();

    this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    auto *layout = new QHBoxLayout;
    layout->setContentsMargins(QMargins());
    layout->setSpacing(0);
    this->setLayout(layout);

    contentBox = new QFrame;
    contentBox->setObjectName("contentBox");
    auto *contentBoxLayout = new QVBoxLayout;
    contentBoxLayout->setContentsMargins(QMargins());
    contentBoxLayout->setSpacing(0);
    contentBox->setLayout(contentBoxLayout);

    leftMenu = new ReptiloidsLeftMenu;

    toolbar = new ReptiloidsToolBar(this);
    contentBoxLayout->addWidget(toolbar, 0, Qt::AlignTop);

    layout->addWidget(leftMenu, 0);
    layout->addWidget(contentBox, 1);

    connect(leftMenu, &ReptiloidsLeftMenu::menuChanged, this, &ReptiloidsWallet::setPage);
    connect(toolbar, &ReptiloidsToolBar::passphrase, this, &ReptiloidsWallet::changePassphrase);
    connect(toolbar, &ReptiloidsToolBar::lock, this, &ReptiloidsWallet::onLockRequest);
    connect(toolbar, &ReptiloidsToolBar::progressClicked, this, [this]{
        Q_EMIT progressClicked();
    });
}

bool ReptiloidsWallet::setCurrentWallet(const QString & name) {
    for (WalletModel *w : wallets.values()) {
        disconnect(w, &WalletModel::balanceChanged, this, &ReptiloidsWallet::balanceChanged);
        disconnect(w->getTransactionTableModel(), &TransactionTableModel::rowsInserted, this, &ReptiloidsWallet::processNewTransaction);
        disconnect(w->getOptionsModel(), &OptionsModel::displayUnitChanged, this, &ReptiloidsWallet::displayUnitChanged);
        disconnect(w, &WalletModel::requireUnlock, this, &ReptiloidsWallet::unlockWallet);
        disconnect(w, &WalletModel::showProgress, this, &ReptiloidsWallet::showProgress);
        disconnect(w, &WalletModel::encryptionStatusChanged, this, &ReptiloidsWallet::onEncryptionStatus);
    }

    walletModel = wallets[name];
    if (!walletModel)
        return false;

    connect(walletModel, &WalletModel::balanceChanged, this, &ReptiloidsWallet::balanceChanged);
    connect(walletModel->getTransactionTableModel(), &TransactionTableModel::rowsInserted, this, &ReptiloidsWallet::processNewTransaction);
    connect(walletModel->getOptionsModel(), &OptionsModel::displayUnitChanged, this, &ReptiloidsWallet::displayUnitChanged);
    connect(walletModel, &WalletModel::requireUnlock, this, &ReptiloidsWallet::unlockWallet);
    connect(walletModel, &WalletModel::showProgress, this, &ReptiloidsWallet::showProgress);
    connect(walletModel, &WalletModel::encryptionStatusChanged, this, &ReptiloidsWallet::onEncryptionStatus);

    // Send funds screen
    if (sendFunds == nullptr) {
        sendFunds = new ReptiloidsSendFunds(walletModel);
        connect(sendFunds, &ReptiloidsSendFunds::dashboard, this, &ReptiloidsWallet::goToDashboard);
    } else
        sendFunds->setWalletModel(walletModel);

    // Dashboard screen
    if (dashboard == nullptr) {
        dashboard = new ReptiloidsDashboard;
        dashboard->setWalletModel(walletModel);
        connect(dashboard, &ReptiloidsDashboard::quicksend, this, &ReptiloidsWallet::goToQuickSend);
        connect(dashboard, &ReptiloidsDashboard::history, this, &ReptiloidsWallet::goToHistory);
    } else
        dashboard->setWalletModel(walletModel);

    // wallet lock state
    setLock(walletModel->getEncryptionStatus() == WalletModel::Locked, util::unlockedForStakingOnly);

    // Update balances
    balanceChanged(walletModel->wallet().getBalances());

    // Staking
    updateStakingStatus(walletModel);

    return true;
}

void ReptiloidsWallet::setProgress(int progress, const QString &msg, int maximum) {
    toolbar->setProgress(progress, msg, maximum);
}

void ReptiloidsWallet::updateStakingStatus(WalletModel *w) {
    const auto staking = gArgs.GetBoolArg("-staking", true);
    auto msg = tr("Staking is off");
    const auto canStake = staking && w->wallet().getBalance() > 0
            && (util::unlockedForStakingOnly || w->getEncryptionStatus() == WalletModel::EncryptionStatus::Unlocked
                                             || w->getEncryptionStatus() == WalletModel::EncryptionStatus::Unencrypted);
    if (canStake)
        msg = tr("Staking is active");
    else if (staking && w->wallet().getBalance() <= 0)
        msg = tr("Staking is off, your staking balance is 0");
    else if (staking)
        msg = tr("Staking is pending, please unlock the wallet to stake funds");
    toolbar->setStaking(canStake , msg);
}

void ReptiloidsWallet::setPeers(const int peers) {
    toolbar->setPeers(peers);
}

void ReptiloidsWallet::setLock(const bool lock, const bool stakingOnly) {
    toolbar->setLock(lock, stakingOnly);
}

void ReptiloidsWallet::setPage(ReptiloidsPage page) {
    bool same = this->page == page;
    leftMenu->selectMenu(page);

    if (same)
        return;
    this->page = page;

    if (screen) {
        screen->hide();
        contentBox->layout()->removeWidget(screen);
        if (screen != sendFunds && screen != dashboard && screen != createProposal && screen != btools)
            screen->deleteLater();
    }

    switch (page) {
        case ReptiloidsPage::DASHBOARD: {
            dashboard->show();
            screen = dashboard;
            break;
        }
        case ReptiloidsPage::ADDRESSBOOK: {
            auto *addressBook = new ReptiloidsAddressBook;
            addressBook->setWalletModel(walletModel);
            connect(addressBook, &ReptiloidsAddressBook::send, this, &ReptiloidsWallet::onSendToAddress);
            screen = addressBook;
            break;
        }
        case ReptiloidsPage::SEND: {
            sendFunds->show();
            screen = sendFunds;
            break;
        }
        case ReptiloidsPage::QUICKSEND: {
            auto *quickSend = new ReptiloidsQuickSend(walletModel);
            connect(quickSend, &ReptiloidsQuickSend::submit, this, &ReptiloidsWallet::onSendFunds);
            connect(quickSend, &ReptiloidsQuickSend::dashboard, this, &ReptiloidsWallet::goToDashboard);
            screen = quickSend;
            break;
        }
        case ReptiloidsPage::REQUEST: {
            std::string platformName = gArgs.GetArg("-uiplatform", "other");
            auto platformStyle = PlatformStyle::instantiate(QString::fromStdString(platformName));
            if (!platformStyle) // Fall back to "other" if specified name not found
                platformStyle = PlatformStyle::instantiate("other");
            auto *recieve = new ReceiveCoinsDialog(platformStyle);
            recieve->setModel(walletModel);
            screen = recieve;
            break;
        }
        case ReptiloidsPage::HISTORY: {
            auto *transactionView = new ReptiloidsTransactionHistory(walletModel);
            screen = transactionView;
            break;
        }
        case ReptiloidsPage::SNODES: {
            auto *snode = new ReptiloidsServiceNodes;
            snode->setClientModel(clientModel);
            screen = snode;
            break;
        }
        case ReptiloidsPage::PROPOSALS: {
            auto *proposals = new ReptiloidsProposals;
            connect(proposals, &ReptiloidsProposals::createProposal, this, &ReptiloidsWallet::goToCreateProposal);
            proposals->setModels(clientModel, walletModel);
            screen = proposals;
            break;
        }
        case ReptiloidsPage::CREATEPROPOSAL: {
            if (createProposal == nullptr) {
                createProposal = new ReptiloidsCreateProposal;
                connect(createProposal, &ReptiloidsCreateProposal::done, this, &ReptiloidsWallet::goToProposals);
            }
            createProposal->setWalletModel(walletModel);
            createProposal->show();
            screen = createProposal;
            break;
        }
        case ReptiloidsPage::SETTINGS: {
            auto *settings = new ReptiloidsSettings(node);
            settings->setWalletModel(walletModel);
            screen = settings;
            break;
        }
//        case ReptiloidsPage::ANNOUNCEMENTS:
        case ReptiloidsPage::TOOLS: {
            if (btools == nullptr) {
                btools = new ReptiloidsTools(node, platformStyle);
                connect(btools, &ReptiloidsTools::handleRestart, this, [this](QStringList args) { Q_EMIT handleRestart(args); });
            }
            btools->setModels(clientModel, walletModel);
            btools->show();
            screen = btools;
            break;
        }
        default:
            screen = new QFrame;
            break;
    }

    contentBox->layout()->addWidget(screen);
    screen->setFocus();
}

void ReptiloidsWallet::onSendFunds() {
    goToDashboard();
}

void ReptiloidsWallet::onSendToAddress(const QString &address) {
    setPage(ReptiloidsPage::SEND);
    if (sendFunds != nullptr)
        sendFunds->addAddress(address);
}

void ReptiloidsWallet::goToDashboard() {
    setPage(ReptiloidsPage::DASHBOARD);
}

void ReptiloidsWallet::goToQuickSend() {
    setPage(ReptiloidsPage::QUICKSEND);
}

void ReptiloidsWallet::goToHistory() {
    setPage(ReptiloidsPage::HISTORY);
}

void ReptiloidsWallet::goToProposals() {
    setPage(ReptiloidsPage::PROPOSALS);
}

void ReptiloidsWallet::goToCreateProposal() {
    setPage(ReptiloidsPage::CREATEPROPOSAL);
}

void ReptiloidsWallet::balanceChanged(const interfaces::WalletBalances & balances) {
    leftMenu->setBalance(balances.balance, walletModel->getOptionsModel() ? walletModel->getOptionsModel()->getDisplayUnit() : 0);
    if (dashboard)
        dashboard->balanceChanged(balances);
}

void ReptiloidsWallet::displayUnitChanged(const int unit) {
    balanceChanged(walletModel->wallet().getBalances());
}

void ReptiloidsWallet::changePassphrase() {
    if (!walletModel)
        return;
    AskPassphraseDialog dlg(AskPassphraseDialog::ChangePass, this);
    dlg.setObjectName("redesign");
    dlg.setModel(walletModel);
    dlg.exec();
}

void ReptiloidsWallet::encryptWallet(bool status) {
    if (!walletModel)
        return;
    AskPassphraseDialog dlg(status ? AskPassphraseDialog::Encrypt : AskPassphraseDialog::Decrypt, this);
    dlg.setObjectName("redesign");
    dlg.setModel(walletModel);
    dlg.exec();

    Q_EMIT encryptionStatusChanged(walletModel->getEncryptionStatus());
}

void ReptiloidsWallet::backupWallet() {
    QString filename = GUIUtil::getSaveFileName(this, tr("Backup Wallet"), QString(),
            tr("Wallet Data (*.dat)"), nullptr);

    if (filename.isEmpty())
        return;

    if (!walletModel->wallet().backupWallet(filename.toLocal8Bit().data()))
        QMessageBox::warning(this, tr("Backup Failed"), tr("There was an error trying to save the wallet data to %1.").arg(filename));
    else
        QMessageBox::information(this, tr("Backup Successful"), tr("The wallet data was successfully saved to %1.").arg(filename));
}

void ReptiloidsWallet::onLockRequest(bool locked, bool stakingOnly) {
    if (locked) {
        util::unlockedForStakingOnly = false;
        walletModel->setWalletLocked(locked);
    } else {
        // Unlock wallet when requested by wallet model
        AskPassphraseDialog dlg(AskPassphraseDialog::Unlock, this);
        dlg.setObjectName("redesign");
        dlg.setModel(walletModel);
        if (stakingOnly)
            dlg.stakingOnly();
        dlg.exec();
    }
    Q_EMIT encryptionStatusChanged(walletModel->getEncryptionStatus());
}

void ReptiloidsWallet::onEncryptionStatus() {
    setLock(walletModel->getEncryptionStatus() == WalletModel::EncryptionStatus::Locked, util::unlockedForStakingOnly);
    updateStakingStatus(walletModel);
}

void ReptiloidsWallet::usedSendingAddresses() {
    if (!walletModel)
        return;
    ReptiloidsAddressBookDialog dlg(walletModel, Qt::WindowSystemMenuHint | Qt::WindowTitleHint, ReptiloidsAddressBook::FILTER_SENDING);
    dlg.exec();
}

void ReptiloidsWallet::usedReceivingAddresses() {
    if (!walletModel)
        return;
    ReptiloidsAddressBookDialog dlg(walletModel, Qt::WindowSystemMenuHint | Qt::WindowTitleHint, ReptiloidsAddressBook::FILTER_RECEIVING);
    dlg.exec();
}

void ReptiloidsWallet::unlockWallet() {
    if (!walletModel)
        return;
    // Unlock wallet when requested by wallet model
    if (walletModel->getEncryptionStatus() == WalletModel::Locked || util::unlockedForStakingOnly) {
        AskPassphraseDialog dlg(AskPassphraseDialog::Unlock, this);
        dlg.setObjectName("redesign");
        dlg.setModel(walletModel);
        dlg.exec();
    }
}

void ReptiloidsWallet::showProgress(const QString &title, int nProgress) {
    if (nProgress == 0) {
        progressDialog = new QProgressDialog(title, tr("Cancel"), 0, 100);
        GUIUtil::PolishProgressDialog(progressDialog);
        progressDialog->setWindowModality(Qt::ApplicationModal);
        progressDialog->setMinimumDuration(0);
        progressDialog->setAutoClose(false);
        progressDialog->setValue(0);
    } else if (nProgress == 100) {
        if (progressDialog) {
            progressDialog->close();
            progressDialog->deleteLater();
        }
    } else if (progressDialog) {
        if (progressDialog->wasCanceled()) {
            walletModel->wallet().abortRescan();
        } else {
            progressDialog->setValue(nProgress);
        }
    }
}

void ReptiloidsWallet::gotoSignMessageTab(QString addr) {
    // calls show() in showTab_VM()
    auto *signVerifyMessageDialog = new SignVerifyMessageDialog(platformStyle, this);
    signVerifyMessageDialog->setAttribute(Qt::WA_DeleteOnClose);
    signVerifyMessageDialog->setModel(walletModel);
    signVerifyMessageDialog->showTab_SM(true);

    if (!addr.isEmpty())
        signVerifyMessageDialog->setAddress_SM(addr);
}

void ReptiloidsWallet::gotoVerifyMessageTab(QString addr) {
    // calls show() in showTab_VM()
    auto *signVerifyMessageDialog = new SignVerifyMessageDialog(platformStyle, this);
    signVerifyMessageDialog->setAttribute(Qt::WA_DeleteOnClose);
    signVerifyMessageDialog->setModel(walletModel);
    signVerifyMessageDialog->showTab_VM(true);

    if (!addr.isEmpty())
        signVerifyMessageDialog->setAddress_VM(addr);
}

void ReptiloidsWallet::processNewTransaction(const QModelIndex& parent, int start, int /*end*/) {
    // Prevent balloon-spam when initial block download is in progress
    if (!walletModel || !clientModel || clientModel->node().isInitialBlockDownload())
        return;

    TransactionTableModel *ttm = walletModel->getTransactionTableModel();
    if (!ttm || ttm->processingQueuedTransactions())
        return;

    QString date = ttm->index(start, TransactionTableModel::Date, parent).data().toString();
    qint64 amount = ttm->index(start, TransactionTableModel::Amount, parent).data(Qt::EditRole).toULongLong();
    QString type = ttm->index(start, TransactionTableModel::Type, parent).data().toString();
    QModelIndex index = ttm->index(start, 0, parent);
    QString address = ttm->data(index, TransactionTableModel::AddressRole).toString();
    QString label = ttm->data(index, TransactionTableModel::LabelRole).toString();

    Q_EMIT incomingTransaction(date, walletModel->getOptionsModel()->getDisplayUnit(), amount, type, address,
            label, walletModel->getWalletName());
}