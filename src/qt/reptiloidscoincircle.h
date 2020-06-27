// Copyright (c) 2018-2019 The Blocknet developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef REPTILOIDSCOIN_QT_REPTILOIDSCOINCIRCLE_H
#define REPTILOIDSCOIN_QT_REPTILOIDSCOINCIRCLE_H

#include <qt/reptiloidscoinguiutil.h>

#include <QWidget>

class ReptiloidsCoinCircle : public QWidget
{
    Q_OBJECT
protected:
    void paintEvent(QPaintEvent *event) override;

public:
    explicit ReptiloidsCoinCircle(qreal w = BGU::spr(25), qreal h = BGU::spr(25),
                            QColor color1 = QColor(0xFB, 0x7F, 0x70), QColor color2 = QColor(0xF6, 0x50, 0x8A),
                            QWidget *parent = nullptr);

Q_SIGNALS:

public Q_SLOTS:

private:
    QColor color1;
    QColor color2;
    qreal w;
    qreal h;
};

#endif // REPTILOIDSCOIN_QT_REPTILOIDSCOINCIRCLE_H