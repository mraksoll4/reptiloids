// Copyright (c) 2018-2019 The Blocknet developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef REPTILOIDSCOIN_QT_REPTILOIDSCOINFONTMGR_H
#define REPTILOIDSCOIN_QT_REPTILOIDSCOINFONTMGR_H

#include <QFont>
#include <QHash>

class ReptiloidsCoinFontMgr
{
public:
    ReptiloidsCoinFontMgr() = default;
    enum Fonts { Roboto, RobotoMono };
    static void setup();
    static QFont getFont(Fonts font);
private:
    static QHash<Fonts, int> fonts;
};

#endif // REPTILOIDSCOIN_QT_REPTILOIDSCOINFONTMGR_H
