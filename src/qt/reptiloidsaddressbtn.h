// Copyright (c) 2018-2019 The Blocknet developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef REPTILOIDS_QT_REPTILOIDSADDRESSBTN_H
#define REPTILOIDS_QT_REPTILOIDSADDRESSBTN_H

#include <QPushButton>

class ReptiloidsAddressBtn : public QPushButton
{
    Q_OBJECT
public:
    explicit ReptiloidsAddressBtn(QPushButton *parent = nullptr);
    void setID(QString id) { this->id = id; }
    QString getID() { return this->id; }

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    QString id;
};

#endif // REPTILOIDS_QT_REPTILOIDSADDRESSBTN_H
