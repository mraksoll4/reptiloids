// Copyright (c) 2018-2020 The Blocknet developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef REPTILOIDS_QT_REPTILOIDSCREATEPROPOSAL3_H
#define REPTILOIDS_QT_REPTILOIDSCREATEPROPOSAL3_H

#include <qt/reptiloidscreateproposalutil.h>
#include <qt/reptiloidsformbtn.h>

#include <qt/walletmodel.h>

#include <QFrame>
#include <QLabel>
#include <QTimer>
#include <QVBoxLayout>

class ReptiloidsCreateProposal3 : public ReptiloidsCreateProposalPage {
    Q_OBJECT
public:
    explicit ReptiloidsCreateProposal3(int id, QFrame *parent = nullptr);
    void setModel(const ReptiloidsCreateProposalPageModel &model);
    void clear() override;
    bool validated() override;

protected:
    void keyPressEvent(QKeyEvent *event) override;

Q_SIGNALS:
    void done();

public Q_SLOTS:
    void onCancel() override;

private Q_SLOTS:
    void onSubmit();

private:
    QVBoxLayout *layout;
    QLabel *titleLbl;
    QLabel *subtitleLbl;
    QLabel *proposalTitleLbl;
    QLabel *proposalLbl;
    QLabel *proposalDetailTitleLbl;
    QLabel *proposalDetailLbl;
    QLabel *feeHashLbl;
    QLabel *feeHashValLbl;
    QLabel *feeTitleLbl;
    QLabel *feeLbl;
    ReptiloidsFormBtn *doneBtn;
    ReptiloidsCreateProposalPageModel model;
    QTimer *timer;

    int collateralConfirmations();
};

#endif // REPTILOIDS_QT_REPTILOIDSCREATEPROPOSAL3_H
