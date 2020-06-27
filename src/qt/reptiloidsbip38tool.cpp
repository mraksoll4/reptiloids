// Copyright (c) 2018-2019 The Blocknet developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <qt/reptiloidscoinbip38tool.h>

#include <qt/reptiloidscoinhdiv.h>
#include <qt/reptiloidscoinguiutil.h>
#include <qt/reptiloidscoinkeydialog.h>

#include <QDoubleValidator>
#include <QHBoxLayout>

ReptiloidsCoinBIP38Tool::ReptiloidsCoinBIP38Tool(QWidget *popup, int id, QFrame *parent) : ReptiloidsCoinToolsPage(id, parent), popupWidget(popup), layout(new QVBoxLayout) {
    this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    this->setLayout(layout);
    layout->setContentsMargins(QMargins());
    layout->setSpacing(0);

    content = new QFrame;
    content->setContentsMargins(QMargins());
    content->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Minimum);
    content->setObjectName("contentFrame");
    contentLayout = new QVBoxLayout;
    contentLayout->setContentsMargins(QMargins());
    content->setLayout(contentLayout);
    
    scrollArea = new QScrollArea;
    scrollArea->setContentsMargins(QMargins());
    scrollArea->setWidgetResizable(true);
    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    scrollArea->setWidget(content);

    titleLbl = new QLabel(tr("BIP38 Encrypt"));
    titleLbl->setObjectName("h2");

    addressTi = new ReptiloidsCoinLineEditWithTitle(tr("Address"), tr("Enter ReptiloidsCoin address..."), BGU::spi(100));
    addressTi->setObjectName("proposal");
    addressTi->setMaximumWidth(BGU::spi(500));
    addressTi->lineEdit->setFocusPolicy(Qt::FocusPolicy::StrongFocus);
    addressTi->lineEdit->setValidator(new QRegExpValidator(QRegExp("[a-zA-Z0-9-_]+"), this));
    addressTi->lineEdit->setMaxLength(20);

    addressBtn = new ReptiloidsCoinAddressBtn;

    auto *addressBtnFrame = new QFrame;
    auto *addressBtnLayout = new QVBoxLayout;
    addressBtnFrame->setContentsMargins(QMargins());
    addressBtnLayout->setContentsMargins(0, 0, 0, 0);
    addressBtnFrame->setLayout(addressBtnLayout);
    addressBtnLayout->addWidget(addressBtn, 0, Qt::AlignBottom);

    auto *addressFrame = new QFrame;
    auto *addressLayout = new QHBoxLayout;
    addressFrame->setContentsMargins(QMargins());
    addressLayout->setContentsMargins(0, 0, 0, 0);
    addressLayout->setSpacing(BGU::spi(10));
    addressFrame->setLayout(addressLayout);
    addressLayout->addWidget(addressTi, 0, Qt::AlignLeft);
    addressLayout->addWidget(addressBtnFrame, 0, Qt::AlignJustify);
    addressLayout->addStretch(1);

    addressPasswordTi = new ReptiloidsCoinLineEditWithTitle(tr("Password"), tr("Enter password..."), BGU::spi(100));
    addressPasswordTi->setObjectName("proposal");
    addressPasswordTi->setMaximumWidth(BGU::spi(500));
    addressPasswordTi->lineEdit->setFocusPolicy(Qt::FocusPolicy::StrongFocus);
    addressPasswordTi->lineEdit->setMaxLength(20);

    generateBtn = new ReptiloidsCoinFormBtn;
    generateBtn->setText(tr("Generate Encryption Key"));

    auto *div1 = new ReptiloidsCoinHDiv;

    keyTi = new ReptiloidsCoinLineEditWithTitle(tr("Encrypted Key"), tr("Enter encrypted key..."), BGU::spi(100));
    keyTi->setObjectName("proposal");
    keyTi->setMaximumWidth(500);
    keyTi->lineEdit->setFocusPolicy(Qt::FocusPolicy::StrongFocus);
    keyTi->lineEdit->setValidator(new QRegExpValidator(QRegExp("[a-zA-Z0-9-_]+"), this));
    keyTi->lineEdit->setMaxLength(20);

    keyPasswordTi = new ReptiloidsCoinLineEditWithTitle(tr("Password"), tr("Enter password..."), BGU::spi(100));
    keyPasswordTi->setObjectName("proposal");
    keyPasswordTi->setMaximumWidth(BGU::spi(500));
    keyPasswordTi->lineEdit->setFocusPolicy(Qt::FocusPolicy::StrongFocus);
    keyPasswordTi->lineEdit->setMaxLength(20);

    decryptBtn = new ReptiloidsCoinFormBtn;
    decryptBtn->setText(tr("Decrypt Key"));

    contentLayout->addWidget(titleLbl, 0, Qt::AlignTop | Qt::AlignLeft);
    contentLayout->addSpacing(BGU::spi(10));
    contentLayout->addWidget(addressFrame);
    contentLayout->addSpacing(BGU::spi(5));
    contentLayout->addWidget(addressPasswordTi);
    //contentLayout->addSpacing(5);
    contentLayout->addWidget(generateBtn);
    contentLayout->addSpacing(BGU::spi(20));
    contentLayout->addWidget(div1);
    contentLayout->addSpacing(BGU::spi(10));
    contentLayout->addWidget(keyTi);
    contentLayout->addSpacing(BGU::spi(5));
    contentLayout->addWidget(keyPasswordTi);
    contentLayout->addSpacing(BGU::spi(5));
    contentLayout->addWidget(decryptBtn);
    contentLayout->addStretch(BGU::spi(1));

    layout->addWidget(scrollArea);

    connect(generateBtn, &ReptiloidsCoinFormBtn::clicked, this, &ReptiloidsCoinBIP38Tool::onGenerateEncryptedKey);
    connect(decryptBtn, &ReptiloidsCoinFormBtn::clicked, this, &ReptiloidsCoinBIP38Tool::onDecryptKey);
}

void ReptiloidsCoinBIP38Tool::setWalletModel(WalletModel *w) {
    if (!walletModel)
        return;

    walletModel = w;
}

void ReptiloidsCoinBIP38Tool::onGenerateEncryptedKey() {
    ReptiloidsCoinKeyDialog dlg(tr("MlgLuh-fsFDJ-Fjhabcsc-dIOJbda-nbcsc-dIOJbda-ncacjttt"), tr("Address: POKLuhfsFDJFjhabcscdIOJbdancacnksd"), tr("Add to Address Book"), tr("Your Encrypted Key"), this);
    dlg.setFixedSize(BGU::spi(500), BGU::spi(280));
    connect(&dlg, &QDialog::accepted, this, [this, &dlg]() {
        // ...
    });
    dlg.exec();
} 

void ReptiloidsCoinBIP38Tool::onDecryptKey() {
    ReptiloidsCoinKeyDialog dlg(tr("MlgLuh-fsFDJ-Fjhabcsc-dIOJbda-nbcsc-dIOJbda-ncacjttt"), tr("Address: POKLuhfsFDJFjhabcscdIOJbdancacnksd"), tr("Add to Address Book"), tr("Your Decrypted Key"), this);
    dlg.setFixedSize(BGU::spi(500), BGU::spi(280));
    connect(&dlg, &QDialog::accepted, this, [this, &dlg]() {
        // ...
    });
    dlg.exec();
} 
