// Copyright (c) 2018-2019 The Blocknet developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef REPTILOIDS_QT_REPTILOIDSSENDFUNDS1_H
#define REPTILOIDS_QT_REPTILOIDSSENDFUNDS1_H

#include <qt/reptiloidsaddresseditor.h>
#include <qt/reptiloidsformbtn.h>
#include <qt/reptiloidssendfundsutil.h>

#include <qt/walletmodel.h>

#include <QFrame>
#include <QLabel>
#include <QVBoxLayout>

class ReptiloidsSendFunds1 : public ReptiloidsSendFundsPage {
    Q_OBJECT
protected:
    void keyPressEvent(QKeyEvent *event) override;
    void focusInEvent(QFocusEvent *event) override;

public:
    explicit ReptiloidsSendFunds1(WalletModel *w, int id, QFrame *parent = nullptr);
    void setData(ReptiloidsSendFundsModel *model) override;
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
    ReptiloidsAddressEditor *addressTi;
    ReptiloidsFormBtn *continueBtn;
};

#endif // REPTILOIDS_QT_REPTILOIDSSENDFUNDS1_H
