// Copyright (c) 2018-2019 The Blocknet developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef REPTILOIDS_QT_REPTILOIDSBREADCRUMB_H
#define REPTILOIDS_QT_REPTILOIDSBREADCRUMB_H

#include <QButtonGroup>
#include <QFrame>
#include <QHBoxLayout>
#include <QLabel>
#include <QWidget>

class ReptiloidsArrow : public QWidget {
    Q_OBJECT
public:
    explicit ReptiloidsArrow(QWidget *parent = nullptr);

protected:
    void paintEvent(QPaintEvent *event) override;
};

class ReptiloidsBreadCrumb : public QFrame {
    Q_OBJECT
public:
    explicit ReptiloidsBreadCrumb(QFrame *parent = nullptr);
    ~ReptiloidsBreadCrumb() override;
    QSize sizeHint() const override;
    void addCrumb(QString title, int crumb);
    int getCrumb() {
        return currentCrumb;
    }
    bool showCrumb(int crumb);

    struct ReptiloidsCrumb {
        int crumb;
        QString title;
    };

Q_SIGNALS:
    void crumbChanged(int crumb);

public Q_SLOTS:
    void goToCrumb(int crumb);

private:
    QHBoxLayout *layout;
    QVector<ReptiloidsCrumb> crumbs;
    QButtonGroup *group;
    int currentCrumb;
};

#endif // REPTILOIDS_QT_REPTILOIDSBREADCRUMB_H