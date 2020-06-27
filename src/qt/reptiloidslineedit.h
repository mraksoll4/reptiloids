// Copyright (c) 2018-2019 The Blocknet developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef REPTILOIDS_QT_REPTILOIDSLINEEDIT_H
#define REPTILOIDS_QT_REPTILOIDSLINEEDIT_H

#include <qt/reptiloidsguiutil.h>

#include <QLineEdit>

class ReptiloidsLineEdit : public QLineEdit
{
    Q_OBJECT
public:
    explicit ReptiloidsLineEdit(int w = BGU::spi(250), int h = BGU::spi(40), QLineEdit *parent = nullptr);
    void setID(QString id);
    QString getID();

Q_SIGNALS:

public Q_SLOTS:

private:
    QString id;
};

#endif // REPTILOIDS_QT_REPTILOIDSLINEEDIT_H
