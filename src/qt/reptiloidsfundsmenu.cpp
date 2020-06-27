// Copyright (c) 2018-2019 The Blocknet developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <qt/reptiloidsfundsmenu.h>

#include <qt/reptiloidsguiutil.h>

#include <QEvent>
#include <QLabel>

ReptiloidsFundsMenu::ReptiloidsFundsMenu(QFrame *parent) : QFrame(parent), layout(new QVBoxLayout) {
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

//    connect(sendFundsBtn, &QPushButton::clicked, this, &ReptiloidsFundsMenu::onLockWallet);
//    connect(requestFundsBtn, &QPushButton::clicked, this, &ReptiloidsFundsMenu::onChangePw);
}

void ReptiloidsFundsMenu::show() {
    this->raise();
    QWidget::show();
}

void ReptiloidsFundsMenu::setDisplayWidget(QWidget *widget) {
    if (!widget)
        return;
    this->setParent(widget);
    displayWidget = widget;
    displayWidget->installEventFilter(this);
}

void ReptiloidsFundsMenu::onSendFunds() {
    if (hOnSendFunds)
        hOnSendFunds();
    removeSelf();
}

void ReptiloidsFundsMenu::onRequestFunds() {
    if (hOnRequestFunds)
        hOnRequestFunds();
    removeSelf();
}

void ReptiloidsFundsMenu::removeSelf(bool kill) {
    if ((!this->underMouse() || kill))
        this->hide();
}

void ReptiloidsFundsMenu::setupBtn(QPushButton *btn) {
    btn->setFixedHeight(BGU::spi(40));
    btn->setFlat(true);
    btn->setCursor(Qt::PointingHandCursor);
}

bool ReptiloidsFundsMenu::eventFilter(QObject *obj, QEvent *event) {
    if (event->type() == QEvent::MouseButtonPress && !this->isHidden() && !this->underMouse()) {
        removeSelf(false);
    }
    return QObject::eventFilter(obj, event);
}

ReptiloidsFundsMenu::~ReptiloidsFundsMenu() {
    hOnSendFunds = nullptr;
    hOnRequestFunds = nullptr;
    displayWidget->removeEventFilter(this);
}
