// Copyright (c) 2012-2013 The PPCoin developers
// Copyright (c) 2017-2020 The Blocknet developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <hash.h>
#include <kernel.h>
#include <script/interpreter.h>
#include <util/system.h>
#include <validation.h>

#include <boost/assign/list_of.hpp>
#include <boost/foreach.hpp>
#include <boost/lexical_cast.hpp>

using namespace std;

// ratio of group interval length between the last group and the first group
static constexpr int MODIFIER_INTERVAL_RATIO = 3;
// Modifier interval: time to elapse before new modifier is computed
unsigned int getInterval() {
    return 60;
}

// Get the last stake modifier and its generation time from a given block
static bool GetLastStakeModifier(const CBlockIndex* pindex, uint64_t& nStakeModifier, int64_t& nModifierTime) {
    if (!pindex)
        return error("GetLastStakeModifier: null pindex");
    while (pindex && pindex->pprev && !pindex->GeneratedStakeModifier())
        pindex = pindex->pprev;
    if (!pindex->GeneratedStakeModifier())
        return error("GetLastStakeModifier: no generation at genesis block");
    nStakeModifier = pindex->nStakeModifier;
    nModifierTime = pindex->GetBlockTime();
    return true;
}

// Get selection interval section (in seconds)
static int64_t GetStakeModifierSelectionIntervalSection(int nSection) {
    assert(nSection >= 0 && nSection < 64);
    int64_t a = getInterval() * 63 / (63 + ((63 - nSection) * (MODIFIER_INTERVAL_RATIO - 1)));
    return a;
}

// Get stake modifier selection interval (in seconds)
int64_t GetStakeModifierSelectionInterval() {
    int64_t nSelectionInterval = 0;
    for (int nSection = 0; nSection < 64; nSection++) {
        nSelectionInterval += GetStakeModifierSelectionIntervalSection(nSection);
    }
    return nSelectionInterval;
}

// select a block from the candidate blocks in vSortedByTimestamp, excluding
// already selected blocks in vSelectedBlocks, and with timestamp up to
// nSelectionIntervalStop.
static bool SelectBlockFromCandidates( vector<pair<int64_t, uint256> >& vSortedByTimestamp, map<uint256,
        const CBlockIndex*>& mapSelectedBlocks, int64_t nSelectionIntervalStop, uint64_t nStakeModifierPrev,
        const CBlockIndex** pindexSelected)
{
    bool fModifierV2 = false;
    bool fModifierV3 = false;
    bool fFirstRun = true;
    bool fSelected = false;
    arith_uint256 hashBest = 0;
    *pindexSelected = (const CBlockIndex*)0;
    BOOST_FOREACH (const auto & item, vSortedByTimestamp) {
        const CBlockIndex* pindex = nullptr;
        {
            LOCK(cs_main);
            pindex = LookupBlockIndex(item.second);
        }
        if (!pindex)
            return error("SelectBlockFromCandidates: failed to find block index for candidate block %s", item.second.ToString().c_str());
        if (fSelected && pindex->GetBlockTime() > nSelectionIntervalStop)
            break;

        //if the lowest block height (vSortedByTimestamp[0]) is >= switch height, use new modifier calc
        if (fFirstRun) {
            fModifierV2 = pindex->nHeight >= Params().GetConsensus().stakingModiferV2Block;
            fModifierV3 = IsProtocolV05(pindex->GetBlockTime());
            fFirstRun = false;
        }

        if (mapSelectedBlocks.count(pindex->GetBlockHash()) > 0)
            continue;

        // compute the selection hash by hashing an input that is unique to that block
        uint256 hashProof;
        if (fModifierV3)
            hashProof = pindex->hashProofOfStake;
        else if (fModifierV2)
            hashProof = pindex->GetBlockHash();
        else
            hashProof = IsProofOfStake(pindex->nHeight) ? ArithToUint256(0) : pindex->GetBlockHash();

        CDataStream ss(SER_GETHASH, 0);
        ss << hashProof << nStakeModifierPrev;
        arith_uint256 hashSelection = UintToArith256(Hash(ss.begin(), ss.end()));

        // the selection hash is divided by 2**32 so that proof-of-stake block
        // is always favored over proof-of-work block. this is to preserve
        // the energy efficiency property
        if (IsProofOfStake(pindex->nHeight))
            hashSelection >>= 32;

        if (fSelected && hashSelection < hashBest) {
            hashBest = hashSelection;
            *pindexSelected = (const CBlockIndex*)pindex;
        } else if (!fSelected) {
            fSelected = true;
            hashBest = hashSelection;
            *pindexSelected = (const CBlockIndex*)pindex;
        }
    }
    if (gArgs.GetBoolArg("-printstakemodifier", false))
        LogPrintf("SelectBlockFromCandidates: selection hash=%s\n", hashBest.ToString().c_str());
    return fSelected;
}

// Stake Modifier (hash modifier of proof-of-stake):
// The purpose of stake modifier is to prevent a txout (coin) owner from
// computing future proof-of-stake generated by this txout at the time
// of transaction confirmation. To meet kernel protocol, the txout
// must hash with a future stake modifier to generate the proof.
// Stake modifier consists of bits each of which is contributed from a
// selected block of a given block group in the past.
// The selection of a block is based on a hash of the block's proof-hash and
// the previous stake modifier.
// Stake modifier is recomputed at a fixed time interval instead of every
// block. This is to make it difficult for an attacker to gain control of
// additional bits in the stake modifier, even after generating a chain of
// blocks.
bool ComputeNextStakeModifier(const CBlockIndex* pindexPrev, uint64_t& nStakeModifier, bool& fGeneratedStakeModifier,
        const Consensus::Params & consensus)
{
    nStakeModifier = 0;
    fGeneratedStakeModifier = false;
    if (!pindexPrev) {
        fGeneratedStakeModifier = true;
        return true; // genesis block's modifier is 0
    }
    if (pindexPrev->nHeight == 0) {
        //Give a stake modifier to the first block
        fGeneratedStakeModifier = true;
        nStakeModifier = uint64_t("stakemodifier");
        return true;
    }

    const bool printModifier = gArgs.GetBoolArg("-printstakemodifier", false);

    // First find current stake modifier and its generation block time
    // if it's not old enough, return the same stake modifier
    int64_t nModifierTime = 0;
    if (!GetLastStakeModifier(pindexPrev, nStakeModifier, nModifierTime))
        return error("ComputeNextStakeModifier: unable to get last modifier");

    if (printModifier)
        LogPrintf("ComputeNextStakeModifier: prev modifier= %s time=%s\n", boost::lexical_cast<std::string>(nStakeModifier).c_str(), DateTimeStrFormat("%Y-%m-%d %H:%M:%S", nModifierTime).c_str());

    if (nModifierTime / getInterval() >= pindexPrev->GetBlockTime() / getInterval())
        return true;

    // Sort candidate blocks by timestamp
    vector<pair<int64_t, uint256> > vSortedByTimestamp;
    vSortedByTimestamp.reserve(64 * getInterval() / consensus.nPowTargetSpacing);
    int64_t nSelectionInterval = GetStakeModifierSelectionInterval();
    int64_t nSelectionIntervalStart = (pindexPrev->GetBlockTime() / getInterval()) * getInterval() - nSelectionInterval;
    const CBlockIndex* pindex = pindexPrev;

    while (pindex && pindex->GetBlockTime() >= nSelectionIntervalStart) {
        vSortedByTimestamp.push_back(make_pair(pindex->GetBlockTime(), pindex->GetBlockHash()));
        pindex = pindex->pprev;
    }

    int nHeightFirstCandidate = pindex ? (pindex->nHeight + 1) : 0;
    reverse(vSortedByTimestamp.begin(), vSortedByTimestamp.end());
    // Need to handle a side effect in the staking protocol, where selection of modifier
    // breaks a tie based on hash instead of block number, see comparator below checking
    // for this case.
    sort(vSortedByTimestamp.begin(), vSortedByTimestamp.end(),
            [](const pair<int64_t, uint256> & a, const pair<int64_t, uint256> & b) -> bool {
                if (a.first == b.first)
                    return UintToArith256(a.second) < UintToArith256(b.second);
                return a.first < b.first;
            });

    // Select 64 blocks from candidate blocks to generate stake modifier
    uint64_t nStakeModifierNew = 0;
    int64_t nSelectionIntervalStop = nSelectionIntervalStart;
    map<uint256, const CBlockIndex*> mapSelectedBlocks;
    for (int nRound = 0; nRound < min(64, (int)vSortedByTimestamp.size()); nRound++) {
        // add an interval section to the current selection round
        nSelectionIntervalStop += GetStakeModifierSelectionIntervalSection(nRound);

        // select a block from the candidates of current round
        if (!SelectBlockFromCandidates(vSortedByTimestamp, mapSelectedBlocks, nSelectionIntervalStop, nStakeModifier, &pindex))
            return error("ComputeNextStakeModifier: unable to select block at round %d", nRound);

        // write the entropy bit of the selected block
        const auto ebit = pindex->GetStakeEntropyBit();
        if (ebit)
            nStakeModifierNew |= 1ULL << nRound;
        else
            nStakeModifierNew &= ~(1ULL << nRound);

        // add the selected block from candidates to selected list
        mapSelectedBlocks.insert(make_pair(pindex->GetBlockHash(), pindex));
        if (printModifier)
            LogPrintf("ComputeNextStakeModifier: selected round %d stop=%s height=%d bit=%d\n",
                nRound, DateTimeStrFormat("%Y-%m-%d %H:%M:%S", nSelectionIntervalStop).c_str(), pindex->nHeight, ebit);
    }

    // Print selection map for visualization of the selected blocks
    if (printModifier) {
        string strSelectionMap;
        // '-' indicates proof-of-work blocks not selected
        strSelectionMap.insert(0, pindexPrev->nHeight - nHeightFirstCandidate + 1, '-');
        pindex = pindexPrev;
        while (pindex && pindex->nHeight >= nHeightFirstCandidate) {
            // '=' indicates proof-of-stake blocks not selected
            if (IsProofOfStake(pindex->nHeight))
                strSelectionMap.replace(pindex->nHeight - nHeightFirstCandidate, 1, "=");
            pindex = pindex->pprev;
        }
        for (const auto & item : mapSelectedBlocks) {
            // 'S' indicates selected proof-of-stake blocks
            // 'W' indicates selected proof-of-work blocks
            strSelectionMap.replace(item.second->nHeight - nHeightFirstCandidate, 1, IsProofOfStake(item.second->nHeight) ? "S" : "W");
        }
        LogPrintf("ComputeNextStakeModifier: selection height [%d, %d] map %s\n", nHeightFirstCandidate, pindexPrev->nHeight, strSelectionMap.c_str());
        LogPrintf("ComputeNextStakeModifier: new modifier=%s time=%s\n", boost::lexical_cast<std::string>(nStakeModifierNew).c_str(), DateTimeStrFormat("%Y-%m-%d %H:%M:%S", pindexPrev->GetBlockTime()).c_str());
    }

    nStakeModifier = nStakeModifierNew;
    fGeneratedStakeModifier = true;
    return true;
}

bool IsProtocolV05(uint64_t nTimeTx) {
    return nTimeTx >= Params().GetConsensus().stakingV05UpgradeTime;
}

bool IsProtocolV06(uint64_t nTimeTx, const Consensus::Params & consensusParams) {
    return nTimeTx >= consensusParams.stakingV06UpgradeTime;
}

bool IsProtocolV07(uint64_t nTimeTx, const Consensus::Params & consensusParams) {
    return nTimeTx >= consensusParams.stakingV07UpgradeTime;
}

// Get the stake modifier specified by the protocol to hash for a stake kernel
bool GetKernelStakeModifier(const CBlockIndex* pindexPrev, const CBlockIndex *pindexStake, const int64_t & nBlockTime,
        uint64_t & nStakeModifier, int & nStakeModifierHeight, int64_t & nStakeModifierTime)
{
    if (IsProtocolV05(nBlockTime))
        return GetKernelStakeModifierReptiloids(pindexPrev, pindexStake, nBlockTime, nStakeModifier, nStakeModifierHeight, nStakeModifierTime);
    else
        return GetKernelStakeModifierV03(pindexStake, nStakeModifier, nStakeModifierHeight, nStakeModifierTime);
}

// Select the modifier from the most recent block index.
// Fails if:
// 1) stake input block is not indexed
// 2) stake input hasn't matured
// 3) stake modifier doesn't exist on the specified index
//
// Implementation modified from peercoin (https://github.com/peercoin/peercoin/blob/70e86347e126a3dbd00a5e65b23305b2a768cb56/src/kernel.cpp#L336)
bool GetKernelStakeModifierReptiloids(const CBlockIndex *pindexPrev, const CBlockIndex *pindexStake, const int64_t & blockStakeTime,
        uint64_t & nStakeModifier, int & nStakeModifierHeight, int64_t & nStakeModifierTime)
{
    if (!pindexStake)
        return false;

    nStakeModifierHeight = pindexPrev->nHeight;
    nStakeModifierTime = pindexPrev->GetBlockTime();

    // Do not allow picking a modifier that is generated before or at the time the utxo is confirmed in a block
    const auto useInterval = static_cast<int64_t>(Params().GetConsensus().stakeMinAge);
    if (blockStakeTime - useInterval <= pindexStake->GetBlockTime())
        return error("GetKernelStakeModifierReptiloids stake min age check failed for staking block %s", pindexStake->GetBlockHash().ToString());

    nStakeModifier = pindexPrev->nStakeModifier;
    return true;
}

// The stake modifier used to hash for a stake kernel is chosen as the stake
// modifier about a selection interval later than the coin generating the kernel
bool GetKernelStakeModifierV03(const CBlockIndex *pindexStake, uint64_t & nStakeModifier, int & nStakeModifierHeight, int64_t & nStakeModifierTime)
{
    nStakeModifier = 0;
    if (!pindexStake)
        return false;

    nStakeModifierHeight = pindexStake->nHeight;
    nStakeModifierTime = pindexStake->GetBlockTime();
    int64_t nStakeModifierSelectionInterval = GetStakeModifierSelectionInterval();
    CBlockIndex* pindex = nullptr;
    CBlockIndex* pindexNext = nullptr;
    {
        LOCK(cs_main);
        pindex = chainActive[pindexStake->nHeight];
        if (chainActive.Height() >= pindexStake->nHeight + 1)
            pindexNext = chainActive[pindexStake->nHeight + 1];
    }
    auto slowSearch = [](std::map<int, CBlockIndex*> & mbi, const int blockNumber) -> CBlockIndex* {
        LOCK(cs_main);
        if (mbi.count(blockNumber))
            return mbi[blockNumber];
        return nullptr;
    };
    if (!pindexNext) // slow search
        pindexNext = slowSearch(mapHeaderIndex, pindexStake->nHeight + 1);

    // loop to find the stake modifier later by a selection interval
    while (nStakeModifierTime < pindexStake->GetBlockTime() + nStakeModifierSelectionInterval) {
        if (!pindexNext) {
            // Should never happen
            return error("Null pindexNext\n");
        }

        const int nextBlock = pindexNext->nHeight + 1;
        pindex = pindexNext;

        if (chainActive.Height() >= nextBlock)
            pindexNext = chainActive[nextBlock];
        else pindexNext = nullptr;

        if (!pindexNext) // slow search
            pindexNext = slowSearch(mapHeaderIndex, nextBlock);
        if (pindex->GeneratedStakeModifier()) {
            nStakeModifierHeight = pindex->nHeight;
            nStakeModifierTime = pindex->GetBlockTime();
        }
    }
    nStakeModifier = pindex->nStakeModifier;
    return true;
}

// Legacy stake hash
uint256 stakeHash(unsigned int nTimeTx, CDataStream ss, unsigned int prevoutIndex, uint256 prevoutHash, unsigned int nTimeBlockFrom) {
    ss << nTimeBlockFrom << prevoutIndex << prevoutHash << nTimeTx;
    return Hash(ss.begin(), ss.end());
}

// Reptiloids staking protocol (based on ppcoin V05 stake protocol)
uint256 stakeHashV05(CDataStream ss, const unsigned int & nTimeBlockFrom, const int & blockHeight, const unsigned int & prevoutIndex, const unsigned int & nTimeTx) {
    ss << nTimeBlockFrom << blockHeight << prevoutIndex << nTimeTx;
    return Hash(ss.begin(), ss.end());
}

// Reptiloids staking protocol (custom)
uint256 stakeHashV06(CDataStream ss, const uint256 & hashBlockFrom, const unsigned int & nTimeBlockFrom, const int & blockHeight, const unsigned int & prevoutIndex, const unsigned int & nTimeTx) {
    ss << hashBlockFrom << nTimeBlockFrom << blockHeight << prevoutIndex << nTimeTx;
    return Hash(ss.begin(), ss.end());
}

bool stakeTargetHit(const uint256 & hashProofOfStake, const int64_t & nValueIn, const arith_uint256 & bnTargetPerCoinDay) {
    //get the stake weight - weight is equal to coin amount
    const auto bnCoinDayWeight = arith_uint256(nValueIn) / 100;
    // Now check if proof-of-stake hash meets target protocol
    return (UintToArith256(hashProofOfStake) < bnCoinDayWeight * bnTargetPerCoinDay);
}

bool stakeTargetHitV06(const uint256 & hashProofOfStake, const int64_t & nValueIn, const arith_uint256 & bnTargetPerCoinDay) {
    // Stake weight is 1/200 of the staked input amount
    const auto bnCoinDayWeight = arith_uint256(nValueIn) / 200;
    // Now check if proof-of-stake hash meets target protocol
    return (UintToArith256(hashProofOfStake) < bnCoinDayWeight * bnTargetPerCoinDay);
}

bool stakeTargetHitV07(const uint256 & hashProofOfStake, const int64_t & currentStakingTime, const int64_t & prevStakingTime, const int64_t & nValueIn, const arith_uint256 & bnTargetPerCoinDay, const int & nPowTargetSpacing) {
    const auto numberOfSpaces = static_cast<double>(currentStakingTime - prevStakingTime) / nPowTargetSpacing;
    double multiplier{0};
    if (numberOfSpaces <= 1) { // if lte to target spacing
        multiplier = std::max<double>(0, pow(numberOfSpaces, 4)); // min 0
        multiplier = std::min<double>(1.0, multiplier); // max 1x staking weight
    } else { // if greater than target spacing
        const double pw = -1.0 * pow(numberOfSpaces - 2.0, 4) + 2.0;
        multiplier = std::max<double>(1.0, pw);
        multiplier = std::min<double>(2.0, multiplier); // max 2x staking weight
    }
    const auto stakeWeightMultiplier = arith_uint256(multiplier * 100);
    // Stake weight is 1/200 of the staked input amount multiplied by the multiplier with 100 denominator removed
    const auto bnCoinDayWeight = arith_uint256(nValueIn) * stakeWeightMultiplier / 100 / 100;
    // Now check if proof-of-stake hash meets target protocol
    return (UintToArith256(hashProofOfStake) < bnCoinDayWeight * bnTargetPerCoinDay);
}

bool CheckStakeKernelHash(const CBlockIndex *pindexPrev, const CBlockIndex *pindexStake, const unsigned int & nBits,
        const CAmount & txInAmount, const COutPoint & prevout, const int64_t & nBlockTime, const unsigned int & nNonce,
        uint256 & hashProofOfStake, const Consensus::Params & consensus)
{
    const auto & txInBlockHash = pindexStake->GetBlockHash();
    const unsigned int nTimeBlockFrom = pindexStake->GetBlockTime();

    if (nBlockTime < nTimeBlockFrom) // Transaction timestamp violation
        return error("CheckStakeKernelHash block time violation");

    if (nTimeBlockFrom + consensus.stakeMinAge > nBlockTime)
        return error("CheckStakeKernelHash min age violation: nTimeBlockFrom=%d nStakeMinAge=%d nBlockTime=%d",
                nTimeBlockFrom, consensus.stakeMinAge, nBlockTime);

    //grab difficulty
    arith_uint256 bnTargetPerCoinDay;
    bnTargetPerCoinDay.SetCompact(nBits);

    const int currentBlock = pindexPrev->nHeight + 1;

    //grab stake modifier
    uint64_t nStakeModifier = 0;
    int nStakeModifierHeight = 0;
    int64_t nStakeModifierTime = 0;
    if (!GetKernelStakeModifier(pindexPrev, pindexStake, nBlockTime, nStakeModifier, nStakeModifierHeight, nStakeModifierTime))
        return error("CheckStakeKernelHash: failed to get kernel stake modifier");

    CDataStream ss(SER_GETHASH, 0);
    ss << nStakeModifier;

    bool v07StakeProtocol = IsProtocolV07(nBlockTime, consensus);
    if (v07StakeProtocol) {
        hashProofOfStake = stakeHashV06(ss, txInBlockHash, nTimeBlockFrom, currentBlock, prevout.n, nNonce);
        return stakeTargetHitV07(hashProofOfStake, nNonce, pindexPrev->nNonce, txInAmount, bnTargetPerCoinDay, consensus.nPowTargetSpacing);
    }

    bool v06StakeProtocol = IsProtocolV06(nBlockTime, consensus);
    if (v06StakeProtocol) {
        hashProofOfStake = stakeHashV06(ss, txInBlockHash, nTimeBlockFrom, currentBlock, prevout.n, nNonce);
        return stakeTargetHitV06(hashProofOfStake, txInAmount, bnTargetPerCoinDay);
    }

    // Legacy stake target check
    bool v05StakeProtocol = IsProtocolV05(nBlockTime);
    hashProofOfStake = v05StakeProtocol ? stakeHashV05(ss, nTimeBlockFrom, currentBlock, prevout.n, nBlockTime)
                                        : stakeHash(nBlockTime, ss, prevout.n, prevout.hash, nTimeBlockFrom);
    return stakeTargetHit(hashProofOfStake, txInAmount, bnTargetPerCoinDay);
}

bool CheckProofOfStake(const CBlockHeader & block, const CBlockIndex* pindexPrev, uint256 & hashProofOfStake, const Consensus::Params & consensusParams) {
    CBlockIndex *pindexStake = nullptr;
    {
        LOCK(cs_main);
        pindexStake = LookupBlockIndex(block.hashStakeBlock);
    }
    if (!pindexStake)
        return error("Stake block not in index %s", block.hashStakeBlock.ToString());

    if (!CheckStakeKernelHash(pindexPrev, pindexStake, block.nBits, block.nStakeAmount, { block.hashStake, block.nStakeIndex },
            block.nTime, block.nNonce, hashProofOfStake, consensusParams))
    {
        return error("check kernel failed on coinstake %s, hashProof=%s %s", // may occur during initial download or if behind on block chain sync
                     block.hashStake.ToString().c_str(), hashProofOfStake.ToString().c_str(), __func__);
    }

    return true;
}

/**
 * peercoin: entropy bit.
 * @param blockHash
 * @param blockTime
 * @return
 */
unsigned int GetStakeEntropyBit(const uint256 & blockHash, const int64_t & blockTime) {
    if (IsProtocolV05(static_cast<uint64_t>(blockTime))) {
        unsigned int nEntropyBit = 0;
        nEntropyBit = static_cast<unsigned int>(UintToArith256(blockHash).GetLow64() & 1llu); // last bit of block hash
        LogPrint(BCLog::ALL, "GetStakeEntropyBit: hashBlock=%s nEntropyBit=%u\n", blockHash.ToString().c_str(), nEntropyBit);
        return nEntropyBit;
    }

    unsigned int nEntropyBit = static_cast<unsigned int>(UintToArith256(blockHash).Get64() & 1);
    LogPrint(BCLog::ALL, "GetStakeEntropyBit: hashBlock=%s nEntropyBit=%u\n", blockHash.ToString().c_str(), nEntropyBit);
    return nEntropyBit;
}

bool IsProofOfStake(int blockHeight, const Consensus::Params & consensusParams) {
    return blockHeight > consensusParams.lastPOWBlock;
}
bool IsProofOfStake(int blockHeight) {
    return IsProofOfStake(blockHeight, Params().GetConsensus());
}