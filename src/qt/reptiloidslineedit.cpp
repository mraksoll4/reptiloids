// Copyright (c) 2018-2019 The Blocknet developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <qt/reptiloidscoinlineedit.h>

ReptiloidsCoinLineEdit::ReptiloidsCoinLineEdit(int w, int h, QLineEdit *parent) : QLineEdit(parent) {
    this->setMinimumSize(w, h);
}

void ReptiloidsCoinLineEdit::setID(const QString id) {
    this->id = id;
}

QString ReptiloidsCoinLineEdit::getID() {
    return this->id;
}

