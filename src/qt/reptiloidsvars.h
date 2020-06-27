// Copyright (c) 2018-2019 The Blocknet developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef REPTILOIDS_QT_REPTILOIDSVARS_H
#define REPTILOIDS_QT_REPTILOIDSVARS_H

#include <amount.h>

#include <QDateTime>
#include <QString>

#define REPTILOIDSGUI_FUNDS_PRECISION 8
#define REPTILOIDSGUI_FUNDS_MAX 100000000
#define REPTILOIDSGUI_MAXCHARS 17 // string len(REPTILOIDSGUI_FUNDS_MAX-1)*2+1, i.e. length of "99999999.99999999"

enum ReptiloidsPage {
    DASHBOARD = 1,
    ADDRESSBOOK,
    SEND,
    QUICKSEND,
    REQUEST,
    HISTORY,
    SNODES,
    PROPOSALS,
    CREATEPROPOSAL,
    ANNOUNCEMENTS,
    SETTINGS,
    TOOLS,
};

#endif // REPTILOIDS_QT_REPTILOIDSVARS_H
