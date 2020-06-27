// Copyright (c) 2018-2019 The Blocknet developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef REPTILOIDS_QT_REPTILOIDSQUICKSEND_H
#define REPTILOIDS_QT_REPTILOIDSQUICKSEND_H

#include <qt/reptiloidsformbtn.h>
#include <qt/reptiloidslineedit.h>
#include <qt/reptiloidssendfundsutil.h>

#include <qt/walletmodel.h>

#include <QFrame>
#include <QHideEvent>
#include <QLabel>
#include <QList>
#include <QShowEvent>
#include <QVBoxLayout>
#include <QWidget>

class ReptiloidsQuickSend : public QFrame
{
    Q_OBJECT
public:
    explicit ReptiloidsQuickSend(WalletModel *w, QWidget *parent = nullptr);
    bool validated();

Q_SIGNALS:
    void dashboard();
    void submit();

public Q_SLOTS:
    void onSubmit();
    void onCancel() { Q_EMIT dashboard(); }

protected:
    void keyPressEvent(QKeyEvent *event) override;
    void showEvent(QShowEvent *event) override;
    void hideEvent(QHideEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    bool focusNextPrevChild(bool next) override;

private Q_SLOTS:
    void onAmountChanged();
    void onDisplayUnit(int);
    void onEncryptionStatus();
    void openAddressBook();

private:
    WalletModel *walletModel;
    int displayUnit;
    CAmount totalAmount{0};
    CAmount txFees{0};

    QVBoxLayout *layout;
    QLabel *titleLbl;
    ReptiloidsLineEdit *addressTi;
    ReptiloidsLineEdit *amountTi;
    QLabel *feeValueLbl;
    QLabel *totalValueLbl;
    QLabel *warningLbl;
    ReptiloidsFormBtn *cancelBtn;
    ReptiloidsFormBtn *confirmBtn;

    void addAddress(const QString &address);
    void updateLabels(const WalletModel::SendCoinsReturn & result);
    WalletModel::SendCoinsReturn submitFunds();
};

#endif // REPTILOIDS_QT_REPTILOIDSQUICKSEND_H