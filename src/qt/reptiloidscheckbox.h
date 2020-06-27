// Copyright (c) 2019 The Blocknet developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef REPTILOIDS_QT_REPTILOIDSCHECKBOX_H
#define REPTILOIDS_QT_REPTILOIDSCHECKBOX_H

#include <QCheckBox>

class ReptiloidsCheckBox : public QCheckBox {
    Q_OBJECT
public:
    explicit ReptiloidsCheckBox(const QString & text, QWidget* parent = nullptr);
    explicit ReptiloidsCheckBox(QWidget* parent = nullptr);
};


#endif //REPTILOIDS_QT_REPTILOIDSCHECKBOX_H
