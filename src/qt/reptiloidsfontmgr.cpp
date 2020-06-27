// Copyright (c) 2018-2019 The Blocknet developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <qt/reptiloidscoinfontmgr.h>

#include <QFontDatabase>

QHash<ReptiloidsCoinFontMgr::Fonts, int> ReptiloidsCoinFontMgr::fonts = QHash<ReptiloidsCoinFontMgr::Fonts, int>();

void ReptiloidsCoinFontMgr::setup() {
    // Setup fonts
    int id1 = QFontDatabase::addApplicationFont(":/redesign/Roboto-Black.ttf");
    QFontDatabase::addApplicationFont(":/redesign/Roboto-BlackItalic.ttf");
    QFontDatabase::addApplicationFont(":/redesign/Roboto-Bold.ttf");
    QFontDatabase::addApplicationFont(":/redesign/Roboto-BoldItalic.ttf");
    QFontDatabase::addApplicationFont(":/redesign/Roboto-Italic.ttf");
    QFontDatabase::addApplicationFont(":/redesign/Roboto-Light.ttf");
    QFontDatabase::addApplicationFont(":/redesign/Roboto-LightItalic.ttf");
    QFontDatabase::addApplicationFont(":/redesign/Roboto-Medium.ttf");
    QFontDatabase::addApplicationFont(":/redesign/Roboto-MediumItalic.ttf");
    QFontDatabase::addApplicationFont(":/redesign/Roboto-Regular.ttf");
    QFontDatabase::addApplicationFont(":/redesign/Roboto-Thin.ttf");
    QFontDatabase::addApplicationFont(":/redesign/Roboto-ThinItalic.ttf");
    ReptiloidsCoinFontMgr::fonts[Roboto] = id1;

    int id2 = QFontDatabase::addApplicationFont(":/redesign/RobotoMono-Bold.ttf");
    QFontDatabase::addApplicationFont(":/redesign/RobotoMono-BoldItalic.ttf");
    QFontDatabase::addApplicationFont(":/redesign/RobotoMono-Italic.ttf");
    QFontDatabase::addApplicationFont(":/redesign/RobotoMono-Light.ttf");
    QFontDatabase::addApplicationFont(":/redesign/RobotoMono-LightItalic.ttf");
    QFontDatabase::addApplicationFont(":/redesign/RobotoMono-Medium.ttf");
    QFontDatabase::addApplicationFont(":/redesign/RobotoMono-MediumItalic.ttf");
    QFontDatabase::addApplicationFont(":/redesign/RobotoMono-Regular.ttf");
    QFontDatabase::addApplicationFont(":/redesign/RobotoMono-Thin.ttf");
    QFontDatabase::addApplicationFont(":/redesign/RobotoMono-ThinItalic.ttf");
    ReptiloidsCoinFontMgr::fonts[RobotoMono] = id2;
}

QFont ReptiloidsCoinFontMgr::getFont(Fonts font) {
    QString family = QFontDatabase::applicationFontFamilies(ReptiloidsCoinFontMgr::fonts[font]).at(0);
    return QFont(family);
}
