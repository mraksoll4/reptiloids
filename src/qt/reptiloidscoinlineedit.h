// Copyright (c) 2018-2019 The Blocknet developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef REPTILOIDSCOIN_QT_REPTILOIDSCOINLINEEDIT_H
#define REPTILOIDSCOIN_QT_REPTILOIDSCOINLINEEDIT_H

#include <qt/reptiloidscoinguiutil.h>

#include <QLineEdit>

class ReptiloidsCoinLineEdit : public QLineEdit
{
    Q_OBJECT
public:
    explicit ReptiloidsCoinLineEdit(int w = BGU::spi(250), int h = BGU::spi(40), QLineEdit *parent = nullptr);
    void setID(QString id);
    QString getID();

Q_SIGNALS:

public Q_SLOTS:

private:
    QString id;
};

#endif // REPTILOIDSCOIN_QT_REPTILOIDSCOINLINEEDIT_H
