// Copyright (c) 2018-2019 The Blocknet developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef REPTILOIDS_QT_REPTILOIDSFORMBTN_H
#define REPTILOIDS_QT_REPTILOIDSFORMBTN_H

#include <QPushButton>

class ReptiloidsFormBtn : public QPushButton
{
    Q_OBJECT
public:
    explicit ReptiloidsFormBtn(QPushButton *parent = nullptr);
    void setID(QString id) { this->id = id; }
    QString getID() { return this->id; }
Q_SIGNALS:

public Q_SLOTS:

private:
    QString id;
};

#endif // REPTILOIDS_QT_REPTILOIDSFORMBTN_H
