// Copyright (c) 2018-2019 The Blocknet developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef REPTILOIDS_QT_REPTILOIDSCREATEPROPOSAL1_H
#define REPTILOIDS_QT_REPTILOIDSCREATEPROPOSAL1_H

#include <qt/reptiloidscreateproposalutil.h>
#include <qt/reptiloidsformbtn.h>
#include <qt/reptiloidslineeditwithtitle.h>

#include <qt/walletmodel.h>

#include <base58.h>
#include <key_io.h>

#include <QFrame>
#include <QLabel>
#include <QVBoxLayout>

class ReptiloidsCreateProposal1 : public ReptiloidsCreateProposalPage {
    Q_OBJECT
public:
    explicit ReptiloidsCreateProposal1(int id, QFrame *parent = nullptr);
    bool validated() override;
    ReptiloidsCreateProposalPageModel getModel() {
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
    ReptiloidsLineEditWithTitle *proposalTi;
    ReptiloidsLineEditWithTitle *urlTi;
    ReptiloidsLineEditWithTitle *descriptionTi;
    ReptiloidsLineEditWithTitle *superBlockTi;
    ReptiloidsLineEditWithTitle *amountTi;
    ReptiloidsLineEditWithTitle *paymentAddrTi;
    ReptiloidsFormBtn *continueBtn;
    ReptiloidsFormBtn *cancelBtn;

    static int nextSuperblock();
};

#endif // REPTILOIDS_QT_REPTILOIDSCREATEPROPOSAL1_H
