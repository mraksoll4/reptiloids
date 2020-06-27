// Copyright (c) 2018-2019 The Blocknet developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef REPTILOIDSCOIN_QT_REPTILOIDSCOINADDRESSEDIT_H
#define REPTILOIDSCOIN_QT_REPTILOIDSCOINADDRESSEDIT_H

#include <qt/reptiloidscoinformbtn.h>
#include <qt/reptiloidscoinlineeditwithtitle.h>

#include <qt/walletmodel.h>
#include <qt/addresstablemodel.h>

#include <base58.h>

#include <QDialog>
#include <QFrame>
#include <QLabel>
#include <QRadioButton>
#include <QVBoxLayout>
#include <QWidget>

class ReptiloidsCoinAddressEdit : public QFrame {
    Q_OBJECT
public:
    explicit ReptiloidsCoinAddressEdit(bool editMode, const QString &title, const QString &buttonString, QWidget *parent = nullptr);
    QSize sizeHint() const override;
    bool validated();
    bool setNewAddress(const CTxDestination & dest);
    void setData(const QString &address, const QString &alias, const int &type, const QString &key);
    QString getAddress();
    QString getAlias();
    QString getKey();
    QString getType();

Q_SIGNALS:
    void cancel();
    void accept();

public Q_SLOTS:
    void clear();
    void onApply();
    void onCancel() { Q_EMIT cancel(); }

protected:
    void keyPressEvent(QKeyEvent *event) override;
    void focusInEvent(QFocusEvent *event) override;

private Q_SLOTS:
    void onPrivateKey(const QString &text);
    void onAddressChanged(const QString &text);
    void onOtherUser(bool checked);

private:
    QVBoxLayout *layout;
    QString title;
    QString buttonString;
    bool editMode;
    QLabel *titleLbl;
    ReptiloidsCoinLineEditWithTitle *addressTi;
    ReptiloidsCoinLineEditWithTitle *createAddressTi;
    ReptiloidsCoinLineEditWithTitle *aliasTi;
    QRadioButton *myAddressBtn;
    QRadioButton *otherUserBtn;
    ReptiloidsCoinFormBtn *confirmBtn;
    ReptiloidsCoinFormBtn *cancelBtn;
};

class ReptiloidsCoinAddressEditDialog : public QDialog {
    Q_OBJECT
public:
    explicit ReptiloidsCoinAddressEditDialog(AddressTableModel *model, WalletModel *walletModel, Qt::WindowFlags f, QWidget *parent = nullptr);
    void accept() override;
    void setData(const QString &address, const QString &alias, const int &type, const QString &key);
    ReptiloidsCoinAddressEdit *form;
protected:
    void resizeEvent(QResizeEvent *evt) override;
private:
    AddressTableModel *model;
    WalletModel *walletModel;
};

class ReptiloidsCoinAddressAddDialog : public QDialog {
Q_OBJECT
public:
    explicit ReptiloidsCoinAddressAddDialog(AddressTableModel *model, WalletModel *walletModel, Qt::WindowFlags f, QWidget *parent = nullptr);
    void accept() override;
    ReptiloidsCoinAddressEdit *form;
protected:
    void resizeEvent(QResizeEvent *evt) override;
    bool importPrivateKey(CKey & key, const QString & alias);
private:
    AddressTableModel *model;
    WalletModel *walletModel;
};

#endif // REPTILOIDSCOIN_QT_REPTILOIDSCOINADDRESSEDIT_H