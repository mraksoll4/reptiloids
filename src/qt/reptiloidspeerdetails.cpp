// Copyright (c) 2018-2019 The Blocknet developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <qt/reptiloidspeerdetails.h>

#include <qt/reptiloidsguiutil.h>

ReptiloidsPeerDetails::ReptiloidsPeerDetails(QFrame *parent) : QFrame(parent), layout(new QVBoxLayout),
                                                           detailsLayout(new QGridLayout), detailsFrame(new QFrame)
{
    this->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Preferred);
    layout->setContentsMargins(BGU::spi(30), BGU::spi(30), BGU::spi(30), BGU::spi(30));
    this->setLayout(layout);

    titleLbl = new QLabel(tr("Selected Peer Details"));
    titleLbl->setObjectName("h4");
    titleLbl->setFixedHeight(BGU::spi(26));

    detailsLayout->setContentsMargins(QMargins());
    detailsFrame->setLayout(detailsLayout);

    layout->addWidget(titleLbl);
    layout->addWidget(detailsFrame);
}

void ReptiloidsPeerDetails::show(PeerDetails details) {
    directionLbl = new QLabel(tr("Direction: ") + details.direction);
    directionLbl->setObjectName("detailLbl");
    detailsLayout->addWidget(directionLbl, 0, 0, Qt::AlignLeft);
    protocolLbl = new QLabel(tr("Protocol: ") + details.protocol);
    protocolLbl->setObjectName("detailLbl");
    detailsLayout->addWidget(protocolLbl, 1, 0, Qt::AlignLeft);
    versionLbl = new QLabel(tr("Version: ") + details.version);
    versionLbl->setObjectName("detailLbl");
    detailsLayout->addWidget(versionLbl, 2, 0, Qt::AlignLeft);
    startHeightLbl = new QLabel(tr("Starting Height: ") + details.startingHeight);
    startHeightLbl->setObjectName("detailLbl");
    detailsLayout->addWidget(startHeightLbl, 3, 0, Qt::AlignLeft);
    syncHeightLbl = new QLabel(tr("Sync Height: ") + details.syncHeight);
    syncHeightLbl->setObjectName("detailLbl");
    detailsLayout->addWidget(syncHeightLbl, 4, 0, Qt::AlignLeft);
    banScoreLbl = new QLabel(tr("Ban Score: ") + details.banScore);
    banScoreLbl->setObjectName("detailLbl");
    detailsLayout->addWidget(banScoreLbl, 0, 1, Qt::AlignLeft);
    connTimeLbl = new QLabel(tr("Connection Time: ") + details.connectionTime);
    connTimeLbl->setObjectName("detailLbl");
    detailsLayout->addWidget(connTimeLbl, 1, 1, Qt::AlignLeft);
    lastSendLbl = new QLabel(tr("Last Send: ") + details.lastSend);
    lastSendLbl->setObjectName("detailLbl");
    detailsLayout->addWidget(lastSendLbl, 2, 1, Qt::AlignLeft);
    lastReceiveLbl = new QLabel(tr("Last Receive: ") + details.lastReceive);
    lastReceiveLbl->setObjectName("detailLbl");
    detailsLayout->addWidget(lastReceiveLbl, 3, 1, Qt::AlignLeft);
    bytesSentLbl = new QLabel(tr("Bytes Sent: ") + details.bytesSent);
    bytesSentLbl->setObjectName("detailLbl");
    detailsLayout->addWidget(bytesSentLbl, 4, 1, Qt::AlignLeft);
    bytesReceivedLbl = new QLabel(tr("Bytes Received: ") + details.bytesReceived);
    bytesReceivedLbl->setObjectName("detailLbl");
    detailsLayout->addWidget(bytesReceivedLbl, 0, 2, Qt::AlignLeft);
    pingTimeLbl = new QLabel(tr("Ping Time: ") + details.pingTime);
    pingTimeLbl->setObjectName("detailLbl");
    detailsLayout->addWidget(pingTimeLbl, 1, 2, Qt::AlignLeft);

    this->raise();
    QWidget::show();
}

void ReptiloidsPeerDetails::setDisplayWidget(QWidget *widget) {
    if (!widget)
        return;
    this->setParent(widget);
    displayWidget = widget;
    displayWidget->installEventFilter(this);
}

void ReptiloidsPeerDetails::removeSelf(bool kill) {
    if ((!this->underMouse() || kill))
        this->hide();
}

bool ReptiloidsPeerDetails::eventFilter(QObject *obj, QEvent *event) {
    if (event->type() == QEvent::MouseButtonPress && !this->isHidden() && !this->underMouse()) {
        removeSelf(false);
    }
    return QObject::eventFilter(obj, event);
}

ReptiloidsPeerDetails::~ReptiloidsPeerDetails() {
    displayWidget->removeEventFilter(this);
}