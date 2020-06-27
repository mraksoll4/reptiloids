// Copyright (c) 2018-2019 The Blocknet developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef REPTILOIDSCOIN_QT_REPTILOIDSCOINCREATEPROPOSAL1_H
#define REPTILOIDSCOIN_QT_REPTILOIDSCOINCREATEPROPOSAL1_H

#include <qt/reptiloidscoincreateproposalutil.h>
#include <qt/reptiloidscoinformbtn.h>
#include <qt/reptiloidscoinlineeditwithtitle.h>

#include <qt/walletmodel.h>

#include <base58.h>
#include <key_io.h>

#include <QFrame>
#include <QLabel>
#include <QVBoxLayout>

class ReptiloidsCoinCreateProposal1 : public ReptiloidsCoinCreateProposalPage {
    Q_OBJECT
public:
    explicit ReptiloidsCoinCreateProposal1(int id, QFrame *parent = nullptr);
    bool validated() override;
    ReptiloidsCoinCreateProposalPageModel getModel() {
        auto proposalAddr = paymentAddrTi->lineEdit->text().toStdString();
        auto proposalDest = DecodeDestination(proposalAddr);
        return {
            proposalTi->lineEdit->text().toStdString(),
            urlTi->lineEdit->text().toStdString(),
            descriptionTi->lineEdit->text().toStdString(),
            superBlockTi->lineEdit->text().toInt(),
            amountTi->lineEdit->text().toInt(),
            proposalDest,
            uint256()
        };
    }

public Q_SLOTS:
    void clear() override;

protected:
    void keyPressEvent(QKeyEvent *event) override;
    void focusInEvent(QFocusEvent *event) override;
    void showEvent(QShowEvent *event) override;
    void inputChanged(const QString &);

private:
    QVBoxLayout *layout;
    QLabel *titleLbl;
    QLabel *feeTitleLbl;
    QLabel *feeLbl;
    QLabel *charCountLbl;
    ReptiloidsCoinLineEditWithTitle *proposalTi;
    ReptiloidsCoinLineEditWithTitle *urlTi;
    ReptiloidsCoinLineEditWithTitle *descriptionTi;
    ReptiloidsCoinLineEditWithTitle *superBlockTi;
    ReptiloidsCoinLineEditWithTitle *amountTi;
    ReptiloidsCoinLineEditWithTitle *paymentAddrTi;
    ReptiloidsCoinFormBtn *continueBtn;
    ReptiloidsCoinFormBtn *cancelBtn;

    static int nextSuperblock();
};

#endif // REPTILOIDSCOIN_QT_REPTILOIDSCOINCREATEPROPOSAL1_H
