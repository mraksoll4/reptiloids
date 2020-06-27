// Copyright (c) 2018-2019 The Blocknet developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <qt/reptiloidslineeditwithtitle.h>

#include <qt/reptiloidsguiutil.h>

#include <QEvent>

ReptiloidsLineEditWithTitle::ReptiloidsLineEditWithTitle(QString title, QString placeholder, int w, QFrame *parent)
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

    lineEdit = new ReptiloidsLineEdit(w);
    lineEdit->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Preferred);
    lineEdit->setPlaceholderText(placeholder);
    layout->addWidget(lineEdit, 1);

    this->setFocusProxy(lineEdit);
}

QSize ReptiloidsLineEditWithTitle::sizeHint() const {
    return { lineEdit->width(), BGU::spi(30) + layout->spacing() + titleLbl->height() };
}

bool ReptiloidsLineEditWithTitle::isEmpty() {
    return lineEdit->text().trimmed().isEmpty();
}

void ReptiloidsLineEditWithTitle::setID(const QString id) {
    this->id = id;
}

QString ReptiloidsLineEditWithTitle::getID() {
    return this->id;
}

void ReptiloidsLineEditWithTitle::setError(bool flag) {
    lineEdit->setProperty("error", flag);
}

void ReptiloidsLineEditWithTitle::setTitle(const QString &title) {
    titleLbl->setText(title);
}

void ReptiloidsLineEditWithTitle::setExpanding() {
    this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    this->lineEdit->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
}