// Copyright (c) 2018-2019 The Blocknet developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <qt/reptiloidscoinlockmenu.h>

#include <qt/reptiloidscoinguiutil.h>

#include <QLabel>
#include <QEvent>
#include <QDebug>

ReptiloidsCoinLockMenu::ReptiloidsCoinLockMenu(QFrame *parent) : QFrame(parent), layout(new QVBoxLayout) {
    layout->setContentsMargins(QMargins());
    layout->setSpacing(0);
    this->setMaximumSize(BGU::spi(285), BGU::spi(290));
    this->setLayout(layout);

    lockWalletBtn = new QPushButton(tr("Lock Wallet"));
    changePwBtn = new QPushButton(tr("Change Password"));
    unlockWalletBtn = new QPushButton(tr("Unlock Wallet"));
    unlockForStakingBtn = new QPushButton(tr("Unlock Wallet for Staking Only"));
    timedUnlockBtn = new QPushButton(tr("Timed Unlock"));
    auto *hdiv = new QLabel;
    hdiv->setObjectName("hdiv");
    hdiv->setFixedHeight(1);
    auto *hdivBox = new QFrame;
    auto *hdivBoxLayout = new QVBoxLayout;
    hdivBox->setFixedHeight(BGU::spi(8));
    hdivBox->setLayout(hdivBoxLayout);
    hdivBoxLayout->setContentsMargins(QMargins());
    hdivBoxLayout->addWidget(hdiv, 0, Qt::AlignVCenter);

    layout->addWidget(lockWalletBtn);
    layout->addWidget(changePwBtn);
    layout->addWidget(hdivBox);
    layout->addWidget(unlockWalletBtn);
    layout->addWidget(unlockForStakingBtn);
//    layout->addWidget(timedUnlockBtn);

    setupBtn(lockWalletBtn);
    setupBtn(changePwBtn);
    setupBtn(unlockWalletBtn);
    setupBtn(unlockForStakingBtn);
    setupBtn(timedUnlockBtn);

    connect(lockWalletBtn, &QPushButton::clicked, this, &ReptiloidsCoinLockMenu::onLockWallet);
    connect(changePwBtn, &QPushButton::clicked, this, &ReptiloidsCoinLockMenu::onChangePw);
    connect(unlockWalletBtn, &QPushButton::clicked, this, &ReptiloidsCoinLockMenu::onUnlockWallet);
    connect(unlockForStakingBtn, &QPushButton::clicked, this, &ReptiloidsCoinLockMenu::onUnlockForStaking);
    connect(timedUnlockBtn, &QPushButton::clicked, this, &ReptiloidsCoinLockMenu::onTimedUnlock);
}

void ReptiloidsCoinLockMenu::show() {
    this->raise();
    QWidget::show();
}

void ReptiloidsCoinLockMenu::setDisplayWidget(QWidget *widget) {
    if (!widget)
        return;
    this->setParent(widget);
    displayWidget = widget;
    displayWidget->installEventFilter(this);
}

void ReptiloidsCoinLockMenu::onLockWallet() {
    if (hOnLockWallet)
        hOnLockWallet();
    removeSelf();
}

void ReptiloidsCoinLockMenu::onChangePw() {
    if (hOnChangePw)
        hOnChangePw();
    removeSelf();
}

void ReptiloidsCoinLockMenu::onUnlockWallet() {
    if (hOnUnlockWallet)
        hOnUnlockWallet();
    removeSelf();
}

void ReptiloidsCoinLockMenu::onUnlockForStaking() {
    if (hOnUnlockForStaking)
        hOnUnlockForStaking();
    removeSelf();
}

void ReptiloidsCoinLockMenu::onTimedUnlock() {
    if (hOnTimedUnlock)
        hOnTimedUnlock();
    removeSelf();
}

void ReptiloidsCoinLockMenu::removeSelf(bool kill) {
    if ((!this->underMouse() || kill))
        this->hide();
}

void ReptiloidsCoinLockMenu::setupBtn(QPushButton *btn) {
    btn->setFixedHeight(BGU::spi(40));
    btn->setFlat(true);
    btn->setCursor(Qt::PointingHandCursor);
}

bool ReptiloidsCoinLockMenu::eventFilter(QObject *obj, QEvent *event) {
    if (event->type() == QEvent::MouseButtonPress && !this->isHidden() && !this->underMouse()) {
        removeSelf(false);
    }
    return QObject::eventFilter(obj, event);
}

ReptiloidsCoinLockMenu::~ReptiloidsCoinLockMenu() {
    hOnLockWallet = nullptr;
    hOnChangePw = nullptr;
    hOnUnlockWallet = nullptr;
    hOnUnlockForStaking = nullptr;
    hOnTimedUnlock = nullptr;
    displayWidget->removeEventFilter(this);
}
