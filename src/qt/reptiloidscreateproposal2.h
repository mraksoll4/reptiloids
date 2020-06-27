// Copyright (c) 2018-2020 The Blocknet developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef REPTILOIDSCOIN_QT_REPTILOIDSCOINCREATEPROPOSAL2_H
#define REPTILOIDSCOIN_QT_REPTILOIDSCOINCREATEPROPOSAL2_H

#include <qt/reptiloidscoincreateproposalutil.h>
#include <qt/reptiloidscoinformbtn.h>

#include <qt/walletmodel.h>

#include <QFrame>
#include <QLabel>
#include <QVBoxLayout>

class ReptiloidsCoinCreateProposal2 : public ReptiloidsCoinCreateProposalPage {
    Q_OBJECT
public:
    explicit ReptiloidsCoinCreateProposal2(int id, QFrame *parent = nullptr);
    void setModel(const ReptiloidsCoinCreateProposalPageModel &model);
    void clear() override;
    bool validated() override;
    ReptiloidsCoinCreateProposalPageModel getModel() {
        return model;
    }

protected:
    void keyPressEvent(QKeyEvent *event) override;

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
    QLabel *proposalAddrLbl;
    QLabel *proposalAddrValLbl;
    QLabel *urlLbl;
    QLabel *urlValLbl;
    QLabel *feeTitleLbl;
    QLabel *feeLbl;
    QLabel *descLbl;
    QLabel *descValLbl;
    ReptiloidsCoinFormBtn *backBtn;
    ReptiloidsCoinFormBtn *submitBtn;
    ReptiloidsCoinFormBtn *cancelBtn;
    ReptiloidsCoinCreateProposalPageModel model;

    void disableButtons(const bool &disable);
};

#endif // REPTILOIDSCOIN_QT_REPTILOIDSCOINCREATEPROPOSAL2_H
