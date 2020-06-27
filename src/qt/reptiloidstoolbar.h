// Copyright (c) 2018-2019 The Blocknet developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef REPTILOIDS_QT_REPTILOIDSTOOLBAR_H
#define REPTILOIDS_QT_REPTILOIDSTOOLBAR_H

#include <qt/reptiloidslockmenu.h>

#include <QBoxLayout>
#include <QDateTime>
#include <QFrame>
#include <QHBoxLayout>
#include <QLabel>
#include <QProgressBar>
#include <QPushButton>

class ReptiloidsPeersIndicator : public QFrame
{
Q_OBJECT
public:
    explicit ReptiloidsPeersIndicator(QFrame *parent = nullptr);
    void setPeers(int peers);

Q_SIGNALS:

public Q_SLOTS:

private:
    QHBoxLayout *layout;
    QLabel *peersLbl;
};

class ReptiloidsStakingIndicator : public QFrame
{
Q_OBJECT
public:
    explicit ReptiloidsStakingIndicator(QFrame *parent = nullptr);
    void setOn(bool on);

Q_SIGNALS:

public Q_SLOTS:

private:
    QHBoxLayout *layout;
    QLabel *stakingIcon = nullptr;
};

class ReptiloidsLockIndicator : public QPushButton
{
Q_OBJECT
public:
    explicit ReptiloidsLockIndicator(QPushButton *parent = nullptr);
    void setTime(QDateTime time);
    void setLock(bool locked, bool stakingOnly = false);

Q_SIGNALS:
    void lockRequest(bool wantLock);

private Q_SLOTS:
    void tick();
    void onClick(bool checked);

private:
    bool locked;
    bool stakingOnly;
    QHBoxLayout *layout;
    QLabel *lockIcon = nullptr;
    QLabel *elapsedLbl = nullptr;
    QDateTime lockTime;
    QTimer *timer = nullptr;

    void removeLockIcon();
    void clearTimer();
};

class ReptiloidsToolBar : public QFrame
{
Q_OBJECT
public:
    explicit ReptiloidsToolBar(QWidget *popups, QFrame *parent = nullptr);
    void setPeers(int peers);
    void setStaking(bool on, const QString &msg);
    void setLock(bool lock, bool stakingOnly);
    void setProgress(int progress, const QString &msg, int maximum);
    static QLabel* getIcon(QString path, QString description, QSize size);

Q_SIGNALS:
    void lock(bool locked, bool stakingOnly = false);
    void passphrase();
    void progressClicked();

public Q_SLOTS:
    void onLockClicked(bool lock);

protected:
    bool eventFilter(QObject *object, QEvent *event);

private:
    QHBoxLayout *layout;
    ReptiloidsPeersIndicator *peersIndicator;
    ReptiloidsStakingIndicator *stakingIndicator;
    QFrame *progressIndicator;
    QProgressBar *progressBar;
    ReptiloidsLockIndicator *lockIndicator;
    ReptiloidsLockMenu *lockMenu = nullptr;
    QWidget *popupWidget;
};

#endif // REPTILOIDS_QT_REPTILOIDSTOOLBAR_H
