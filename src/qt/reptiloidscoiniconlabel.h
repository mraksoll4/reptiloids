// Copyright (c) 2018-2019 The Blocknet developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef REPTILOIDSCOIN_QT_REPTILOIDSCOINICONLABEL_H
#define REPTILOIDSCOIN_QT_REPTILOIDSCOINICONLABEL_H

#include <QHBoxLayout>
#include <QLabel>
#include <QPixmap>
#include <QPushButton>

class ReptiloidsCoinIconLabel : public QPushButton
{
    Q_OBJECT
public:
    explicit ReptiloidsCoinIconLabel(QPushButton *parent = nullptr);
    ~ReptiloidsCoinIconLabel() override;
    void setIcon(const QString active, const QString disabled);
    void setLabel(const QString &label);

protected:
    void paintEvent(QPaintEvent *e) override;

private Q_SLOTS:
    void onSelected(bool selected);

private:
    QHBoxLayout *layout;
    QLabel *label;
    QLabel *icon;
    QString iconActive;
    QString iconDisabled;
    QString labelText;
    bool *iconActiveState = nullptr;
};

#endif // REPTILOIDSCOIN_QT_REPTILOIDSCOINICONLABEL_H
