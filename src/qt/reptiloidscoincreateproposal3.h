// Copyright (c) 2018-2020 The Blocknet developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef REPTILOIDSCOIN_QT_REPTILOIDSCOINCREATEPROPOSAL3_H
#define REPTILOIDSCOIN_QT_REPTILOIDSCOINCREATEPROPOSAL3_H

#include <qt/reptiloidscoincreateproposalutil.h>
#include <qt/reptiloidscoinformbtn.h>

#include <qt/walletmodel.h>

#include <QFrame>
#include <QLabel>
#include <QTimer>
#include <QVBoxLayout>

class ReptiloidsCoinCreateProposal3 : public ReptiloidsCoinCreateProposalPage {
    Q_OBJECT
public:
    explicit ReptiloidsCoinCreateProposal3(int id, QFrame *parent = nullptr);
    void setModel(const ReptiloidsCoinCreateProposalPageModel &model);
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
    ReptiloidsCoinFormBtn *doneBtn;
    ReptiloidsCoinCreateProposalPageModel model;
    QTimer *timer;

    int collateralConfirmations();
};

#endif // REPTILOIDSCOIN_QT_REPTILOIDSCOINCREATEPROPOSAL3_H
