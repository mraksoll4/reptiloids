// Copyright (c) 2018-2020 The Blocknet developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef REPTILOIDSCOIN_QT_REPTILOIDSCOINSENDFUNDS3_H
#define REPTILOIDSCOIN_QT_REPTILOIDSCOINSENDFUNDS3_H

#include <qt/reptiloidscoinformbtn.h>
#include <qt/reptiloidscoinlineedit.h>
#include <qt/reptiloidscoinsendfundsutil.h>

#include <qt/walletmodel.h>

#include <wallet/coincontrol.h>

#include <QFrame>
#include <QVBoxLayout>
#include <QLabel>
#include <QRadioButton>
#include <QShowEvent>
#include <QHideEvent>

class ReptiloidsCoinSendFunds3 : public ReptiloidsCoinSendFundsPage {
    Q_OBJECT
protected:
    void keyPressEvent(QKeyEvent *event) override;
    void showEvent(QShowEvent *event) override;
    void hideEvent(QHideEvent *event) override;

public:
    explicit ReptiloidsCoinSendFunds3(WalletModel *w, int id, QFrame *parent = nullptr);
    void setData(ReptiloidsCoinSendFundsModel *model) override;
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
    ReptiloidsCoinLineEdit *specificFeeTi;
    QLabel *specificFeeLbl;
    QLabel *totalFeeLbl;
    ReptiloidsCoinFormBtn *continueBtn;
    ReptiloidsCoinFormBtn *cancelBtn;
    QLabel *transactionFeeDesc;
    QCheckBox *subtractFeeCb;
    QLabel *warningLbl;

    void updateFee();
    void updateModelTxFees(CAmount fee);
};

#endif // REPTILOIDSCOIN_QT_REPTILOIDSCOINSENDFUNDS3_H
