// Copyright (c) 2018-2019 The Blocknet developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <qt/reptiloidslockmenu.h>

#include <qt/reptiloidsguiutil.h>

#include <QLabel>
#include <QEvent>
#include <QDebug>

ReptiloidsLockMenu::ReptiloidsLockMenu(QFrame *parent) : QFrame(parent), layout(new QVBoxLayout) {
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

    connect(lockWalletBtn, &QPushButton::clicked, this, &ReptiloidsLockMenu::onLockWallet);
    connect(changePwBtn, &QPushButton::clicked, this, &ReptiloidsLockMenu::onChangePw);
    connect(unlockWalletBtn, &QPushButton::clicked, this, &ReptiloidsLockMenu::onUnlockWallet);
    connect(unlockForStakingBtn, &QPushButton::clicked, this, &ReptiloidsLockMenu::onUnlockForStaking);
    connect(timedUnlockBtn, &QPushButton::clicked, this, &ReptiloidsLockMenu::onTimedUnlock);
}

void ReptiloidsLockMenu::show() {
    this->raise();
    QWidget::show();
}

void ReptiloidsLockMenu::setDisplayWidget(QWidget *widget) {
    if (!widget)
        return;
    this->setParent(widget);
    displayWidget = widget;
    displayWidget->installEventFilter(this);
}

void ReptiloidsLockMenu::onLockWallet() {
    if (hOnLockWallet)
        hOnLockWallet();
    removeSelf();
}

void ReptiloidsLockMenu::onChangePw() {
    if (hOnChangePw)
        hOnChangePw();
    removeSelf();
}

void ReptiloidsLockMenu::onUnlockWallet() {
    if (hOnUnlockWallet)
        hOnUnlockWallet();
    removeSelf();
}

void ReptiloidsLockMenu::onUnlockForStaking() {
    if (hOnUnlockForStaking)
        hOnUnlockForStaking();
    removeSelf();
}

void ReptiloidsLockMenu::onTimedUnlock() {
    if (hOnTimedUnlock)
        hOnTimedUnlock();
    removeSelf();
}

void ReptiloidsLockMenu::removeSelf(bool kill) {
    if ((!this->underMouse() || kill))
        this->hide();
}

void ReptiloidsLockMenu::setupBtn(QPushButton *btn) {
    btn->setFixedHeight(BGU::spi(40));
    btn->setFlat(true);
    btn->setCursor(Qt::PointingHandCursor);
}

bool ReptiloidsLockMenu::eventFilter(QObject *obj, QEvent *event) {
    if (event->type() == QEvent::MouseButtonPress && !this->isHidden() && !this->underMouse()) {
        removeSelf(false);
    }
    return QObject::eventFilter(obj, event);
}

ReptiloidsLockMenu::~ReptiloidsLockMenu() {
    hOnLockWallet = nullptr;
    hOnChangePw = nullptr;
    hOnUnlockWallet = nullptr;
    hOnUnlockForStaking = nullptr;
    hOnTimedUnlock = nullptr;
    displayWidget->removeEventFilter(this);
}
