// Copyright (c) 2018-2019 The Blocknet developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <qt/reptiloidscointabbar.h>

#include <qt/reptiloidscoinguiutil.h>
#include <qt/reptiloidscoinhdiv.h>
#include <qt/reptiloidscointabbtn.h>

#include <QPainter>
#include <QPushButton>

ReptiloidsCoinTabBar::ReptiloidsCoinTabBar(QFrame *parent) : QFrame(parent), mainLayout(new QVBoxLayout), layout(new QHBoxLayout) {
    this->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Preferred);
    this->setFixedHeight(BGU::spi(60));
    mainLayout->setContentsMargins(QMargins());
    mainLayout->setSpacing(0);
    this->setLayout(mainLayout);

    auto *tabFrame = new QFrame;
    tabFrame->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    layout->setContentsMargins(QMargins());
    layout->setAlignment(Qt::AlignBottom);
    tabFrame->setLayout(layout);

    group = new QButtonGroup;
    group->setExclusive(true);

    auto *hdiv = new ReptiloidsCoinHDiv;

    mainLayout->addWidget(tabFrame);
    mainLayout->addWidget(hdiv, 0, Qt::AlignTop);

    connect(group, static_cast<void(QButtonGroup::*)(int)>(&QButtonGroup::buttonClicked), this, &ReptiloidsCoinTabBar::goToTab);
}

/**
 * Add a button. Buttons are disabled by default. Use goToTab to activate a button.
 * @param title
 * @param tab
 */
void ReptiloidsCoinTabBar::addTab(QString title, int tab) {
    tabs.append({ tab, std::move(title)});

    // Remove buttons from group first
    for (QAbstractButton *btn : group->buttons())
        group->removeButton(btn);

    // Clear existing buttons
    QLayoutItem *item;
    while ((item = layout->takeAt(0)) != nullptr)
        item->widget()->deleteLater();

    for (int i = 0; i < tabs.size(); ++i) {
        ReptiloidsCoinTab &c = tabs[i];
        auto *tabBtn = new ReptiloidsCoinTabBtn;
        tabBtn->setText(c.title);
        layout->addWidget(tabBtn);
        group->addButton(tabBtn, c.tab);
    }
}

void ReptiloidsCoinTabBar::goToTab(int tab){
    Q_EMIT tabChanged(tab);
}

bool ReptiloidsCoinTabBar::showTab(int tab) {
    currentTab = tab;
    QAbstractButton *btn = group->button(tab);
    if (btn)
        btn->setChecked(true);
    return true;
}

ReptiloidsCoinTabBar::~ReptiloidsCoinTabBar() = default;
