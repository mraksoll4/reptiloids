// Copyright (c) 2018-2019 The Blocknet developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <qt/reptiloidslineedit.h>

ReptiloidsLineEdit::ReptiloidsLineEdit(int w, int h, QLineEdit *parent) : QLineEdit(parent) {
    this->setMinimumSize(w, h);
}

void ReptiloidsLineEdit::setID(const QString id) {
    this->id = id;
}

QString ReptiloidsLineEdit::getID() {
    return this->id;
}

