// Copyright (c) 2018-2020 The Blocknet developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef REPTILOIDS_QT_REPTILOIDSSENDFUNDS3_H
#define REPTILOIDS_QT_REPTILOIDSSENDFUNDS3_H

#include <qt/reptiloidsformbtn.h>
#include <qt/reptiloidslineedit.h>
#include <qt/reptiloidssendfundsutil.h>

#include <qt/walletmodel.h>

#include <wallet/coincontrol.h>

#include <QFrame>
#include <QVBoxLayout>
#include <QLabel>
#include <QRadioButton>
#include <QShowEvent>
#include <QHideEvent>

class ReptiloidsSendFunds3 : public ReptiloidsSendFundsPage {
    Q_OBJECT
protected:
    void keyPressEvent(QKeyEvent *event) override;
    void showEvent(QShowEvent *event) override;
    void hideEvent(QHideEvent *event) override;

public:
    explicit ReptiloidsSendFunds3(WalletModel *w, int id, QFrame *parent = nullptr);
    void setData(ReptiloidsSendFundsModel *model) override;
    bool validated() override;
    void clear() override;

Q_SIGNALS:
    void feeOption(double fee);

private Q_SLOTS:
    void onFeeDesignation();
    void onSpecificFee();
    void onEncryptionStatus();
    void onSubtractFee();
    void onDisplayUnit(int unit);

private:
    int displayUnit;

    QVBoxLayout *layout;
    QLabel *titleLbl;
    QLabel *recommendedDescLbl;
    QRadioButton *recommendedRb;
    QRadioButton *specificRb;
    ReptiloidsLineEdit *specificFeeTi;
    QLabel *specificFeeLbl;
    QLabel *totalFeeLbl;
    ReptiloidsFormBtn *continueBtn;
    ReptiloidsFormBtn *cancelBtn;
    QLabel *transactionFeeDesc;
    QCheckBox *subtractFeeCb;
    QLabel *warningLbl;

    void updateFee();
    void updateModelTxFees(CAmount fee);
};

#endif // REPTILOIDS_QT_REPTILOIDSSENDFUNDS3_H
