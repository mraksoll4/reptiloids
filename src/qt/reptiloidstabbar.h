// Copyright (c) 2018-2019 The Blocknet developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef REPTILOIDSCOIN_QT_REPTILOIDSCOINTABBAR_H
#define REPTILOIDSCOIN_QT_REPTILOIDSCOINTABBAR_H

#include <QButtonGroup>
#include <QFrame>
#include <QHBoxLayout>
#include <QLabel>
#include <QVBoxLayout>
#include <QWidget>

class ReptiloidsCoinTabBar : public QFrame {
    Q_OBJECT
public:
    explicit ReptiloidsCoinTabBar(QFrame *parent = nullptr);
    ~ReptiloidsCoinTabBar() override;

    void addTab(QString title, int tab);
    int getTab() {
        return currentTab;
    }
    bool showTab(int tab);

    struct ReptiloidsCoinTab {
        int tab;
        QString title;
    };

Q_SIGNALS:
    void tabChanged(int tab);

public Q_SLOTS:
    void goToTab(int tab);

private:
    QVBoxLayout *mainLayout;
    QHBoxLayout *layout;
    QVector<ReptiloidsCoinTab> tabs;
    QButtonGroup *group;
    int currentTab;
};

#endif // REPTILOIDSCOIN_QT_REPTILOIDSCOINTABBAR_H