// Copyright (c) 2018-2019 The Blocknet developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <qt/reptiloidsfontmgr.h>

#include <QFontDatabase>

QHash<ReptiloidsFontMgr::Fonts, int> ReptiloidsFontMgr::fonts = QHash<ReptiloidsFontMgr::Fonts, int>();

void ReptiloidsFontMgr::setup() {
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
    ReptiloidsFontMgr::fonts[Roboto] = id1;

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
    ReptiloidsFontMgr::fonts[RobotoMono] = id2;
}

QFont ReptiloidsFontMgr::getFont(Fonts font) {
    QString family = QFontDatabase::applicationFontFamilies(ReptiloidsFontMgr::fonts[font]).at(0);
    return QFont(family);
}
