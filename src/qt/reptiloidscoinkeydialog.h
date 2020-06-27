// Copyright (c) 2018-2019 The Blocknet developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef REPTILOIDSCOIN_QT_REPTILOIDSCOINKEYDIALOG_H
#define REPTILOIDSCOIN_QT_REPTILOIDSCOINKEYDIALOG_H

#include <qt/reptiloidscoinformbtn.h>
#include <qt/reptiloidscoinclosebtn.h>

#include <QBoxLayout>
#include <QDialog>
#include <QLabel>

class ReptiloidsCoinKeyDialog : public QDialog
{
    Q_OBJECT
public:
    explicit ReptiloidsCoinKeyDialog(QString highlight = "", QString note = "", QString action = "Add to Address Book", QString title = "Your Decrypted Key", QWidget *parent = nullptr, Qt::WindowFlags f = Qt::CustomizeWindowHint);

private:
    QLabel *titleLbl;
    QLabel *highlightLbl;
    QLabel *noteLbl;
    ReptiloidsCoinCloseBtn *cancelBtn;
    ReptiloidsCoinFormBtn *okBtn;
    QVBoxLayout *layout;
};

#endif // REPTILOIDSCOIN_QT_REPTILOIDSCOINKEYDIALOG_H
