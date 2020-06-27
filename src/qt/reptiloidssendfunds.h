// Copyright (c) 2018-2019 The Blocknet developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef REPTILOIDS_QT_REPTILOIDSSENDFUNDS_H
#define REPTILOIDS_QT_REPTILOIDSSENDFUNDS_H

#include <qt/reptiloidsbreadcrumb.h>
#include <qt/reptiloidssendfunds1.h>
#include <qt/reptiloidssendfunds2.h>
#include <qt/reptiloidssendfunds3.h>
#include <qt/reptiloidssendfunds4.h>
#include <qt/reptiloidssendfundsdone.h>
#include <qt/reptiloidssendfundsutil.h>

#include <qt/walletmodel.h>

#include <wallet/coincontrol.h>

#include <QFrame>
#include <QSet>
#include <QVBoxLayout>

class ReptiloidsSendFunds : public QFrame {
    Q_OBJECT
public:
    explicit ReptiloidsSendFunds(WalletModel *w, QFrame *parent = nullptr);
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
    ReptiloidsSendFundsModel *model;
    QVector<ReptiloidsSendFundsPage*> pages;

    QVBoxLayout *layout;
    ReptiloidsSendFunds1 *page1;
    ReptiloidsSendFunds2 *page2;
    ReptiloidsSendFunds3 *page3;
    ReptiloidsSendFunds4 *page4;
    ReptiloidsSendFundsDone *done;
    ReptiloidsBreadCrumb *breadCrumb;
    ReptiloidsSendFundsPage *screen = nullptr;

    void positionCrumb(QPoint pt = QPoint());
    void goToDone();
};

#endif // REPTILOIDS_QT_REPTILOIDSSENDFUNDS_H
