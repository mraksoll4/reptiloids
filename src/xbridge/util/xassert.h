// Copyright (c) 2017-2019 The Blocknet developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

//*****************************************************************************
//*****************************************************************************

#ifndef REPTILOIDSCOIN_XBRIDGE_UTIL_XASSERT_H
#define REPTILOIDSCOIN_XBRIDGE_UTIL_XASSERT_H

#ifdef _XDEBUG
#define xassert(__expr) assert(__expr)
#else
#define xassert(__expr) void(0)
#endif

#endif // REPTILOIDSCOIN_XBRIDGE_UTIL_XASSERT_H
