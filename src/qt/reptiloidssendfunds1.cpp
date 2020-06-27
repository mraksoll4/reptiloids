// Copyright (c) 2018-2020 The Blocknet developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <qt/reptiloidssendfunds1.h>

#include <qt/reptiloidsaddressbook.h>
#include <qt/reptiloidsguiutil.h>
#include <qt/reptiloidsiconbtn.h>

#include <QKeyEvent>
#include <QMessageBox>

ReptiloidsSendFunds1::ReptiloidsSendFunds1(WalletModel *w, int id, QFrame *parent) : ReptiloidsSendFundsPage(w, id, parent),
                                                                                 layout(new QVBoxLayout) {
//    this->setStyleSheet("border: 1px solid red");
    this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    this->setLayout(layout);
    layout->setContentsMargins(BGU::spi(15), BGU::spi(10), BGU::spi(35), BGU::spi(30));

    titleLbl = new QLabel(tr("Send Funds"));
    titleLbl->setObjectName("h4");

    auto *subtitleLbl = new QLabel(tr("Who would you like to send funds to?"));
    subtitleLbl->setObjectName("h2");

    addressTi = new ReptiloidsAddressEditor(BGU::spi(675));
    addressTi->setPlaceholderText(tr("Enter Reptiloids Address..."));
    addressTi->setAddressValidator([w](QString &addr) -> bool {
        if (w == nullptr)
            return false;
        else return w->validateAddress(addr);
    });

    auto *addAddressBtn = new ReptiloidsIconBtn(QString("Open Address Book"), ":/redesign/QuickActions/AddressBookIcon.png");

    continueBtn = new ReptiloidsFormBtn;
    continueBtn->setText(tr("Continue"));
    continueBtn->setDisabled(true);

    auto *hdiv = new QLabel;
    hdiv->setFixedHeight(1);
    hdiv->setObjectName("hdiv");

    layout->addWidget(titleLbl, 0, Qt::AlignTop | Qt::AlignLeft);
    layout->addSpacing(BGU::spi(30));
    layout->addWidget(subtitleLbl, 0, Qt::AlignTop);
    layout->addSpacing(BGU::spi(25));
    layout->addWidget(addressTi);
    layout->addSpacing(BGU::spi(20));
    layout->addWidget(addAddressBtn, 0, Qt::AlignLeft);
    layout->addSpacing(BGU::spi(35));
    layout->addWidget(hdiv);
    layout->addSpacing(BGU::spi(40));
    layout->addWidget(continueBtn);
    layout->addStretch(1);

    connect(addressTi, &ReptiloidsAddressEditor::textChanged, this, &ReptiloidsSendFunds1::textChanged);
    connect(addressTi, &ReptiloidsAddressEditor::addresses, this, &ReptiloidsSendFunds1::onAddressesChanged);
    connect(addressTi, &ReptiloidsAddressEditor::returnPressed, this, &ReptiloidsSendFunds1::onNext);
    connect(addAddressBtn, &ReptiloidsIconBtn::clicked, this, &ReptiloidsSendFunds1::openAddressBook);
    connect(continueBtn, &ReptiloidsFormBtn::clicked, this, &ReptiloidsSendFunds1::onNext);
}

void ReptiloidsSendFunds1::setData(ReptiloidsSendFundsModel *model) {
    ReptiloidsSendFundsPage::setData(model);
    addressTi->blockSignals(true);
    // Add addresses to display
    for (const SendCoinsRecipient & r : model->txRecipients())
        addressTi->addAddress(r.address);
    addressTi->blockSignals(false);
}

void ReptiloidsSendFunds1::addAddress(const QString &address) {
    addressTi->addAddress(address);
}

void ReptiloidsSendFunds1::openAddressBook() {
    ReptiloidsAddressBookDialog dlg(walletModel, Qt::WindowSystemMenuHint | Qt::WindowTitleHint);
    connect(&dlg, &ReptiloidsAddressBookDialog::send, [this](const QString &address) {
        addAddress(address);
    });
    dlg.exec();
}

void ReptiloidsSendFunds1::clear() {
    addressTi->blockSignals(true);
    addressTi->clearData();
    addressTi->blockSignals(false);
}

void ReptiloidsSendFunds1::focusInEvent(QFocusEvent *event) {
    QWidget::focusInEvent(event);
    addressTi->setFocus();
}

void ReptiloidsSendFunds1::keyPressEvent(QKeyEvent *event) {
    QWidget::keyPressEvent(event);
    if (this->isHidden())
        return;
    if (event->key() == Qt::Key_Enter || event->key() == Qt::Key_Return)
        onNext();
}

void ReptiloidsSendFunds1::textChanged() {
    continueBtn->setDisabled(addressTi->getAddresses().isEmpty());
}

bool ReptiloidsSendFunds1::validated() {
    if (addressTi->getAddresses().isEmpty()) {
        QMessageBox::warning(this->parentWidget(), tr("Issue"), tr("Please add a valid address to the send box."));
        return false;
    }

    // Use wallet to validate address
    auto addresses = addressTi->getAddresses().toList();
    QString invalidAddresses;
    for (QString &addr : addresses) {
        if (!walletModel->validateAddress(addr))
            invalidAddresses += QString("\n%1").arg(addr);
    }

    // Display invalid addresses
    if (!invalidAddresses.isEmpty()) {
        QMessageBox::warning(this->parentWidget(), tr("Issue"),
                             QString("%1:\n%2").arg(tr("Please correct the invalid addresses below"), invalidAddresses));
        return false;
    }

    return true;
}

void ReptiloidsSendFunds1::onAddressesChanged() {
    // First add any new addresses (do not overwrite existing)
    auto addresses = addressTi->getAddresses();
    QSet<QString> setAddresses;
    for (const QString &addr : addresses) {
        setAddresses.insert(addr);
        if (!model->hasRecipient(addr))
            model->addRecipient(addr, 0);
    }
    // Remove any unspecified addresses
    for (const auto & r : model->txRecipients())  {
        if (!setAddresses.contains(r.address))
            model->removeRecipient(r.address);
    }
}
