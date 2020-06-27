// Copyright (c) 2018-2019 The Blocknet developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef REPTILOIDSCOIN_QT_REPTILOIDSCOINTABBTN_H
#define REPTILOIDSCOIN_QT_REPTILOIDSCOINTABBTN_H

#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>

class ReptiloidsCoinTabBtn : public QPushButton
{
    Q_OBJECT
public:
    explicit ReptiloidsCoinTabBtn(QPushButton *parent = nullptr);

protected:
    bool event(QEvent *event) override;

private:
    QVBoxLayout *layout;
    QLabel *subLine;
};

#endif // REPTILOIDSCOIN_QT_REPTILOIDSCOINTABBTN_H