// Copyright (c) 2018-2020 The Blocknet developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef REPTILOIDS_QT_REPTILOIDSCREATEPROPOSAL2_H
#define REPTILOIDS_QT_REPTILOIDSCREATEPROPOSAL2_H

#include <qt/reptiloidscreateproposalutil.h>
#include <qt/reptiloidsformbtn.h>

#include <qt/walletmodel.h>

#include <QFrame>
#include <QLabel>
#include <QVBoxLayout>

class ReptiloidsCreateProposal2 : public ReptiloidsCreateProposalPage {
    Q_OBJECT
public:
    explicit ReptiloidsCreateProposal2(int id, QFrame *parent = nullptr);
    void setModel(const ReptiloidsCreateProposalPageModel &model);
    void clear() override;
    bool validated() override;
    ReptiloidsCreateProposalPageModel getModel() {
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
    ReptiloidsFormBtn *backBtn;
    ReptiloidsFormBtn *submitBtn;
    ReptiloidsFormBtn *cancelBtn;
    ReptiloidsCreateProposalPageModel model;

    void disableButtons(const bool &disable);
};

#endif // REPTILOIDS_QT_REPTILOIDSCREATEPROPOSAL2_H
