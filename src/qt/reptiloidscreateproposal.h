// Copyright (c) 2018-2019 The Blocknet developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef REPTILOIDS_QT_REPTILOIDSCREATEPROPOSAL_H
#define REPTILOIDS_QT_REPTILOIDSCREATEPROPOSAL_H

#include <qt/reptiloidsbreadcrumb.h>
#include <qt/reptiloidscreateproposalutil.h>
#include <qt/reptiloidscreateproposal1.h>
#include <qt/reptiloidscreateproposal2.h>
#include <qt/reptiloidscreateproposal3.h>

#include <QFrame>
#include <QSet>
#include <QVBoxLayout>
#include <QWidget>

class ReptiloidsCreateProposal : public QFrame
{
    Q_OBJECT

public:
    explicit ReptiloidsCreateProposal(QWidget *parent = nullptr);
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
    QVector<ReptiloidsCreateProposalPage*> pages;
    WalletModel *walletModel = nullptr;

    QVBoxLayout *layout;
    ReptiloidsCreateProposal1 *page1;
    ReptiloidsCreateProposal2 *page2;
    ReptiloidsCreateProposal3 *page3;
    ReptiloidsBreadCrumb *breadCrumb;
    ReptiloidsCreateProposalPage *screen = nullptr;

    void positionCrumb(QPoint pt = QPoint());
    void goToDone();
};

#endif // REPTILOIDS_QT_REPTILOIDSCREATEPROPOSAL_H
