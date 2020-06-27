// Copyright (c) 2019 The Blocknet developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef REPTILOIDSCOIN_QT_REPTILOIDSCOINCHECKBOX_H
#define REPTILOIDSCOIN_QT_REPTILOIDSCOINCHECKBOX_H

#include <QCheckBox>

class ReptiloidsCoinCheckBox : public QCheckBox {
    Q_OBJECT
public:
    explicit ReptiloidsCoinCheckBox(const QString & text, QWidget* parent = nullptr);
    explicit ReptiloidsCoinCheckBox(QWidget* parent = nullptr);
};


#endif //REPTILOIDSCOIN_QT_REPTILOIDSCOINCHECKBOX_H
