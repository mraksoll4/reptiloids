// Copyright (c) 2017-2019 The Blocknet developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

//*****************************************************************************
//*****************************************************************************

#ifndef REPTILOIDSCOIN_XBRIDGE_XBRIDGEWALLET_H
#define REPTILOIDSCOIN_XBRIDGE_XBRIDGEWALLET_H

#include <sync.h>

#include <cstring>
#include <set>
#include <stdint.h>
#include <string>
#include <vector>

#include <boost/thread.hpp>

//*****************************************************************************
//*****************************************************************************
namespace xbridge
{

//*****************************************************************************
//*****************************************************************************
namespace wallet
{
typedef std::pair<std::string, std::vector<std::string> > AddressBookEntry;

struct UtxoEntry
{
    std::string txId;
    uint32_t    vout;
    double      amount{0};
    std::string address;
    std::string scriptPubKey;
    uint32_t    confirmations{0};

    std::vector<unsigned char> rawAddress;
    std::vector<unsigned char> signature;

    std::string toString() const;
    bool hasConfirmations{false};

    bool operator < (const UtxoEntry & r) const
    {
        return (txId < r.txId) || ((txId == r.txId) && (vout < r.vout));
    }

    bool operator == (const UtxoEntry & r) const
    {
        return (txId == r.txId) && (vout ==r.vout);
    }

    void setConfirmations(const uint32_t confs) {
        confirmations = confs;
        hasConfirmations = true;
    }
};

} // namespace wallet

static constexpr int XMIN_LOCKTIME_REPTS = 6;
static constexpr int XMAX_LOCKTIME_DRIFT_REPTS = 4; // must be less than XMIN_LOCKTIME_REPTS
static constexpr int XMAKER_LOCKTIME_TARGET_SECONDS = 2*60*60; // 2 hours
static constexpr int XTAKER_LOCKTIME_TARGET_SECONDS = 30*60; // 30 mins, must be less than maker locktime target
static constexpr int XSLOW_TAKER_LOCKTIME_TARGET_SECONDS = 60*60; // 1 hour, must be less than maker locktime target
static constexpr int XSLOW_REPTTIME_SECONDS = 600;
static constexpr int XLOCKTIME_DRIFT_SECONDS = XTAKER_LOCKTIME_TARGET_SECONDS/2; // 15 minutes


//*****************************************************************************
//*****************************************************************************
class WalletParam
{
public:
    WalletParam()
        : txVersion(1)
        , COIN(0)
        , minTxFee(0)
        , feePerByte(0)
        , dustAmount(0)
        , blockTime(0)
        , blockSize(1024)
        , requiredConfirmations(0)
        , serviceNodeFee(.015)
        , txWithTimeField(false)
        , isLockCoinsSupported(false)
    {
        addrPrefix.resize(1, '\0');
        scriptPrefix.resize(1, '\0');
        secretPrefix.resize(1, '\0');
    }

    WalletParam & operator = (const WalletParam & other)
    {
        title                       = other.title;
        currency                    = other.currency;
        address                     = other.address;

        m_ip                        = other.m_ip;
        m_port                      = other.m_port;
        m_user                      = other.m_user;
        m_passwd                    = other.m_passwd;

        addrPrefix                  = other.addrPrefix;
        scriptPrefix                = other.scriptPrefix;
        secretPrefix                = other.secretPrefix;

        txVersion                   = other.txVersion;
        COIN                        = other.COIN;
        minTxFee                    = other.minTxFee;
        feePerByte                  = other.feePerByte;
        dustAmount                  = other.dustAmount;
        method                      = other.method;
        blockTime                   = other.blockTime;
        blockSize                   = other.blockSize;
        requiredConfirmations       = other.requiredConfirmations;
        txWithTimeField             = other.txWithTimeField;
        isLockCoinsSupported        = other.isLockCoinsSupported;
        jsonver                     = other.jsonver;
        contenttype                 = other.contenttype;

        return *this;
    }

// TODO temporary public
public:
    std::string                  title;
    std::string                  currency;

    std::string                  address;

    std::string                  m_ip;
    std::string                  m_port;
    std::string                  m_user;
    std::string                  m_passwd;

    std::string                  addrPrefix;
    std::string                  scriptPrefix;
    std::string                  secretPrefix;
    uint32_t                     txVersion;
    uint64_t                     COIN;
    uint64_t                     minTxFee;
    uint64_t                     feePerByte;
    uint64_t                     dustAmount;
    std::string                  method;

    // block time in seconds
    uint32_t                     blockTime;

    // block size in megabytes
    uint32_t                     blockSize;

    // required confirmations for tx
    uint32_t                     requiredConfirmations;

    //service node fee, see rpc::createFeeTransaction
    const double                 serviceNodeFee;

    // serialized transaction contains time field (default not)
    bool                         txWithTimeField;

    // support for lock/unlock coins (default off)
    bool                         isLockCoinsSupported;
    mutable CCriticalSection     lockedCoinsLocker;
    std::set<wallet::UtxoEntry>  lockedCoins;

    // json version for use with rpc
    std::string                  jsonver;
    // content type for rpc requests
    std::string                  contenttype;
};

} // namespace xbridge

#endif // REPTILOIDSCOIN_XBRIDGE_XBRIDGEWALLET_H
