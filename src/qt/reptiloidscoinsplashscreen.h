// Copyright (c) 2011-2018 The Bitcoin Core developers
// Copyright (c) 2019-2020 The Blocknet developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef BITCOIN_QT_REPTILOIDSCOINSPLASHSCREEN_H
#define BITCOIN_QT_REPTILOIDSCOINSPLASHSCREEN_H

#include <QLabel>
#include <QWidget>

#include <memory>

class NetworkStyle;

namespace interfaces {
class Handler;
class Node;
class Wallet;
};

/** Class for the splashscreen with information of the running client.
 *
 * @note this is intentionally not a QSplashScreen. Bitcoin Core initialization
 * can take a long time, and in that case a progress window that cannot be
 * moved around and minimized has turned out to be frustrating to the user.
 */
class ReptiloidsCoinSplashScreen : public QWidget
{
    Q_OBJECT

public:
    explicit ReptiloidsCoinSplashScreen(interfaces::Node& node, Qt::WindowFlags f, const NetworkStyle *networkStyle);
    ~ReptiloidsCoinSplashScreen();

protected:
    void closeEvent(QCloseEvent *event);

public Q_SLOTS:
    /** Hide the splash screen window and schedule the splash screen object for deletion */
    void finish();

    /** Show message and progress */
    void showMessage(const QString &message);

protected:
    bool eventFilter(QObject * obj, QEvent * ev);

private:
    /** Connect core signals to splash screen */
    void subscribeToCoreSignals();
    /** Disconnect core signals to splash screen */
    void unsubscribeFromCoreSignals();
    /** Connect wallet signals to splash screen */
    void ConnectWallet(std::unique_ptr<interfaces::Wallet> wallet);

    QLabel *bg;
    QLabel *msg;

    interfaces::Node& m_node;
    std::unique_ptr<interfaces::Handler> m_handler_init_message;
    std::unique_ptr<interfaces::Handler> m_handler_show_progress;
    std::unique_ptr<interfaces::Handler> m_handler_load_wallet;
    std::list<std::unique_ptr<interfaces::Wallet>> m_connected_wallets;
    std::list<std::unique_ptr<interfaces::Handler>> m_connected_wallet_handlers;
};

#endif // BITCOIN_QT_REPTILOIDSCOINSPLASHSCREEN_H