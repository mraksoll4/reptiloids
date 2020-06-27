// Copyright (c) 2018-2020 The Blocknet developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef REPTILOIDSCOIN_QT_REPTILOIDSCOINSENDFUNDS4_H
#define REPTILOIDSCOIN_QT_REPTILOIDSCOINSENDFUNDS4_H

#include <qt/reptiloidscoinformbtn.h>
#include <qt/reptiloidscoinsendfundsutil.h>

#include <qt/walletmodel.h>

#include <wallet/coincontrol.h>

#include <QFrame>
#include <QHideEvent>
#include <QLabel>
#include <QScrollArea>
#include <QShowEvent>
#include <QVBoxLayout>

class ReptiloidsCoinSendFunds4 : public ReptiloidsCoinSendFundsPage {
    Q_OBJECT

public:
    explicit ReptiloidsCoinSendFunds4(WalletModel *w, int id, QFrame *parent = nullptr);
    void setData(ReptiloidsCoinSendFundsModel *model) override;
    bool validated() override;
    void clear() override;

Q_SIGNALS:
    void edit();
    void submit();

protected:
    void keyPressEvent(QKeyEvent *event) override;
    void showEvent(QShowEvent *event) override;
    void hideEvent(QHideEvent *event) override;

private Q_SLOTS:
    void onEdit();
    void onSubmit();
    void onDisplayUnit(int unit);
    void onEncryptionStatus();

private:
    int displayUnit;

    QVBoxLayout *layout;
    QLabel *titleLbl;
    ReptiloidsCoinFormBtn *continueBtn;
    ReptiloidsCoinFormBtn *cancelBtn;
    QFrame *content;
    QVBoxLayout *contentLayout;
    QFrame *recipients = nullptr;
    QScrollArea *scrollArea = nullptr;
    QLabel *feeValueLbl = nullptr;
    QLabel *totalValueLbl = nullptr;
    QLabel *warningLbl = nullptr;

    void fillWalletData();
    void displayMultiple();
    void clearRecipients();
    QString feeText(QString fee);
    QString totalText(QString total);
};

#endif // REPTILOIDSCOIN_QT_REPTILOIDSCOINSENDFUNDS4_H
