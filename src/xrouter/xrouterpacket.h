// Copyright (c) 2018-2019 The Blocknet developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef REPTILOIDSCOIN_XROUTER_XROUTERPACKET_H
#define REPTILOIDSCOIN_XROUTER_XROUTERPACKET_H

#include <xrouter/xrouterlogger.h>

#include <xrouter/version.h>

#include <keystore.h>

#include <deque>

#include <boost/preprocessor.hpp>

//*****************************************************************************
//*****************************************************************************
namespace xrouter
{

//******************************************************************************
//******************************************************************************

enum XRouterCommand
{
    xrInvalid                        = 0,
    xrReply                          = 1,
    xrGetReply                       = 2,
    xrGetConfig                      = 3,
    xrConfigReply                    = 4,
    xrDefault                        = 5,

    xrGetBlockCount                  = 20,
    xrGetBlockHash                   = 21,
    xrGetBlock                       = 22,
    xrGetTransaction                 = 23,
    xrSendTransaction                = 24,

    xrGetTxBloomFilter               = 40,
    xrGenerateBloomFilter            = 41,

    xrGetBlocks                      = 50,
    xrGetTransactions                = 51,
    xrGetBlockAtTime                 = 52,
    xrDecodeRawTransaction           = 53,

    xrGetBalance                     = 60,

    xrService                        = 1000,
};

inline const char* XRouterCommand_ToString(enum XRouterCommand c)
{
    switch (c)
    {
        case xrInvalid                    : return "xrInvalid";
        case xrReply                      : return "xrReply";
        case xrGetReply                   : return "xrGetReply";
        case xrGetConfig                  : return "xrGetConfig";
        case xrConfigReply                : return "xrConfigReply";
        case xrGetBlockCount              : return "xrGetBlockCount";
        case xrGetBlockHash               : return "xrGetBlockHash";
        case xrGetBlock                   : return "xrGetBlock";
        case xrGetTransaction             : return "xrGetTransaction";
        case xrSendTransaction            : return "xrSendTransaction";
        case xrGetTxBloomFilter           : return "xrGetTxBloomFilter";
        case xrGenerateBloomFilter        : return "xrGenerateBloomFilter";
        case xrGetBlocks                  : return "xrGetBlocks";
        case xrGetTransactions            : return "xrGetTransactions";
        case xrGetBlockAtTime             : return "xrGetBlockAtTime";
        case xrDecodeRawTransaction       : return "xrDecodeRawTransaction";
        case xrGetBalance                 : return "xrGetBalance";
        case xrService                    : return "xrs";
        case xrDefault                    : return "";
        default: {
            char * s = nullptr;
            sprintf(s, "[Unknown XRouterCommand] %u", c);
            return s;
        }
    }
};

inline bool XRouterCommand_IsValid(const char* c)
{
    return XRouterCommand_ToString(xrInvalid)                    == c ||
           XRouterCommand_ToString(xrReply)                      == c ||
           XRouterCommand_ToString(xrGetReply)                   == c ||
           XRouterCommand_ToString(xrGetConfig)                  == c ||
           XRouterCommand_ToString(xrConfigReply)                == c ||
           XRouterCommand_ToString(xrGetBlockCount)              == c ||
           XRouterCommand_ToString(xrGetBlockHash)               == c ||
           XRouterCommand_ToString(xrGetBlock)                   == c ||
           XRouterCommand_ToString(xrGetTransaction)             == c ||
           XRouterCommand_ToString(xrSendTransaction)            == c ||
           XRouterCommand_ToString(xrGetTxBloomFilter)           == c ||
           XRouterCommand_ToString(xrGenerateBloomFilter)        == c ||
           XRouterCommand_ToString(xrGetBlocks)                  == c ||
           XRouterCommand_ToString(xrGetTransactions)            == c ||
           XRouterCommand_ToString(xrGetBlockAtTime)             == c ||
           XRouterCommand_ToString(xrDecodeRawTransaction)       == c ||
           XRouterCommand_ToString(xrGetBalance)                 == c ||
           XRouterCommand_ToString(xrDefault)                    == c ||
           XRouterCommand_ToString(xrService)                    == c;
};

inline XRouterCommand XRouterCommand_FromString(const std::string & command)
{
    const char* c = command.c_str();

    if (strcmp(XRouterCommand_ToString(xrInvalid)              , c) == 0) return xrInvalid;
    if (strcmp(XRouterCommand_ToString(xrReply)                , c) == 0) return xrReply;
    if (strcmp(XRouterCommand_ToString(xrGetReply)             , c) == 0) return xrGetReply;
    if (strcmp(XRouterCommand_ToString(xrGetConfig)            , c) == 0) return xrGetConfig;
    if (strcmp(XRouterCommand_ToString(xrConfigReply)          , c) == 0) return xrConfigReply;
    if (strcmp(XRouterCommand_ToString(xrGetBlockCount)        , c) == 0) return xrGetBlockCount;
    if (strcmp(XRouterCommand_ToString(xrGetBlockHash)         , c) == 0) return xrGetBlockHash;
    if (strcmp(XRouterCommand_ToString(xrGetBlock)             , c) == 0) return xrGetBlock;
    if (strcmp(XRouterCommand_ToString(xrGetTransaction)       , c) == 0) return xrGetTransaction;
    if (strcmp(XRouterCommand_ToString(xrSendTransaction)      , c) == 0) return xrSendTransaction;
    if (strcmp(XRouterCommand_ToString(xrGetTxBloomFilter)     , c) == 0) return xrGetTxBloomFilter;
    if (strcmp(XRouterCommand_ToString(xrGenerateBloomFilter)  , c) == 0) return xrGenerateBloomFilter;
    if (strcmp(XRouterCommand_ToString(xrGetBlocks)            , c) == 0) return xrGetBlocks;
    if (strcmp(XRouterCommand_ToString(xrGetTransactions)      , c) == 0) return xrGetTransactions;
    if (strcmp(XRouterCommand_ToString(xrGetBlockAtTime)       , c) == 0) return xrGetBlockAtTime;
    if (strcmp(XRouterCommand_ToString(xrDecodeRawTransaction) , c) == 0) return xrDecodeRawTransaction;
    if (strcmp(XRouterCommand_ToString(xrGetBalance)           , c) == 0) return xrGetBalance;

    return xrInvalid; // default
};

inline std::set<XRouterCommand> XRouterCommands()
{
    std::set<XRouterCommand> r;
    r.insert(xrGetConfig);
    r.insert(xrGetBlockCount);
    r.insert(xrGetBlockHash);
    r.insert(xrGetBlock);
    r.insert(xrGetTransaction);
    r.insert(xrSendTransaction);
//    r.insert(xrGetTxBloomFilter);
//    r.insert(xrGenerateBloomFilter);
    r.insert(xrGetBlocks);
    r.insert(xrGetTransactions);
//    r.insert(xrGetBlockAtTime);
    r.insert(xrDecodeRawTransaction);
//    r.insert(xrGetBalance);
    return r;
};

//******************************************************************************
//******************************************************************************

//******************************************************************************
// header 6*4+36+33+64 (157 bytes)
//
// uint32_t version
// uint32_t command
// uint32_t timestamp
// uint32_t size
// uint32_t reserved
// uint32_t reserved
// unsigned char * uuid
// unsigned char * pubkey
// unsigned char * signature
//******************************************************************************
class XRouterPacket
{
    std::vector<unsigned char> m_body;

public:

    enum
    {
        versionSize      = sizeof(uint32_t),
        commandSize      = sizeof(uint32_t),
        timestampSize    = sizeof(uint32_t),
        packetSize       = sizeof(uint32_t),
        reservedSize     = 2*sizeof(uint32_t),
        uuidSize         = 36,
        pubkeySize       = 33,
        rawSignatureSize = 64,
        // header: version, command, timestamp, size, reserved*4, pubkey, signature
        headerSize       = versionSize+commandSize+timestampSize+packetSize+reservedSize+uuidSize+pubkeySize+rawSignatureSize,
        privkeySize      = 32,
    };

    XRouterPacket(const XRouterPacket & other)
    {
        m_body = other.m_body;
    }

    XRouterPacket(XRouterCommand c, const std::string & uuid) : m_body(headerSize, 0)
    {
        versionField()   = static_cast<uint32_t>(XROUTER_PROTOCOL_VERSION);
        commandField()   = static_cast<uint32_t>(c);
        timestampField() = static_cast<uint32_t>(time(nullptr));
        setuuid(uuid);
    }

    XRouterPacket() : m_body(headerSize, 0)
    {
        versionField()   = static_cast<uint32_t>(XROUTER_PROTOCOL_VERSION);
        timestampField() = static_cast<uint32_t>(time(nullptr));
    }

    explicit XRouterPacket(const std::string & raw) : m_body(raw.begin(), raw.end())
    {
        timestampField() = static_cast<uint32_t>(time(nullptr));
    }

    void         alloc()             { m_body.resize(headerSize + size()); }
    uint32_t     size()    const     { return sizeField(); }
    uint32_t     allSize() const     { return static_cast<uint32_t>(m_body.size()); }

    uint32_t version() const                         { return versionField(); }
    XRouterCommand command() const                   { return static_cast<XRouterCommand>(commandField()); }
    const unsigned char * uuid() const               { return uuidField(); }
    const unsigned char * pubkey() const             { return pubkeyField(); }
    const std::vector<unsigned char> vpubkey() const { return std::vector<unsigned char>{pubkey(), pubkey()+pubkeySize}; }
    const unsigned char * signature() const          { return signatureField(); }

    unsigned char * header()                         { return &m_body[0]; }
    unsigned char * data()                           { return &m_body[headerSize]; }

    // convert char * to std::string
    const std::string suuid() const                  {
        std::vector<unsigned char> s{uuid(), uuid()+uuidSize};
        return std::string{s.begin(), s.end()};
    }

    // Service name
    const std::string service() {
        std::string s((const char *)data());
        return s;
    }

    const std::vector<unsigned char> & body() const  { return m_body; }

    void clear()
    {
        m_body.resize(headerSize);
        commandField() = 0;
        timestampField() = static_cast<uint32_t>(time(nullptr));
        sizeField() = 0;
        memset(uuidField(), 0, uuidSize);
        memset(pubkeyField(), 0, pubkeySize);
        memset(signatureField(), 0, rawSignatureSize);
    }

    void resize(const uint32_t size)
    {
        m_body.resize(size+headerSize);
        sizeField() = static_cast<uint32_t>(m_body.size()) - headerSize;
    }

    void setData(const unsigned char data)
    {
        m_body.resize(sizeof(data) + headerSize);
        m_body[headerSize] = data;
        sizeField() = static_cast<uint32_t>(m_body.size()) - headerSize;
    }

    void setData(const std::string & data)
    {
        m_body.resize(data.size()+1 + headerSize);
        if (!data.empty()) {
            std::copy(data.begin(), data.end(), std::back_inserter(m_body));
            m_body.push_back(0);
        }
        sizeField() = static_cast<uint32_t>(m_body.size()) - headerSize;
    }

    void append(const uint16_t data)
    {
        m_body.reserve(m_body.size() + sizeof(data));
        auto * ptr = (unsigned char *)&data;
        std::copy(ptr, ptr+sizeof(data), std::back_inserter(m_body));
        sizeField() = static_cast<uint32_t>(m_body.size()) - headerSize;
    }

    void append(const uint32_t data)
    {
        m_body.reserve(m_body.size() + sizeof(data));
        auto * ptr = (unsigned char *)&data;
        std::copy(ptr, ptr+sizeof(data), std::back_inserter(m_body));
        sizeField() = static_cast<uint32_t>(m_body.size()) - headerSize;
    }

    void append(const uint64_t data)
    {
        m_body.reserve(m_body.size() + sizeof(data));
        auto * ptr = (unsigned char *)&data;
        std::copy(ptr, ptr+sizeof(data), std::back_inserter(m_body));
        sizeField() = static_cast<uint32_t>(m_body.size()) - headerSize;
    }

    void append(const unsigned char * data, const int size)
    {
        m_body.reserve(m_body.size() + size);
        std::copy(data, data+size, std::back_inserter(m_body));
        sizeField() = static_cast<uint32_t>(m_body.size()) - headerSize;
    }

    void append(const std::string & data)
    {
        m_body.reserve(m_body.size() + data.size()+1);
        std::copy(data.begin(), data.end(), std::back_inserter(m_body));
        m_body.push_back(0);
        sizeField() = static_cast<uint32_t>(m_body.size()) - headerSize;
    }

    void append(const std::vector<unsigned char> & data)
    {
        m_body.reserve(m_body.size() + data.size());
        std::copy(data.begin(), data.end(), std::back_inserter(m_body));
        sizeField() = static_cast<uint32_t>(m_body.size()) - headerSize;
    }

    void setuuid(const std::string & id) {
        const std::vector<unsigned char> v{id.begin(), id.end()};
        if (!v.empty())
            memcpy(uuidField(), &v[0], uuidSize);
    }
    void setuuid(const std::vector<unsigned char> & id) {
        if (!id.empty())
            memcpy(uuidField(), &id[0], uuidSize);
    }
    void setuuid(const char * id) {
        if (id)
            memcpy(uuidField(), &id, uuidSize);
    }

    XRouterPacket & operator = (const XRouterPacket & other)
    {
        m_body = other.m_body;
        return *this;
    }

    bool copyFrom(const std::vector<unsigned char> & data);
    bool sign(const std::vector<unsigned char> & pubkey,
              const std::vector<unsigned char> & privkey);
    bool verify();
    bool verify(const std::vector<unsigned char> & pubkey);

private:
    template<uint32_t INDEX>
    uint32_t & field32()
        { return *static_cast<uint32_t *>(static_cast<void *>(&m_body[INDEX * 4])); }

    template<uint32_t INDEX>
    uint32_t const& field32() const
        { return *static_cast<uint32_t const*>(static_cast<void const*>(&m_body[INDEX * 4])); }

    uint32_t       & versionField()              { return field32<0>(); }
    uint32_t const & versionField() const        { return field32<0>(); }
    uint32_t &       commandField()              { return field32<1>(); }
    uint32_t const & commandField() const        { return field32<1>(); }
    uint32_t &       timestampField()            { return field32<2>(); }
    uint32_t const & timestampField() const      { return field32<2>(); }
    uint32_t &       sizeField()                 { return field32<3>(); }
    uint32_t const & sizeField() const           { return field32<3>(); }

    unsigned char *       uuidField()            { return &m_body[versionSize + commandSize + timestampSize + packetSize + reservedSize]; }
    const unsigned char * uuidField() const      { return &m_body[versionSize + commandSize + timestampSize + packetSize + reservedSize]; }
    unsigned char *       pubkeyField()          { return &m_body[versionSize + commandSize + timestampSize + packetSize + reservedSize + uuidSize]; }
    const unsigned char * pubkeyField() const    { return &m_body[versionSize + commandSize + timestampSize + packetSize + reservedSize + uuidSize]; }
    unsigned char *       signatureField()       { return &m_body[versionSize + commandSize + timestampSize + packetSize + reservedSize + uuidSize + pubkeySize]; }
    const unsigned char * signatureField() const { return &m_body[versionSize + commandSize + timestampSize + packetSize + reservedSize + uuidSize + pubkeySize]; }
};

typedef std::shared_ptr<XRouterPacket> XRouterPacketPtr;
typedef std::deque<XRouterPacketPtr>   XRouterPacketQueue;
} // namespace xrouter


#endif // REPTILOIDSCOIN_XROUTER_XROUTERPACKET_H
