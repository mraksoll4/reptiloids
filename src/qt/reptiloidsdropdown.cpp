// Copyright (c) 2018-2019 The Blocknet developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <qt/reptiloidsdropdown.h>

#include <qt/reptiloidsguiutil.h>

#include <QAbstractItemView>
#include <QGridLayout>
#include <QLabel>
#include <QPixmap>
#include <QPoint>
#include <QStyleFactory>

ReptiloidsDropdown::ReptiloidsDropdown(const QStringList &list, QWidget* parent) : ReptiloidsDropdown(parent) {
    for (int i = 0; i < list.size(); i++)
        this->addItem(list[i]);
}

ReptiloidsDropdown::ReptiloidsDropdown(QWidget* parent) : QComboBox(parent), ddW(BGU::spi(180)), ddH(BGU::spi(40)) {
#if defined(Q_OS_MAC)
    this->setStyle(QStyleFactory::create("Windows"));
#endif
    this->setObjectName("dropdown");
    this->setFixedSize(ddW, ddH);
    auto *layout = new QGridLayout;
    auto *label = new QLabel;
    auto *icon = new QImage(":/icons/accordion-arrow");
    label->setPixmap(QPixmap::fromImage(*icon));
    label->adjustSize();
    layout->addWidget(label, 0, 0, Qt::AlignRight);
    this->setLayout(layout);
    connect(this, static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this, &ReptiloidsDropdown::handleSelectionChanged);
}

QVariant ReptiloidsDropdown::value() const {
    return itemData(currentIndex());
}

void ReptiloidsDropdown::setValue(const QVariant& value) {
    setCurrentIndex(findData(value));
}

void ReptiloidsDropdown::handleSelectionChanged(int idx) {
    setCurrentIndex(idx);
    Q_EMIT valueChanged();
}

void ReptiloidsDropdown::showPopup() {
    QComboBox::showPopup();
    QList<QFrame *> widgets = this->findChildren<QFrame*>();
    QWidget *popup = widgets[1];
    popup->setMinimumWidth(ddW);
}

void ReptiloidsDropdown::wheelEvent(QWheelEvent *e) {
    e->ignore();
}
