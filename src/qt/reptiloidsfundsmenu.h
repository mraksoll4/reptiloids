// Copyright (c) 2018-2020 The Blocknet developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef REPTILOIDSCOIN_QT_REPTILOIDSCOINFUNDSMENU_H
#define REPTILOIDSCOIN_QT_REPTILOIDSCOINFUNDSMENU_H

#include <functional>

#include <QFrame>
#include <QLayout>
#include <QPushButton>
#include <QSharedPointer>
#include <QVBoxLayout>

class ReptiloidsCoinFundsMenu : public QFrame
{
    Q_OBJECT
public:
    explicit ReptiloidsCoinFundsMenu(QFrame *parent = nullptr);
    ~ReptiloidsCoinFundsMenu() override;
    void setDisplayWidget(QWidget *widget);
    void show();

    std::function<void ()> hOnSendFunds = nullptr;
    std::function<void ()> hOnRequestFunds = nullptr;

Q_SIGNALS:

public Q_SLOTS:
    void removeSelf(bool kill = true);

private Q_SLOTS:
    void onSendFunds();
    void onRequestFunds();

private:
    QVBoxLayout *layout;
    QPushButton *sendFundsBtn;
    QPushButton *requestFundsBtn;
    QWidget *displayWidget = nullptr;

    void setupBtn(QPushButton *btn);
    bool eventFilter(QObject *obj, QEvent *event) override;
};


#endif //REPTILOIDSCOIN_QT_REPTILOIDSCOINFUNDSMENU_H
