// Copyright (c) 2018-2019 The Blocknet developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <qt/reptiloidscointoolbar.h>

#include <qt/reptiloidscoinguiutil.h>

#include <QApplication>
#include <QProgressBar>
#include <QTimer>

ReptiloidsCoinToolBar::ReptiloidsCoinToolBar(QWidget *popup, QFrame *parent) : QFrame(parent), popupWidget(popup),
                                                                   layout(new QHBoxLayout) {
    layout->setAlignment(Qt::AlignRight);
    layout->setSpacing(BGU::spi(18));
    this->setLayout(layout);

    peersIndicator = new ReptiloidsCoinPeersIndicator;
    peersIndicator->setPeers(BGU::spi(10));

    stakingIndicator = new ReptiloidsCoinStakingIndicator;
    stakingIndicator->setObjectName("staking");

    progressIndicator = new QFrame;
    progressIndicator->setObjectName("progress");
    progressIndicator->setFixedSize(BGU::spi(220), BGU::spi(28));
    progressIndicator->setLayout(new QHBoxLayout);
    progressIndicator->layout()->setContentsMargins(QMargins());
    progressBar = new QProgressBar;
#if defined(Q_OS_WIN)
    progressBar->setProperty("os", "win"); // work-around bug in Qt windows QProgressBar background
#endif
    progressBar->setAlignment(Qt::AlignVCenter);
    progressIndicator->layout()->addWidget(progressBar);

    lockIndicator = new ReptiloidsCoinLockIndicator;
    lockIndicator->setObjectName("lock");

    layout->addStretch(1);
    layout->addWidget(peersIndicator);
    layout->addWidget(stakingIndicator);
    layout->addWidget(progressIndicator);
    layout->addWidget(lockIndicator);

    lockMenu = new ReptiloidsCoinLockMenu;
    lockMenu->setDisplayWidget(popupWidget);
    lockMenu->hOnLockWallet = [&]() { Q_EMIT lock(true); };
    lockMenu->hOnChangePw = [&]() { Q_EMIT passphrase(); };
    lockMenu->hOnUnlockWallet = [&]() { Q_EMIT lock(false); };
    lockMenu->hOnUnlockForStaking = [&]() { Q_EMIT lock(false, true); };
    lockMenu->hOnTimedUnlock = [&]() { /*lockIndicator->setTime();*/ }; // TODO ReptiloidsCoin Qt setTime
    lockMenu->hide();

    connect(lockIndicator, &ReptiloidsCoinLockIndicator::lockRequest, this, &ReptiloidsCoinToolBar::onLockClicked);
    progressIndicator->installEventFilter(this);
}

bool ReptiloidsCoinToolBar::eventFilter(QObject *object, QEvent *event) {
    if (object == progressIndicator && event->type() == QEvent::MouseButtonRelease)
        Q_EMIT progressClicked();
    return QFrame::eventFilter(object, event);
}

QLabel* ReptiloidsCoinToolBar::getIcon(QString path, QString description, QSize size) {
    QPixmap pm(path);
    pm.setDevicePixelRatio(BGU::dpr());
    auto *icon = new QLabel(description);
    icon->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    icon->setFixedSize(size);
    icon->setPixmap(pm.scaled(icon->width()*pm.devicePixelRatio(), icon->height()*pm.devicePixelRatio(),
                              Qt::KeepAspectRatio, Qt::SmoothTransformation));
    return icon;
}

void ReptiloidsCoinToolBar::setPeers(const int peers) {
    peersIndicator->setPeers(peers);
}

void ReptiloidsCoinToolBar::setStaking(const bool on, const QString &msg) {
    stakingIndicator->setOn(on);
    stakingIndicator->setToolTip(msg);
}

void ReptiloidsCoinToolBar::setLock(const bool lock, const bool stakingOnly) {
    lockIndicator->setLock(lock, stakingOnly);
}

void ReptiloidsCoinToolBar::setProgress(const int progress, const QString &msg, const int maximum) {
    progressBar->setMaximum(maximum);
    progressBar->setValue(progress);
    progressBar->setStatusTip(msg);
    progressBar->setToolTip(msg);
    progressBar->setFormat(QString("  %1").arg(msg));
}

void ReptiloidsCoinToolBar::onLockClicked(bool lock) {
    if (lockMenu->isHidden()) {
        QPoint li = lockIndicator->mapToGlobal(QPoint());
        QPoint npos = popupWidget->mapFromGlobal(QPoint(li.x() - lockMenu->width() + 10, li.y() + lockIndicator->height() + 12));
        lockMenu->move(npos);
        lockMenu->show();
    }
}

ReptiloidsCoinPeersIndicator::ReptiloidsCoinPeersIndicator(QFrame *parent) : QFrame(parent), layout(new QHBoxLayout) {
    layout->setContentsMargins(QMargins());
    layout->setSpacing(BGU::spi(4));
    this->setLayout(layout);

    auto *peersIcon = ReptiloidsCoinToolBar::getIcon(QString(":/redesign/UtilityBar/Peers.png"), QString("Peers"), QSize(BGU::spi(21), BGU::spi(20)));
    peersLbl = new QLabel;
    peersLbl->setObjectName("peersLbl");

    layout->addWidget(peersIcon);
    layout->addWidget(peersLbl);
}

void ReptiloidsCoinPeersIndicator::setPeers(const int peers) {
    peersLbl->setText(QString::number(peers));
    this->setToolTip(QString("%1: %2").arg(tr("Connected peers"), QString::number(peers)));
}

ReptiloidsCoinStakingIndicator::ReptiloidsCoinStakingIndicator(QFrame *parent) : QFrame(parent), layout(new QHBoxLayout) {
    layout->setContentsMargins(QMargins());
    layout->setSpacing(0);
    this->setLayout(layout);
    this->setOn(false);
}

void ReptiloidsCoinStakingIndicator::setOn(const bool on) {
    if (stakingIcon != nullptr) {
        layout->removeWidget(stakingIcon);
        stakingIcon->deleteLater();
    }
    QString icon = on ? ":/redesign/UtilityBar/StakingNodeIconActive.png" :
                   ":/redesign/UtilityBar/StakingNodeIconInactive.png";
    stakingIcon = ReptiloidsCoinToolBar::getIcon(icon, tr("Staking"), QSize(BGU::spi(20), BGU::spi(20)));
    layout->addWidget(stakingIcon);
}

/**
 * Manages the lock indicator. If the datetime is set, this icon will set a QTimer to check the time once per second.
 * @param parent
 */
ReptiloidsCoinLockIndicator::ReptiloidsCoinLockIndicator(QPushButton *parent) : QPushButton(parent), layout(new QHBoxLayout) {
    layout->setContentsMargins(QMargins());
    layout->setSpacing(0);
    layout->setAlignment(Qt::AlignVCenter | Qt::AlignHCenter);
    this->setLayout(layout);
    this->setFixedSize(BGU::spi(26), BGU::spi(24));
    this->setCursor(Qt::PointingHandCursor);
    this->setCheckable(true);
    this->setLock(false);
    connect(this, &ReptiloidsCoinLockIndicator::clicked, this, &ReptiloidsCoinLockIndicator::onClick);
}

/**
 * Sets the time. A timer is fired every 1 second. If the time is set to 0 epoch the timer is cleared.
 * @param time
 */
void ReptiloidsCoinLockIndicator::setTime(const QDateTime time) {
    this->lockTime = time;

    // lock if time is in the future
    // TODO ReptiloidsCoin Qt needs work for older qt 5 versions
//    if (time.toSecsSinceEpoch() > QDateTime::currentSecsSinceEpoch())
//        this->setLock(true);
//
//    if (time.toSecsSinceEpoch() == 0) {
//        timer->stop();
//    } else {
//        this->lockTime = time;
//        if (timer == nullptr) {
//            timer = new QTimer;
//            timer->setInterval(1000);
//            timer->setTimerType(Qt::CoarseTimer);
//            connect(timer, &QTimer::timeout, this, &ReptiloidsCointToolBar::tick);
//        }
//        timer->start();
//    }
}

void ReptiloidsCoinLockIndicator::setLock(const bool locked, const bool stakingOnly) {
    this->locked = locked;
    this->stakingOnly = stakingOnly;
    removeLockIcon();

    QString icon = locked ? ":/redesign/UtilityBar/LockedIcon.png" :
                   ":/redesign/UtilityBar/UnlockedIcon.png";
    lockIcon = ReptiloidsCoinToolBar::getIcon(icon, tr("Wallet lock state"), QSize(BGU::spi(20), BGU::spi(16)));
    layout->addWidget(lockIcon);

    if (this->locked)
        this->setToolTip(tr("Wallet is locked"));
    else
        this->setToolTip(this->stakingOnly ? tr("Wallet is unlocked for staking only") : tr("Wallet is unlocked"));
}

void ReptiloidsCoinLockIndicator::tick() {
    // TODO ReptiloidsCoin Qt support older qt versions
//    QDateTime current = QDateTime::currentDateTime();
//    qint64 diff = lockTime.toSecsSinceEpoch() - current.toSecsSinceEpoch();
//    if (diff < 3600 && diff >= 0) { // 1 hr
//        if (lockIcon != nullptr)
//            lockIcon->hide();
//        if (elapsedLbl == nullptr) {
//            elapsedLbl = new QLabel;
//            elapsedLbl->setObjectName("timeLbl");
//            layout->addWidget(elapsedLbl);
//        }
//        qint64 t = lockTime.toSecsSinceEpoch() - current.toSecsSinceEpoch();
//        if (diff < 60) { // seconds
//            elapsedLbl->setText(QString::number(t) + "s");
//        } else { // minutes
//            elapsedLbl->setText(QString::number(ceil((double)t/60)) + "m");
//        }
//    } else if (elapsedLbl != nullptr) {
//        clearTimer();
//    }
}

void ReptiloidsCoinLockIndicator::onClick(bool) {
    if (timer && timer->isActive()) {
        clearTimer();
    }

    // If locked send lock request
    Q_EMIT lockRequest(!this->locked);
}

void ReptiloidsCoinLockIndicator::removeLockIcon() {
    if (lockIcon == nullptr)
        return;
    layout->removeWidget(lockIcon);
    lockIcon->deleteLater();
    lockIcon = nullptr;
}

void ReptiloidsCoinLockIndicator::clearTimer() {
    // TODO ReptiloidsCoin Qt support older qt versions
//    this->lockTime = QDateTime::fromSecsSinceEpoch(0);
//    if (elapsedLbl != nullptr) {
//        if (lockIcon != nullptr)
//            lockIcon->show();
//        layout->removeWidget(elapsedLbl);
//        elapsedLbl->deleteLater();
//        elapsedLbl = nullptr;
//        // now clear the timer
//        timer->stop();
//    }
}
