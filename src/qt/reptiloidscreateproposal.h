// Copyright (c) 2018-2019 The Blocknet developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef REPTILOIDSCOIN_QT_REPTILOIDSCOINCREATEPROPOSAL_H
#define REPTILOIDSCOIN_QT_REPTILOIDSCOINCREATEPROPOSAL_H

#include <qt/reptiloidscoinbreadcrumb.h>
#include <qt/reptiloidscoincreateproposalutil.h>
#include <qt/reptiloidscoincreateproposal1.h>
#include <qt/reptiloidscoincreateproposal2.h>
#include <qt/reptiloidscoincreateproposal3.h>

#include <QFrame>
#include <QSet>
#include <QVBoxLayout>
#include <QWidget>

class ReptiloidsCoinCreateProposal : public QFrame
{
    Q_OBJECT

public:
    explicit ReptiloidsCoinCreateProposal(QWidget *parent = nullptr);
    void setWalletModel(WalletModel *w) { walletModel = w; }
    void clear() {
        page1->clear();
        page2->clear();
        page3->clear();
    }

Q_SIGNALS:
    void done();

protected:
    bool event(QEvent *event) override;
    void focusInEvent(QFocusEvent *event) override;
    void showEvent(QShowEvent *event) override;

private Q_SLOTS:
    void crumbChanged(int crumb);
    void nextCrumb(int crumb);
    void prevCrumb(int crumb);
    void onCancel(int crumb);
    void onDone();
    void reset();

private:
    QVector<ReptiloidsCoinCreateProposalPage*> pages;
    WalletModel *walletModel = nullptr;

    QVBoxLayout *layout;
    ReptiloidsCoinCreateProposal1 *page1;
    ReptiloidsCoinCreateProposal2 *page2;
    ReptiloidsCoinCreateProposal3 *page3;
    ReptiloidsCoinBreadCrumb *breadCrumb;
    ReptiloidsCoinCreateProposalPage *screen = nullptr;

    void positionCrumb(QPoint pt = QPoint());
    void goToDone();
};

#endif // REPTILOIDSCOIN_QT_REPTILOIDSCOINCREATEPROPOSAL_H
