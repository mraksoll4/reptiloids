// Copyright (c) 2018-2019 The Blocknet developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef REPTILOIDS_QT_REPTILOIDSTABBTN_H
#define REPTILOIDS_QT_REPTILOIDSTABBTN_H

#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>

class ReptiloidsTabBtn : public QPushButton
{
    Q_OBJECT
public:
    explicit ReptiloidsTabBtn(QPushButton *parent = nullptr);

protected:
    bool event(QEvent *event) override;

private:
    QVBoxLayout *layout;
    QLabel *subLine;
};

#endif // REPTILOIDS_QT_REPTILOIDSTABBTN_H