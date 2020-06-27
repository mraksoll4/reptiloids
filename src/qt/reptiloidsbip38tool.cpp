// Copyright (c) 2018-2019 The Blocknet developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <qt/reptiloidsbip38tool.h>

#include <qt/reptiloidshdiv.h>
#include <qt/reptiloidsguiutil.h>
#include <qt/reptiloidskeydialog.h>

#include <QDoubleValidator>
#include <QHBoxLayout>

ReptiloidsBIP38Tool::ReptiloidsBIP38Tool(QWidget *popup, int id, QFrame *parent) : ReptiloidsToolsPage(id, parent), popupWidget(popup), layout(new QVBoxLayout) {
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

    addressTi = new ReptiloidsLineEditWithTitle(tr("Address"), tr("Enter Reptiloids address..."), BGU::spi(100));
    addressTi->setObjectName("proposal");
    addressTi->setMaximumWidth(BGU::spi(500));
    addressTi->lineEdit->setFocusPolicy(Qt::FocusPolicy::StrongFocus);
    addressTi->lineEdit->setValidator(new QRegExpValidator(QRegExp("[a-zA-Z0-9-_]+"), this));
    addressTi->lineEdit->setMaxLength(20);

    addressBtn = new ReptiloidsAddressBtn;

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

    addressPasswordTi = new ReptiloidsLineEditWithTitle(tr("Password"), tr("Enter password..."), BGU::spi(100));
    addressPasswordTi->setObjectName("proposal");
    addressPasswordTi->setMaximumWidth(BGU::spi(500));
    addressPasswordTi->lineEdit->setFocusPolicy(Qt::FocusPolicy::StrongFocus);
    addressPasswordTi->lineEdit->setMaxLength(20);

    generateBtn = new ReptiloidsFormBtn;
    generateBtn->setText(tr("Generate Encryption Key"));

    auto *div1 = new ReptiloidsHDiv;

    keyTi = new ReptiloidsLineEditWithTitle(tr("Encrypted Key"), tr("Enter encrypted key..."), BGU::spi(100));
    keyTi->setObjectName("proposal");
    keyTi->setMaximumWidth(500);
    keyTi->lineEdit->setFocusPolicy(Qt::FocusPolicy::StrongFocus);
    keyTi->lineEdit->setValidator(new QRegExpValidator(QRegExp("[a-zA-Z0-9-_]+"), this));
    keyTi->lineEdit->setMaxLength(20);

    keyPasswordTi = new ReptiloidsLineEditWithTitle(tr("Password"), tr("Enter password..."), BGU::spi(100));
    keyPasswordTi->setObjectName("proposal");
    keyPasswordTi->setMaximumWidth(BGU::spi(500));
    keyPasswordTi->lineEdit->setFocusPolicy(Qt::FocusPolicy::StrongFocus);
    keyPasswordTi->lineEdit->setMaxLength(20);

    decryptBtn = new ReptiloidsFormBtn;
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

    connect(generateBtn, &ReptiloidsFormBtn::clicked, this, &ReptiloidsBIP38Tool::onGenerateEncryptedKey);
    connect(decryptBtn, &ReptiloidsFormBtn::clicked, this, &ReptiloidsBIP38Tool::onDecryptKey);
}

void ReptiloidsBIP38Tool::setWalletModel(WalletModel *w) {
    if (!walletModel)
        return;

    walletModel = w;
}

void ReptiloidsBIP38Tool::onGenerateEncryptedKey() {
    ReptiloidsKeyDialog dlg(tr("MlgLuh-fsFDJ-Fjhabcsc-dIOJbda-nbcsc-dIOJbda-ncacjttt"), tr("Address: POKLuhfsFDJFjhabcscdIOJbdancacnksd"), tr("Add to Address Book"), tr("Your Encrypted Key"), this);
    dlg.setFixedSize(BGU::spi(500), BGU::spi(280));
    connect(&dlg, &QDialog::accepted, this, [this, &dlg]() {
        // ...
    });
    dlg.exec();
} 

void ReptiloidsBIP38Tool::onDecryptKey() {
    ReptiloidsKeyDialog dlg(tr("MlgLuh-fsFDJ-Fjhabcsc-dIOJbda-nbcsc-dIOJbda-ncacjttt"), tr("Address: POKLuhfsFDJFjhabcscdIOJbdancacnksd"), tr("Add to Address Book"), tr("Your Decrypted Key"), this);
    dlg.setFixedSize(BGU::spi(500), BGU::spi(280));
    connect(&dlg, &QDialog::accepted, this, [this, &dlg]() {
        // ...
    });
    dlg.exec();
} 
