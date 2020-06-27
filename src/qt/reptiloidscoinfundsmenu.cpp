// Copyright (c) 2018-2019 The Blocknet developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <qt/reptiloidscoinfundsmenu.h>

#include <qt/reptiloidscoinguiutil.h>

#include <QEvent>
#include <QLabel>

ReptiloidsCoinFundsMenu::ReptiloidsCoinFundsMenu(QFrame *parent) : QFrame(parent), layout(new QVBoxLayout) {
    layout->setContentsMargins(QMargins());
    layout->setSpacing(0);
    this->setMaximumSize(BGU::spi(160), BGU::spi(100));
    this->setLayout(layout);

    sendFundsBtn = new QPushButton(tr("Send Funds"));
    requestFundsBtn = new QPushButton(tr("Request Funds"));

    layout->addWidget(sendFundsBtn);
    layout->addWidget(requestFundsBtn);

    setupBtn(sendFundsBtn);
    setupBtn(requestFundsBtn);

//    connect(sendFundsBtn, &QPushButton::clicked, this, &ReptiloidsCoinFundsMenu::onLockWallet);
//    connect(requestFundsBtn, &QPushButton::clicked, this, &ReptiloidsCoinFundsMenu::onChangePw);
}

void ReptiloidsCoinFundsMenu::show() {
    this->raise();
    QWidget::show();
}

void ReptiloidsCoinFundsMenu::setDisplayWidget(QWidget *widget) {
    if (!widget)
        return;
    this->setParent(widget);
    displayWidget = widget;
    displayWidget->installEventFilter(this);
}

void ReptiloidsCoinFundsMenu::onSendFunds() {
    if (hOnSendFunds)
        hOnSendFunds();
    removeSelf();
}

void ReptiloidsCoinFundsMenu::onRequestFunds() {
    if (hOnRequestFunds)
        hOnRequestFunds();
    removeSelf();
}

void ReptiloidsCoinFundsMenu::removeSelf(bool kill) {
    if ((!this->underMouse() || kill))
        this->hide();
}

void ReptiloidsCoinFundsMenu::setupBtn(QPushButton *btn) {
    btn->setFixedHeight(BGU::spi(40));
    btn->setFlat(true);
    btn->setCursor(Qt::PointingHandCursor);
}

bool ReptiloidsCoinFundsMenu::eventFilter(QObject *obj, QEvent *event) {
    if (event->type() == QEvent::MouseButtonPress && !this->isHidden() && !this->underMouse()) {
        removeSelf(false);
    }
    return QObject::eventFilter(obj, event);
}

ReptiloidsCoinFundsMenu::~ReptiloidsCoinFundsMenu() {
    hOnSendFunds = nullptr;
    hOnRequestFunds = nullptr;
    displayWidget->removeEventFilter(this);
}
