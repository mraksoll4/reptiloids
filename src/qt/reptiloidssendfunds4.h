// Copyright (c) 2018-2020 The Blocknet developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef REPTILOIDS_QT_REPTILOIDSSENDFUNDS4_H
#define REPTILOIDS_QT_REPTILOIDSSENDFUNDS4_H

#include <qt/reptiloidsformbtn.h>
#include <qt/reptiloidssendfundsutil.h>

#include <qt/walletmodel.h>

#include <wallet/coincontrol.h>

#include <QFrame>
#include <QHideEvent>
#include <QLabel>
#include <QScrollArea>
#include <QShowEvent>
#include <QVBoxLayout>

class ReptiloidsSendFunds4 : public ReptiloidsSendFundsPage {
    Q_OBJECT

public:
    explicit ReptiloidsSendFunds4(WalletModel *w, int id, QFrame *parent = nullptr);
    void setData(ReptiloidsSendFundsModel *model) override;
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
    ReptiloidsFormBtn *continueBtn;
    ReptiloidsFormBtn *cancelBtn;
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

#endif // REPTILOIDS_QT_REPTILOIDSSENDFUNDS4_H
