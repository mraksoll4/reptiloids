// Copyright (c) 2018-2019 The Blocknet developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef REPTILOIDSCOIN_QT_REPTILOIDSCOINSENDFUNDS_H
#define REPTILOIDSCOIN_QT_REPTILOIDSCOINSENDFUNDS_H

#include <qt/reptiloidscoinbreadcrumb.h>
#include <qt/reptiloidscoinsendfunds1.h>
#include <qt/reptiloidscoinsendfunds2.h>
#include <qt/reptiloidscoinsendfunds3.h>
#include <qt/reptiloidscoinsendfunds4.h>
#include <qt/reptiloidscoinsendfundsdone.h>
#include <qt/reptiloidscoinsendfundsutil.h>

#include <qt/walletmodel.h>

#include <wallet/coincontrol.h>

#include <QFrame>
#include <QSet>
#include <QVBoxLayout>

class ReptiloidsCoinSendFunds : public QFrame {
    Q_OBJECT
public:
    explicit ReptiloidsCoinSendFunds(WalletModel *w, QFrame *parent = nullptr);
    void setWalletModel(WalletModel *w) {
        walletModel = w;
        page1->setWalletModel(walletModel);
        page2->setWalletModel(walletModel);
        page3->setWalletModel(walletModel);
        page4->setWalletModel(walletModel);
    }
    void clear() {
        this->model->reset();
        page1->clear();
        page2->clear();
        page3->clear();
        page4->clear();
    }
    void addAddress(const QString &address);

Q_SIGNALS:
    void sent();
    void dashboard();

public Q_SLOTS:

protected:
    bool event(QEvent *event) override;

    void focusInEvent(QFocusEvent *event) override;

private Q_SLOTS:
    void crumbChanged(int crumb);
    void nextCrumb(int crumb);
    void prevCrumb(int crumb);
    void onCancel(int crumb);
    void onEdit();
    void onSendFunds();
    void goToDashboard() { Q_EMIT dashboard(); };
    void onDoneDashboard();
    void reset();

private:
    WalletModel *walletModel;
    ReptiloidsCoinSendFundsModel *model;
    QVector<ReptiloidsCoinSendFundsPage*> pages;

    QVBoxLayout *layout;
    ReptiloidsCoinSendFunds1 *page1;
    ReptiloidsCoinSendFunds2 *page2;
    ReptiloidsCoinSendFunds3 *page3;
    ReptiloidsCoinSendFunds4 *page4;
    ReptiloidsCoinSendFundsDone *done;
    ReptiloidsCoinBreadCrumb *breadCrumb;
    ReptiloidsCoinSendFundsPage *screen = nullptr;

    void positionCrumb(QPoint pt = QPoint());
    void goToDone();
};

#endif // REPTILOIDSCOIN_QT_REPTILOIDSCOINSENDFUNDS_H
