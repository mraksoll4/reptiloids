// Copyright (c) 2018-2019 The Blocknet developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef REPTILOIDSCOIN_QT_REPTILOIDSCOINSENDFUNDS1_H
#define REPTILOIDSCOIN_QT_REPTILOIDSCOINSENDFUNDS1_H

#include <qt/reptiloidscoinaddresseditor.h>
#include <qt/reptiloidscoinformbtn.h>
#include <qt/reptiloidscoinsendfundsutil.h>

#include <qt/walletmodel.h>

#include <QFrame>
#include <QLabel>
#include <QVBoxLayout>

class ReptiloidsCoinSendFunds1 : public ReptiloidsCoinSendFundsPage {
    Q_OBJECT
protected:
    void keyPressEvent(QKeyEvent *event) override;
    void focusInEvent(QFocusEvent *event) override;

public:
    explicit ReptiloidsCoinSendFunds1(WalletModel *w, int id, QFrame *parent = nullptr);
    void setData(ReptiloidsCoinSendFundsModel *model) override;
    bool validated() override;
    void addAddress(const QString &address);

public Q_SLOTS:
    void clear() override;
    void textChanged();
    void onAddressesChanged();
    void openAddressBook();

private:
    QVBoxLayout *layout;
    QLabel *titleLbl;
    ReptiloidsCoinAddressEditor *addressTi;
    ReptiloidsCoinFormBtn *continueBtn;
};

#endif // REPTILOIDSCOIN_QT_REPTILOIDSCOINSENDFUNDS1_H
