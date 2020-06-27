// Copyright (c) 2018-2019 The Blocknet developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef REPTILOIDSCOIN_QT_REPTILOIDSCOINACTIONBTN_H
#define REPTILOIDSCOIN_QT_REPTILOIDSCOINACTIONBTN_H

#include <QPushButton>
#include <QStyle>

class ReptiloidsCoinActionBtn : public QPushButton
{
    Q_OBJECT
public:
    explicit ReptiloidsCoinActionBtn(QPushButton *parent = nullptr);
    void setID(QString id) { this->id = id; }
    QString getID() { return this->id; }

protected:
    void paintEvent(QPaintEvent *event) override;

Q_SIGNALS:

public Q_SLOTS:

private:
    QString id;
    qreal s;
};

#endif // REPTILOIDSCOIN_QT_REPTILOIDSCOINACTIONBTN_H
