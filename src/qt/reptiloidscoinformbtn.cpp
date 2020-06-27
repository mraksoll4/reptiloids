// Copyright (c) 2018-2019 The Blocknet developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <qt/reptiloidscoinformbtn.h>

#include <qt/reptiloidscoinguiutil.h>

ReptiloidsCoinFormBtn::ReptiloidsCoinFormBtn(QPushButton *parent) : QPushButton(parent) {
    this->setFixedSize(BGU::spi(160), BGU::spi(40));
    this->setFlat(true);
    this->setCursor(Qt::PointingHandCursor);
}
