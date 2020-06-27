// Copyright (c) 2018-2019 The Blocknet developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef REPTILOIDSCOIN_QT_REPTILOIDSCOINVARS_H
#define REPTILOIDSCOIN_QT_REPTILOIDSCOINVARS_H

#include <amount.h>

#include <QDateTime>
#include <QString>

#define REPTILOIDSCOINGUI_FUNDS_PRECISION 8
#define REPTILOIDSCOINGUI_FUNDS_MAX 100000000
#define REPTILOIDSCOINGUI_MAXCHARS 17 // string len(REPTILOIDSCOINGUI_FUNDS_MAX-1)*2+1, i.e. length of "99999999.99999999"

enum ReptiloidsCoinPage {
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

#endif // REPTILOIDSCOIN_QT_REPTILOIDSCOINVARS_H
