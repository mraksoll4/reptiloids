// Copyright (c) 2018-2019 The Blocknet developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef REPTILOIDS_QT_REPTILOIDSFONTMGR_H
#define REPTILOIDS_QT_REPTILOIDSFONTMGR_H

#include <QFont>
#include <QHash>

class ReptiloidsFontMgr
{
public:
    ReptiloidsFontMgr() = default;
    enum Fonts { Roboto, RobotoMono };
    static void setup();
    static QFont getFont(Fonts font);
private:
    static QHash<Fonts, int> fonts;
};

#endif // REPTILOIDS_QT_REPTILOIDSFONTMGR_H
