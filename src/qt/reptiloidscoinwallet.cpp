// Copyright (c) 2018-2020 The Blocknet developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <qt/reptiloidscoinwallet.h>

#include <qt/reptiloidscoinaddressbook.h>
#include <qt/reptiloidscoinfontmgr.h>
#include <qt/reptiloidscoinquicksend.h>
#include <qt/reptiloidscoinservicenodes.h>
#include <qt/reptiloidscoinsettings.h>

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

ReptiloidsCoinWallet::ReptiloidsCoinWallet(interfaces::Node & node, const PlatformStyle *platformStyle, QFrame *parent)
                              : QFrame(parent), node(node), platformStyle(platformStyle)
{
    ReptiloidsCoinFontMgr::setup();

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

    leftMenu = new ReptiloidsCoinLeftMenu;

    toolbar = new ReptiloidsCoinToolBar(this);
    contentBoxLayout->addWidget(toolbar, 0, Qt::AlignTop);

    layout->addWidget(leftMenu, 0);
    layout->addWidget(contentBox, 1);

    connect(leftMenu, &ReptiloidsCoinLeftMenu::menuChanged, this, &ReptiloidsCoinWallet::setPage);
    connect(toolbar, &ReptiloidsCoinToolBar::passphrase, this, &ReptiloidsCoinWallet::changePassphrase);
    connect(toolbar, &ReptiloidsCoinToolBar::lock, this, &ReptiloidsCoinWallet::onLockRequest);
    connect(toolbar, &ReptiloidsCoinToolBar::progressClicked, this, [this]{
        Q_EMIT progressClicked();
    });
}

bool ReptiloidsCoinWallet::setCurrentWallet(const QString & name) {
    for (WalletModel *w : wallets.values()) {
        disconnect(w, &WalletModel::balanceChanged, this, &ReptiloidsCoinWallet::balanceChanged);
        disconnect(w->getTransactionTableModel(), &TransactionTableModel::rowsInserted, this, &ReptiloidsCoinWallet::processNewTransaction);
        disconnect(w->getOptionsModel(), &OptionsModel::displayUnitChanged, this, &ReptiloidsCoinWallet::displayUnitChanged);
        disconnect(w, &WalletModel::requireUnlock, this, &ReptiloidsCoinWallet::unlockWallet);
        disconnect(w, &WalletModel::showProgress, this, &ReptiloidsCoinWallet::showProgress);
        disconnect(w, &WalletModel::encryptionStatusChanged, this, &ReptiloidsCoinWallet::onEncryptionStatus);
    }

    walletModel = wallets[name];
    if (!walletModel)
        return false;

    connect(walletModel, &WalletModel::balanceChanged, this, &ReptiloidsCoinWallet::balanceChanged);
    connect(walletModel->getTransactionTableModel(), &TransactionTableModel::rowsInserted, this, &ReptiloidsCoinWallet::processNewTransaction);
    connect(walletModel->getOptionsModel(), &OptionsModel::displayUnitChanged, this, &ReptiloidsCoinWallet::displayUnitChanged);
    connect(walletModel, &WalletModel::requireUnlock, this, &ReptiloidsCoinWallet::unlockWallet);
    connect(walletModel, &WalletModel::showProgress, this, &ReptiloidsCoinWallet::showProgress);
    connect(walletModel, &WalletModel::encryptionStatusChanged, this, &ReptiloidsCoinWallet::onEncryptionStatus);

    // Send funds screen
    if (sendFunds == nullptr) {
        sendFunds = new ReptiloidsCoinSendFunds(walletModel);
        connect(sendFunds, &ReptiloidsCoinSendFunds::dashboard, this, &ReptiloidsCoinWallet::goToDashboard);
    } else
        sendFunds->setWalletModel(walletModel);

    // Dashboard screen
    if (dashboard == nullptr) {
        dashboard = new ReptiloidsCoinDashboard;
        dashboard->setWalletModel(walletModel);
        connect(dashboard, &ReptiloidsCoinDashboard::quicksend, this, &ReptiloidsCoinWallet::goToQuickSend);
        connect(dashboard, &ReptiloidsCoinDashboard::history, this, &ReptiloidsCoinWallet::goToHistory);
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

void ReptiloidsCoinWallet::setProgress(int progress, const QString &msg, int maximum) {
    toolbar->setProgress(progress, msg, maximum);
}

void ReptiloidsCoinWallet::updateStakingStatus(WalletModel *w) {
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

void ReptiloidsCoinWallet::setPeers(const int peers) {
    toolbar->setPeers(peers);
}

void ReptiloidsCoinWallet::setLock(const bool lock, const bool stakingOnly) {
    toolbar->setLock(lock, stakingOnly);
}

void ReptiloidsCoinWallet::setPage(ReptiloidsCoinPage page) {
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
        case ReptiloidsCoinPage::DASHBOARD: {
            dashboard->show();
            screen = dashboard;
            break;
        }
        case ReptiloidsCoinPage::ADDRESSBOOK: {
            auto *addressBook = new ReptiloidsCoinAddressBook;
            addressBook->setWalletModel(walletModel);
            connect(addressBook, &ReptiloidsCoinAddressBook::send, this, &ReptiloidsCoinWallet::onSendToAddress);
            screen = addressBook;
            break;
        }
        case ReptiloidsCoinPage::SEND: {
            sendFunds->show();
            screen = sendFunds;
            break;
        }
        case ReptiloidsCoinPage::QUICKSEND: {
            auto *quickSend = new ReptiloidsCoinQuickSend(walletModel);
            connect(quickSend, &ReptiloidsCoinQuickSend::submit, this, &ReptiloidsCoinWallet::onSendFunds);
            connect(quickSend, &ReptiloidsCoinQuickSend::dashboard, this, &ReptiloidsCoinWallet::goToDashboard);
            screen = quickSend;
            break;
        }
        case ReptiloidsCoinPage::REQUEST: {
            std::string platformName = gArgs.GetArg("-uiplatform", "other");
            auto platformStyle = PlatformStyle::instantiate(QString::fromStdString(platformName));
            if (!platformStyle) // Fall back to "other" if specified name not found
                platformStyle = PlatformStyle::instantiate("other");
            auto *recieve = new ReceiveCoinsDialog(platformStyle);
            recieve->setModel(walletModel);
            screen = recieve;
            break;
        }
        case ReptiloidsCoinPage::HISTORY: {
            auto *transactionView = new ReptiloidsCoinTransactionHistory(walletModel);
            screen = transactionView;
            break;
        }
        case ReptiloidsCoinPage::SNODES: {
            auto *snode = new ReptiloidsCoinServiceNodes;
            snode->setClientModel(clientModel);
            screen = snode;
            break;
        }
        case ReptiloidsCoinPage::PROPOSALS: {
            auto *proposals = new ReptiloidsCoinProposals;
            connect(proposals, &ReptiloidsCoinProposals::createProposal, this, &ReptiloidsCoinWallet::goToCreateProposal);
            proposals->setModels(clientModel, walletModel);
            screen = proposals;
            break;
        }
        case ReptiloidsCoinPage::CREATEPROPOSAL: {
            if (createProposal == nullptr) {
                createProposal = new ReptiloidsCoinCreateProposal;
                connect(createProposal, &ReptiloidsCoinCreateProposal::done, this, &ReptiloidsCoinWallet::goToProposals);
            }
            createProposal->setWalletModel(walletModel);
            createProposal->show();
            screen = createProposal;
            break;
        }
        case ReptiloidsCoinPage::SETTINGS: {
            auto *settings = new ReptiloidsCoinSettings(node);
            settings->setWalletModel(walletModel);
            screen = settings;
            break;
        }
//        case ReptiloidsCoinPage::ANNOUNCEMENTS:
        case ReptiloidsCoinPage::TOOLS: {
            if (btools == nullptr) {
                btools = new ReptiloidsCoinTools(node, platformStyle);
                connect(btools, &ReptiloidsCoinTools::handleRestart, this, [this](QStringList args) { Q_EMIT handleRestart(args); });
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

void ReptiloidsCoinWallet::onSendFunds() {
    goToDashboard();
}

void ReptiloidsCoinWallet::onSendToAddress(const QString &address) {
    setPage(ReptiloidsCoinPage::SEND);
    if (sendFunds != nullptr)
        sendFunds->addAddress(address);
}

void ReptiloidsCoinWallet::goToDashboard() {
    setPage(ReptiloidsCoinPage::DASHBOARD);
}

void ReptiloidsCoinWallet::goToQuickSend() {
    setPage(ReptiloidsCoinPage::QUICKSEND);
}

void ReptiloidsCoinWallet::goToHistory() {
    setPage(ReptiloidsCoinPage::HISTORY);
}

void ReptiloidsCoinWallet::goToProposals() {
    setPage(ReptiloidsCoinPage::PROPOSALS);
}

void ReptiloidsCoinWallet::goToCreateProposal() {
    setPage(ReptiloidsCoinPage::CREATEPROPOSAL);
}

void ReptiloidsCoinWallet::balanceChanged(const interfaces::WalletBalances & balances) {
    leftMenu->setBalance(balances.balance, walletModel->getOptionsModel() ? walletModel->getOptionsModel()->getDisplayUnit() : 0);
    if (dashboard)
        dashboard->balanceChanged(balances);
}

void ReptiloidsCoinWallet::displayUnitChanged(const int unit) {
    balanceChanged(walletModel->wallet().getBalances());
}

void ReptiloidsCoinWallet::changePassphrase() {
    if (!walletModel)
        return;
    AskPassphraseDialog dlg(AskPassphraseDialog::ChangePass, this);
    dlg.setObjectName("redesign");
    dlg.setModel(walletModel);
    dlg.exec();
}

void ReptiloidsCoinWallet::encryptWallet(bool status) {
    if (!walletModel)
        return;
    AskPassphraseDialog dlg(status ? AskPassphraseDialog::Encrypt : AskPassphraseDialog::Decrypt, this);
    dlg.setObjectName("redesign");
    dlg.setModel(walletModel);
    dlg.exec();

    Q_EMIT encryptionStatusChanged(walletModel->getEncryptionStatus());
}

void ReptiloidsCoinWallet::backupWallet() {
    QString filename = GUIUtil::getSaveFileName(this, tr("Backup Wallet"), QString(),
            tr("Wallet Data (*.dat)"), nullptr);

    if (filename.isEmpty())
        return;

    if (!walletModel->wallet().backupWallet(filename.toLocal8Bit().data()))
        QMessageBox::warning(this, tr("Backup Failed"), tr("There was an error trying to save the wallet data to %1.").arg(filename));
    else
        QMessageBox::information(this, tr("Backup Successful"), tr("The wallet data was successfully saved to %1.").arg(filename));
}

void ReptiloidsCoinWallet::onLockRequest(bool locked, bool stakingOnly) {
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

void ReptiloidsCoinWallet::onEncryptionStatus() {
    setLock(walletModel->getEncryptionStatus() == WalletModel::EncryptionStatus::Locked, util::unlockedForStakingOnly);
    updateStakingStatus(walletModel);
}

void ReptiloidsCoinWallet::usedSendingAddresses() {
    if (!walletModel)
        return;
    ReptiloidsCoinAddressBookDialog dlg(walletModel, Qt::WindowSystemMenuHint | Qt::WindowTitleHint, ReptiloidsCoinAddressBook::FILTER_SENDING);
    dlg.exec();
}

void ReptiloidsCoinWallet::usedReceivingAddresses() {
    if (!walletModel)
        return;
    ReptiloidsCoinAddressBookDialog dlg(walletModel, Qt::WindowSystemMenuHint | Qt::WindowTitleHint, ReptiloidsCoinAddressBook::FILTER_RECEIVING);
    dlg.exec();
}

void ReptiloidsCoinWallet::unlockWallet() {
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

void ReptiloidsCoinWallet::showProgress(const QString &title, int nProgress) {
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

void ReptiloidsCoinWallet::gotoSignMessageTab(QString addr) {
    // calls show() in showTab_VM()
    auto *signVerifyMessageDialog = new SignVerifyMessageDialog(platformStyle, this);
    signVerifyMessageDialog->setAttribute(Qt::WA_DeleteOnClose);
    signVerifyMessageDialog->setModel(walletModel);
    signVerifyMessageDialog->showTab_SM(true);

    if (!addr.isEmpty())
        signVerifyMessageDialog->setAddress_SM(addr);
}

void ReptiloidsCoinWallet::gotoVerifyMessageTab(QString addr) {
    // calls show() in showTab_VM()
    auto *signVerifyMessageDialog = new SignVerifyMessageDialog(platformStyle, this);
    signVerifyMessageDialog->setAttribute(Qt::WA_DeleteOnClose);
    signVerifyMessageDialog->setModel(walletModel);
    signVerifyMessageDialog->showTab_VM(true);

    if (!addr.isEmpty())
        signVerifyMessageDialog->setAddress_VM(addr);
}

void ReptiloidsCoinWallet::processNewTransaction(const QModelIndex& parent, int start, int /*end*/) {
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