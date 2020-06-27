// Copyright (c) 2018-2019 The Blocknet developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef REPTILOIDSCOIN_QT_REPTILOIDSCOINLINEEDITWITHTITLE_H
#define REPTILOIDSCOIN_QT_REPTILOIDSCOINLINEEDITWITHTITLE_H

#include <qt/reptiloidscoinlineedit.h>

#include <QBoxLayout>
#include <QFrame>
#include <QLabel>

class ReptiloidsCoinLineEditWithTitle : public QFrame
{
    Q_OBJECT
public:
    explicit ReptiloidsCoinLineEditWithTitle(QString title = "", QString placeholder = "",
            int w = BGU::spi(250), QFrame *parent = nullptr);
    void setID(QString id);
    void setError(bool flag = true);
    void setTitle(const QString &title);
    QString getID();
    bool isEmpty();
    QSize sizeHint() const override;
    ReptiloidsCoinLineEdit *lineEdit;
    void setExpanding();

Q_SIGNALS:

public Q_SLOTS:

private:
    QString id;
    QLabel *titleLbl;
    QVBoxLayout *layout;
};

#endif // REPTILOIDSCOIN_QT_REPTILOIDSCOINLINEEDITWITHTITLE_H
