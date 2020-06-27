// Copyright (c) 2018-2019 The Blocknet developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef REPTILOIDSCOIN_QT_REPTILOIDSCOINADDRESSBTN_H
#define REPTILOIDSCOIN_QT_REPTILOIDSCOINADDRESSBTN_H

#include <QPushButton>

class ReptiloidsCoinAddressBtn : public QPushButton
{
    Q_OBJECT
public:
    explicit ReptiloidsCoinAddressBtn(QPushButton *parent = nullptr);
    void setID(QString id) { this->id = id; }
    QString getID() { return this->id; }

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    QString id;
};

#endif // REPTILOIDSCOIN_QT_REPTILOIDSCOINADDRESSBTN_H
