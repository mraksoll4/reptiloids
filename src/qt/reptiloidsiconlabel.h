// Copyright (c) 2018-2019 The Blocknet developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef REPTILOIDS_QT_REPTILOIDSICONLABEL_H
#define REPTILOIDS_QT_REPTILOIDSICONLABEL_H

#include <QHBoxLayout>
#include <QLabel>
#include <QPixmap>
#include <QPushButton>

class ReptiloidsIconLabel : public QPushButton
{
    Q_OBJECT
public:
    explicit ReptiloidsIconLabel(QPushButton *parent = nullptr);
    ~ReptiloidsIconLabel() override;
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

#endif // REPTILOIDS_QT_REPTILOIDSICONLABEL_H
