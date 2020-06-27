// Copyright (c) 2018-2019 The Blocknet developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef REPTILOIDS_QT_REPTILOIDSKEYDIALOG_H
#define REPTILOIDS_QT_REPTILOIDSKEYDIALOG_H

#include <qt/reptiloidsformbtn.h>
#include <qt/reptiloidsclosebtn.h>

#include <QBoxLayout>
#include <QDialog>
#include <QLabel>

class ReptiloidsKeyDialog : public QDialog
{
    Q_OBJECT
public:
    explicit ReptiloidsKeyDialog(QString highlight = "", QString note = "", QString action = "Add to Address Book", QString title = "Your Decrypted Key", QWidget *parent = nullptr, Qt::WindowFlags f = Qt::CustomizeWindowHint);

private:
    QLabel *titleLbl;
    QLabel *highlightLbl;
    QLabel *noteLbl;
    ReptiloidsCloseBtn *cancelBtn;
    ReptiloidsFormBtn *okBtn;
    QVBoxLayout *layout;
};

#endif // REPTILOIDS_QT_REPTILOIDSKEYDIALOG_H
