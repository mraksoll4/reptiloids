// Copyright (c) 2018-2019 The Blocknet developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <qt/reptiloidscoinlineeditwithtitle.h>

#include <qt/reptiloidscoinguiutil.h>

#include <QEvent>

ReptiloidsCoinLineEditWithTitle::ReptiloidsCoinLineEditWithTitle(QString title, QString placeholder, int w, QFrame *parent)
                                                    : QFrame(parent), layout(new QVBoxLayout)
{
    this->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Preferred);
    this->setContentsMargins(QMargins());
    layout->setContentsMargins(QMargins());
    layout->setSpacing(BGU::spi(3));
    this->setLayout(layout);

    titleLbl = new QLabel(title);
    titleLbl->setObjectName("lineEditTitle");
    titleLbl->setTextInteractionFlags(Qt::TextSelectableByMouse);
    layout->addWidget(titleLbl);

    lineEdit = new ReptiloidsCoinLineEdit(w);
    lineEdit->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Preferred);
    lineEdit->setPlaceholderText(placeholder);
    layout->addWidget(lineEdit, 1);

    this->setFocusProxy(lineEdit);
}

QSize ReptiloidsCoinLineEditWithTitle::sizeHint() const {
    return { lineEdit->width(), BGU::spi(30) + layout->spacing() + titleLbl->height() };
}

bool ReptiloidsCoinLineEditWithTitle::isEmpty() {
    return lineEdit->text().trimmed().isEmpty();
}

void ReptiloidsCoinLineEditWithTitle::setID(const QString id) {
    this->id = id;
}

QString ReptiloidsCoinLineEditWithTitle::getID() {
    return this->id;
}

void ReptiloidsCoinLineEditWithTitle::setError(bool flag) {
    lineEdit->setProperty("error", flag);
}

void ReptiloidsCoinLineEditWithTitle::setTitle(const QString &title) {
    titleLbl->setText(title);
}

void ReptiloidsCoinLineEditWithTitle::setExpanding() {
    this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    this->lineEdit->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
}