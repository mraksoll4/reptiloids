// Copyright (c) 2018-2019 The Blocknet developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef REPTILOIDS_QT_REPTILOIDSCIRCLE_H
#define REPTILOIDS_QT_REPTILOIDSCIRCLE_H

#include <qt/blocknetguiutil.h>

#include <QWidget>

class BlocknetCircle : public QWidget
{
    Q_OBJECT
protected:
    void paintEvent(QPaintEvent *event) override;

public:
    explicit BlocknetCircle(qreal w = BGU::spr(25), qreal h = BGU::spr(25),
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

#endif // REPTILOIDS_QT_REPTILOIDSCIRCLE_H