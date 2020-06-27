// Copyright (c) 2018-2019 The Blocknet developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef REPTILOIDSCOIN_QT_REPTILOIDSCOINDIALOG_H
#define REPTILOIDSCOIN_QT_REPTILOIDSCOINDIALOG_H

#include <qt/reptiloidscoinformbtn.h>

#include <QBoxLayout>
#include <QDialog>
#include <QTextEdit>

class ReptiloidsCoinDialog : public QDialog
{
    Q_OBJECT
public:
    explicit ReptiloidsCoinDialog(QString message = "", QString actionMsg = "Delete", QString actionStyle = "delete", QWidget *parent = nullptr, Qt::WindowFlags f = Qt::CustomizeWindowHint);

private:
    QTextEdit *messageLbl;
    ReptiloidsCoinFormBtn *cancelBtn;
    ReptiloidsCoinFormBtn *deleteBtn;
    QVBoxLayout *layout;
};

#endif // REPTILOIDSCOIN_QT_REPTILOIDSCOINDIALOG_H
