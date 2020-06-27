// Copyright (c) 2018-2019 The Blocknet developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <qt/reptiloidscoincreateproposal1.h>

#include <qt/reptiloidscoinhdiv.h>
#include <qt/reptiloidscoinguiutil.h>

#include <qt/bitcoinunits.h>

#include <chainparams.h>
#include <governance/governance.h>

#include <boost/lexical_cast.hpp>

#include <QDoubleValidator>
#include <QKeyEvent>
#include <QMessageBox>

ReptiloidsCoinCreateProposal1::ReptiloidsCoinCreateProposal1(int id, QFrame *parent) : ReptiloidsCoinCreateProposalPage(id, parent),
                                                                           layout(new QVBoxLayout) {
    this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    this->setLayout(layout);
    layout->setContentsMargins(BGU::spi(52), BGU::spi(9), BGU::spi(50), 0);
    layout->setSpacing(0);

    titleLbl = new QLabel(tr("Proposals"));
    titleLbl->setObjectName("h4");

    auto *subtitleLbl = new QLabel(tr("Create Proposal"));
    subtitleLbl->setObjectName("h2");

    proposalTi = new ReptiloidsCoinLineEditWithTitle(tr("Proposal name"), tr("Enter proposal name..."));
    proposalTi->setObjectName("proposal");
    proposalTi->setExpanding();
    proposalTi->lineEdit->setFocusPolicy(Qt::FocusPolicy::StrongFocus);
    proposalTi->lineEdit->setValidator(new QRegExpValidator(QRegExp("[a-zA-Z0-9-_]+"), this));
    proposalTi->lineEdit->setMaxLength(100);

    urlTi = new ReptiloidsCoinLineEditWithTitle(tr("URL must start with http:// or https://"), tr("Enter URL..."));
    urlTi->setObjectName("url");
    urlTi->setExpanding();
    urlTi->lineEdit->setValidator(new QRegExpValidator(QRegExp("https?:\\/\\/(www\\.)?[-a-zA-Z0-9@:%._\\+~#=]{2,256}\\.[a-z]{2,6}\\b([-a-zA-Z0-9@:%_\\+.~#?&//=]*)"), this));
    urlTi->lineEdit->setMaxLength(120);

    descriptionTi = new ReptiloidsCoinLineEditWithTitle(tr("Description"), tr("Brief proposal description..."));
    descriptionTi->setObjectName("description");
    descriptionTi->setExpanding();
    descriptionTi->lineEdit->setMaxLength(120);

    auto *compactGrid = new QFrame;
    auto *gridLayout = new QGridLayout;
    gridLayout->setContentsMargins(QMargins());
    gridLayout->setVerticalSpacing(BGU::spi(8));
    compactGrid->setLayout(gridLayout);

    auto superblock = nextSuperblock();
    auto superblockStr = superblock == -1 ? QString() : QString::number(superblock);
    superBlockTi = new ReptiloidsCoinLineEditWithTitle(tr("Superblock #: Next is %1").arg(superblockStr), tr("Enter Superblock #..."), BGU::spi(50));
    superBlockTi->setObjectName("block");
    superBlockTi->setExpanding();
    superBlockTi->lineEdit->setValidator(new QRegExpValidator(QRegExp("[0-9]+"), this));
    superBlockTi->lineEdit->setText(superblockStr);

    amountTi = new ReptiloidsCoinLineEditWithTitle(tr("Amount (%1 minimum)").arg(Params().GetConsensus().proposalMinAmount/COIN), tr("Enter amount..."), BGU::spi(50));
    amountTi->setObjectName("amount");
    amountTi->setExpanding();
    amountTi->lineEdit->setValidator(new QRegExpValidator(QRegExp("[0-9]+"), this));
    amountTi->lineEdit->setMaxLength(std::to_string(Params().GetConsensus().proposalMaxAmount / COIN).length());

    paymentAddrTi = new ReptiloidsCoinLineEditWithTitle(tr("Payment address"), tr("Enter payment address..."));
    paymentAddrTi->setObjectName("address");
    paymentAddrTi->setExpanding();
    paymentAddrTi->lineEdit->setValidator(new QRegExpValidator(QRegExp("[a-zA-Z0-9]+"), this));
    paymentAddrTi->lineEdit->setMaxLength(50);

    gridLayout->addWidget(superBlockTi, 0, 0);
    gridLayout->addWidget(amountTi, 0, 1);
    gridLayout->addWidget(paymentAddrTi, 1, 0, 1, 2);

    auto *div1 = new ReptiloidsCoinHDiv;

    auto *feeGrid = new QFrame;
    auto *feeLayout = new QGridLayout;
    feeLayout->setContentsMargins(QMargins());
    feeGrid->setLayout(feeLayout);

    feeTitleLbl = new QLabel(tr("New Proposal Fee"));
    feeTitleLbl->setObjectName("h4");

    feeLbl = new QLabel(BitcoinUnits::floorWithUnit(0, Params().GetConsensus().proposalFee, 2, false, BitcoinUnits::separatorAlways));
    feeLbl->setObjectName("detail");

    feeLayout->addWidget(feeTitleLbl, 0, 0);
    feeLayout->addWidget(feeLbl, 0, 1, Qt::AlignRight);

    auto *div2 = new ReptiloidsCoinHDiv;

    charCountLbl = new QLabel;
    charCountLbl->setObjectName("charcount");

    auto *buttonGrid = new QFrame;
    buttonGrid->setContentsMargins(QMargins());
    auto *buttonLayout = new QGridLayout;
    buttonLayout->setContentsMargins(QMargins());
    buttonLayout->setColumnStretch(0, 0);
    buttonLayout->setColumnStretch(1, 2);
    buttonGrid->setLayout(buttonLayout);

    continueBtn = new ReptiloidsCoinFormBtn;
    continueBtn->setText(tr("Continue"));
    cancelBtn = new ReptiloidsCoinFormBtn;
    cancelBtn->setObjectName("cancel");
    cancelBtn->setText(tr("Cancel"));

    buttonLayout->addWidget(cancelBtn, 0, 0, Qt::AlignLeft);
    buttonLayout->addWidget(continueBtn, 0, 1, Qt::AlignLeft);

    layout->addWidget(titleLbl, 0, Qt::AlignTop | Qt::AlignLeft);
    layout->addSpacing(BGU::spi(25));
    layout->addWidget(subtitleLbl, 0, Qt::AlignTop);
    layout->addSpacing(BGU::spi(20));
    layout->addWidget(proposalTi);
    layout->addSpacing(BGU::spi(10));
    layout->addWidget(urlTi);
    layout->addSpacing(BGU::spi(10));
    layout->addWidget(descriptionTi);
    layout->addSpacing(BGU::spi(10));
    layout->addWidget(compactGrid);
    layout->addSpacing(BGU::spi(20));
    layout->addWidget(div1);
    layout->addSpacing(BGU::spi(10));
    layout->addWidget(feeGrid);
    layout->addSpacing(BGU::spi(10));
    layout->addWidget(div2);
    layout->addSpacing(BGU::spi(10));
    layout->addWidget(charCountLbl);
    layout->addSpacing(BGU::spi(10));
    layout->addStretch(1);
    layout->addWidget(buttonGrid);
    layout->addSpacing(BGU::spi(20));

    connect(continueBtn, &ReptiloidsCoinFormBtn::clicked, this, &ReptiloidsCoinCreateProposal1::onNext);
    connect(cancelBtn, &ReptiloidsCoinFormBtn::clicked, this, &ReptiloidsCoinCreateProposal1::onCancel);
    connect(proposalTi->lineEdit, &ReptiloidsCoinLineEdit::textEdited, this, &ReptiloidsCoinCreateProposal1::inputChanged);
    connect(urlTi->lineEdit, &ReptiloidsCoinLineEdit::textEdited, this, &ReptiloidsCoinCreateProposal1::inputChanged);
    connect(descriptionTi->lineEdit, &ReptiloidsCoinLineEdit::textEdited, this, &ReptiloidsCoinCreateProposal1::inputChanged);
    connect(paymentAddrTi->lineEdit, &ReptiloidsCoinLineEdit::textEdited, this, &ReptiloidsCoinCreateProposal1::inputChanged);
}

bool ReptiloidsCoinCreateProposal1::validated() {
    bool empty = proposalTi->isEmpty()
        || urlTi->isEmpty()
        || descriptionTi->isEmpty()
        || superBlockTi->isEmpty()
        || amountTi->isEmpty()
        || paymentAddrTi->isEmpty();
    if (empty) {
        QMessageBox::warning(this->parentWidget(), tr("Issue"), tr("Please fill out the entire form"));
        return false;
    }

    auto proposal = proposalTi->lineEdit->text();
    if (proposal.length() < 2) {
        QMessageBox::warning(this->parentWidget(), tr("Issue"), tr("Bad name, it's too short. The name must be at least 2 characters long"));
        return false;
    }

    const auto url = urlTi->lineEdit->text();
    QRegularExpression re("https?:\\/\\/(www\\.)?[-a-zA-Z0-9@:%._\\+~#=]{2,256}\\.[a-z]{2,6}\\b([-a-zA-Z0-9@:%_\\+.~#?&//=]*)");
    if (!re.match(url).hasMatch()) {
        QMessageBox::warning(this->parentWidget(), tr("Issue"), tr("Bad proposal url"));
        return false;
    }
    if (url.length() < 7) {
        QMessageBox::warning(this->parentWidget(), tr("Issue"), tr("Bad url, it's too short. The url must be at least 7 characters"));
        return false;
    }

    const auto description = descriptionTi->lineEdit->text().trimmed().toStdString();

    const auto nextSB = nextSuperblock();
    auto superblock = boost::lexical_cast<int>(superBlockTi->lineEdit->text().toStdString());
    if (superblock < nextSB) {
        QMessageBox::warning(this->parentWidget(), tr("Issue"), tr("Bad Superblock, please enter a future Superblock"));
        return false;
    }
    if (!gov::Governance::isSuperblock(superblock, Params().GetConsensus())) { // bad superblock
        QMessageBox::warning(this->parentWidget(), tr("Issue"), tr("Bad Superblock, the number you entered is not a Superblock"));
        return false;
    }

    const auto amount = boost::lexical_cast<int>(amountTi->lineEdit->text().toStdString());
    if (amount < Params().GetConsensus().proposalFee/COIN) { // amount is too small
        QMessageBox::warning(this->parentWidget(), tr("Issue"), tr("Amount is too small, please specify an amount of %1 or more")
                .arg(Params().GetConsensus().proposalFee/COIN));
        return false;
    }
    if (amount > Params().GetConsensus().proposalMaxAmount/COIN) { // amount is too large
        QMessageBox::warning(this->parentWidget(), tr("Issue"), tr("Amount is too large, maximum amount is %1")
                .arg(Params().GetConsensus().proposalMaxAmount/COIN));
        return false;
    }

    const auto proposalAddr = paymentAddrTi->lineEdit->text().toStdString();
    if (!IsValidDestinationString(proposalAddr)) {
        QMessageBox::warning(this->parentWidget(), tr("Issue"), tr("Bad proposal address"));
        return false;
    }

    const gov::Proposal govProposal(proposal.toStdString(), superblock, amount*COIN, proposalAddr, url.toStdString(), description);
    std::string failureReason;
    if (!govProposal.isValid(Params().GetConsensus(), &failureReason)) {
        QMessageBox::warning(this->parentWidget(), tr("Issue"), tr(failureReason.c_str()));
        return false;
    }

    int currentBlock{0};
    {
        LOCK(cs_main);
        currentBlock = chainActive.Height();
    }
    if (!gov::Governance::outsideProposalCutoff(govProposal, currentBlock, Params().GetConsensus())) {
        QMessageBox::warning(this->parentWidget(), tr("Issue"), tr("Failed to submit proposal because the proposal cutoff time has passed"));
        return false;
    }

    return true;
}

void ReptiloidsCoinCreateProposal1::focusInEvent(QFocusEvent *event) {
    QWidget::focusInEvent(event);
    proposalTi->setFocus();
}

void ReptiloidsCoinCreateProposal1::showEvent(QShowEvent *event) {
    QWidget::showEvent(event);
    if (superBlockTi->isEmpty()) {
        auto superblock = nextSuperblock();
        auto superblockStr = superblock == -1 ? QString() : QString::number(superblock);
        superBlockTi->setTitle(tr("Superblock #: Next is %1").arg(superblockStr));
        superBlockTi->lineEdit->setText(superblockStr);
    }
}

void ReptiloidsCoinCreateProposal1::keyPressEvent(QKeyEvent *event) {
    QWidget::keyPressEvent(event);
    if (this->isHidden())
        return;
    if (event->key() == Qt::Key_Enter || event->key() == Qt::Key_Return)
        onNext();
}

void ReptiloidsCoinCreateProposal1::inputChanged(const QString &) {
    static int maxCharsForEntryFields = gov::PROPOSAL_USERDEFINED_LIMIT;
    const int totalChars = proposalTi->lineEdit->text().size() + urlTi->lineEdit->text().size() +
                           descriptionTi->lineEdit->text().size() + paymentAddrTi->lineEdit->text().size();
    if (totalChars == 0)
        charCountLbl->setText("");
    else
        charCountLbl->setText(tr("Characters remaining on this proposal: %1").arg(maxCharsForEntryFields - totalChars));
}

void ReptiloidsCoinCreateProposal1::clear() {
    proposalTi->lineEdit->clear();
    urlTi->lineEdit->clear();
    descriptionTi->lineEdit->clear();
    superBlockTi->lineEdit->clear();
    amountTi->lineEdit->clear();
    paymentAddrTi->lineEdit->clear();
    inputChanged(QString()); // reset remaining proposal char count
}

int ReptiloidsCoinCreateProposal1::nextSuperblock() {
    int currentHeight{0};
    {
        LOCK(cs_main);
        currentHeight = chainActive.Height();
    }
    if (!currentHeight)
        return -1;
    return gov::NextSuperblock(Params().GetConsensus(), currentHeight);
}