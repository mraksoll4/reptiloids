// Copyright (c) 2018-2019 The Blocknet developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef REPTILOIDSCOIN_QT_REPTILOIDSCOINSENDFUNDSDONE_H
#define REPTILOIDSCOIN_QT_REPTILOIDSCOINSENDFUNDSDONE_H

#include <qt/reptiloidscoinformbtn.h>

#include <QFrame>
#include <QLabel>
#include <QVBoxLayout>

class ReptiloidsCoinSendFundsDone : public QFrame
{
    Q_OBJECT
public:
    explicit ReptiloidsCoinSendFundsDone(QFrame *parent = nullptr);

Q_SIGNALS:
    void dashboard();
    void payment();

public Q_SLOTS:
    void onReturnToDashboard() {
        Q_EMIT dashboard();
    };
    void onSendAnotherPayment() {
        Q_EMIT payment();
    };

private:
    QVBoxLayout *layout;
    QLabel *titleLbl;
    ReptiloidsCoinFormBtn *returnBtn;
    ReptiloidsCoinFormBtn *sendBtn;
};

#endif // REPTILOIDSCOIN_QT_REPTILOIDSCOINSENDFUNDSDONE_H