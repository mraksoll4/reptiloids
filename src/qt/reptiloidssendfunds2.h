// Copyright (c) 2018-2020 The Blocknet developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef REPTILOIDS_QT_REPTILOIDSSENDFUNDS2_H
#define REPTILOIDS_QT_REPTILOIDSSENDFUNDS2_H

#include <qt/reptiloidscoincontrol.h>
#include <qt/reptiloidsformbtn.h>
#include <qt/reptiloidslineedit.h>
#include <qt/reptiloidssendfundsutil.h>

#include <qt/walletmodel.h>

#include <QGridLayout>
#include <QFrame>
#include <QLabel>
#include <QRadioButton>
#include <QScrollArea>

class ReptiloidsSendFunds2List : public QFrame {
    Q_OBJECT
public:
    explicit ReptiloidsSendFunds2List(int displayUnit, QFrame *parent = nullptr);
    QSize sizeHint() const override;
    static QString getName() {
        return QString("funds");
    }
    void clear();
    void requestFocus();
    void addRow(int row, QString addr, QString amount);
    void setAmount(const CAmount &amt);
    static const int rowHeight = 50;
    static const int columns = 4;

Q_SIGNALS:
    void amount(QString addr, QString amount);
    void remove(QString addr);

private Q_SLOTS:
    void onRemove();

private:
    int displayUnit;
    QGridLayout *gridLayout;
    QSet<QWidget*> widgets;
    QVector<ReptiloidsLineEdit*> tis;
};

class ReptiloidsSendFunds2 : public ReptiloidsSendFundsPage {
    Q_OBJECT
public:
    explicit ReptiloidsSendFunds2(WalletModel *w, int id, QFrame *parent = nullptr);
    void setData(ReptiloidsSendFundsModel *model) override;
    bool validated() override;
    void clear() override;

private Q_SLOTS:
    void onCoinControl();
    void onChangeAddress();
    void ccAccepted();
    void onSplitChanged();
    void onAmount(QString addr, QString amount);
    void onRemove(QString addr);

protected:
    void keyPressEvent(QKeyEvent *event) override;
    void focusInEvent(QFocusEvent *event) override;
    void hideEvent(QHideEvent *qHideEvent) override;

private:
    int displayUnit;

    QVBoxLayout *layout;
    QScrollArea *scrollArea;
    QFrame *content;
    QVBoxLayout *contentLayout;
    QLabel *titleLbl;
    ReptiloidsLineEdit *changeAddrTi;
    QFrame *ccManualBox;
    QFrame *fundList;
    ReptiloidsFormBtn *continueBtn;
    ReptiloidsFormBtn *cancelBtn;
    QRadioButton *ccDefaultRb;
    QRadioButton *ccManualRb;
    QCheckBox *ccSplitOutputCb;
    ReptiloidsLineEdit *ccSplitOutputTi;
    QLabel *ccSummary2Lbl;
    ReptiloidsSendFunds2List *bFundList = nullptr;
    ReptiloidsCoinControlDialog *ccDialog;
    const uint maxSplitOutputs = 1500;

    void updateCoinControl();
    void updateCoinControlSummary();
    uint splitCount(bool *ok = nullptr);
    void updateDisplayUnit();
};

#endif // REPTILOIDS_QT_REPTILOIDSSENDFUNDS2_H
