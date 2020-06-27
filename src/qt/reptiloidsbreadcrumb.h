// Copyright (c) 2018-2019 The Blocknet developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef REPTILOIDSCOIN_QT_REPTILOIDSCOINBREADCRUMB_H
#define REPTILOIDSCOIN_QT_REPTILOIDSCOINBREADCRUMB_H

#include <QButtonGroup>
#include <QFrame>
#include <QHBoxLayout>
#include <QLabel>
#include <QWidget>

class ReptiloidsCoinArrow : public QWidget {
    Q_OBJECT
public:
    explicit ReptiloidsCoinArrow(QWidget *parent = nullptr);

protected:
    void paintEvent(QPaintEvent *event) override;
};

class ReptiloidsCoinBreadCrumb : public QFrame {
    Q_OBJECT
public:
    explicit ReptiloidsCoinBreadCrumb(QFrame *parent = nullptr);
    ~ReptiloidsCoinBreadCrumb() override;
    QSize sizeHint() const override;
    void addCrumb(QString title, int crumb);
    int getCrumb() {
        return currentCrumb;
    }
    bool showCrumb(int crumb);

    struct ReptiloidsCoinCrumb {
        int crumb;
        QString title;
    };

Q_SIGNALS:
    void crumbChanged(int crumb);

public Q_SLOTS:
    void goToCrumb(int crumb);

private:
    QHBoxLayout *layout;
    QVector<ReptiloidsCoinCrumb> crumbs;
    QButtonGroup *group;
    int currentCrumb;
};

#endif // REPTILOIDSCOIN_QT_REPTILOIDSCOINBREADCRUMB_H