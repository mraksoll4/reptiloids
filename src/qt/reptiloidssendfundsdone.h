// Copyright (c) 2018-2019 The Blocknet developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef REPTILOIDS_QT_REPTILOIDSSENDFUNDSDONE_H
#define REPTILOIDS_QT_REPTILOIDSSENDFUNDSDONE_H

#include <qt/reptiloidsformbtn.h>

#include <QFrame>
#include <QLabel>
#include <QVBoxLayout>

class ReptiloidsSendFundsDone : public QFrame
{
    Q_OBJECT
public:
    explicit ReptiloidsSendFundsDone(QFrame *parent = nullptr);

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
    ReptiloidsFormBtn *returnBtn;
    ReptiloidsFormBtn *sendBtn;
};

#endif // REPTILOIDS_QT_REPTILOIDSSENDFUNDSDONE_H