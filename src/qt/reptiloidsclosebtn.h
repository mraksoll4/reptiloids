// Copyright (c) 2018-2019 The Blocknet developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef REPTILOIDS_QT_REPTILOIDSCLOSEBTN_H
#define REPTILOIDS_QT_REPTILOIDSCLOSEBTN_H

#include <qt/reptiloidsguiutil.h>

#include <QPaintEvent>
#include <QPushButton>
#include <QWidget>

class ReptiloidsCloseBtn : public QPushButton {
    Q_OBJECT

public:
    explicit ReptiloidsCloseBtn(qreal w = BGU::spr(25), qreal h = BGU::spr(25), QColor xColor = QColor(0xFF, 0xFF, 0xFF),
            QColor fillColor = QColor(0x37, 0x47, 0x5C), QWidget *parent = nullptr);
    void setID(const QString id) {
        this->id = id;
    }
    QString getID() {
        return this->id;
    }

protected:
    void paintEvent(QPaintEvent *event) override;

Q_SIGNALS:

public Q_SLOTS:

private:
    qreal w;
    qreal h;
    QColor xColor;
    QColor fillColor;
    QString id;
};

#endif // REPTILOIDS_QT_REPTILOIDSCLOSEBTN_H
