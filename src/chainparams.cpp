// Copyright (c) 2009-2018 Satoshi Nakamoto
// Copyright (c) 2009-2018 The Bitcoin Developers
// Copyright (c) 2014-2018 The Dash Core Developers
// Copyright (c) 2017-2018 Zumy Developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "chainparams.h"
#include "arith_uint256.h"
#include "hash.h"
#include "consensus/merkle.h"
#include "streams.h"
#include "tinyformat.h"
#include "util.h"
#include "utilstrencodings.h"
#include "uint256.h"
#include <assert.h>
#include <boost/assign/list_of.hpp>
#include "chainparamsseeds.h"

const arith_uint256 maxUint = UintToArith256(uint256S("ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff"));

static CBlock CreateGenesisBlock(const char* pszTimestamp, const CScript& genesisOutputScript, const uint32_t nTime, const uint32_t nNonce, const uint32_t nBits, const int32_t nVersion, const CAmount& genesisReward)
{
    CMutableTransaction txNew;
    txNew.nVersion = 1;
    txNew.vin.resize(1);
    txNew.vout.resize(1);
    txNew.vin[0].scriptSig = CScript() << 1542700106 << CScriptNum(4) << std::vector<unsigned char>((const unsigned char*)pszTimestamp, (const unsigned char*)pszTimestamp + strlen(pszTimestamp));
    txNew.vout[0].nValue = genesisReward;
    txNew.vout[0].scriptPubKey = genesisOutputScript;

    CBlock genesis;
    genesis.nTime    = nTime;
    genesis.nBits    = nBits;
    genesis.nNonce   = nNonce;
    genesis.nVersion = nVersion;
    genesis.vtx.push_back(txNew);
    genesis.hashPrevBlock.SetNull();
    genesis.hashMerkleRoot = BlockMerkleRoot(genesis);
    return genesis;
}

static void MineGenesis(CBlockHeader& genesisBlock, const uint256& powLimit, bool noProduction)
{
    if(noProduction)
        genesisBlock.nTime = std::time(0);
    genesisBlock.nNonce = 0;

    printf("NOTE: Genesis nTime = %u \n", genesisBlock.nTime);
    printf("WARN: Genesis nNonce (BLANK!) = %u \n", genesisBlock.nNonce);

    arith_uint256 besthash;
    memset(&besthash,0xFF,32);
    arith_uint256 hashTarget = UintToArith256(powLimit);
    printf("Target: %s\n", hashTarget.GetHex().c_str());
    arith_uint256 newhash = UintToArith256(genesisBlock.GetHash());
    while (newhash > hashTarget) {
        genesisBlock.nNonce++;
        if (genesisBlock.nNonce == 0) {
            printf("NONCE WRAPPED, incrementing time\n");
            ++genesisBlock.nTime;
        }
        // If nothing found after trying for a while, print status
        if ((genesisBlock.nNonce & 0xfff) == 0)
            printf("nonce %08X: hash = %s (target = %s)\n",
                   genesisBlock.nNonce, newhash.ToString().c_str(),
                   hashTarget.ToString().c_str());

        if(newhash < besthash) {
            besthash = newhash;
            printf("New best: %s\n", newhash.GetHex().c_str());
        }
        newhash = UintToArith256(genesisBlock.GetHash());
    }
    printf("Genesis nTime = %u \n", genesisBlock.nTime);
    printf("Genesis nNonce = %u \n", genesisBlock.nNonce);
    printf("Genesis nBits: %08x\n", genesisBlock.nBits);
    printf("Genesis Hash = %s\n", newhash.ToString().c_str());
    printf("Genesis Hash Merkle Root = %s\n", genesisBlock.hashMerkleRoot.ToString().c_str());
}

/**
 * Build the genesis block. Note that the output of its generation
 * transaction cannot be spent since it did not originally exist in the
 * database.
 *
 * CBlock(hash=00000ffd590b14, ver=1, hashPrevBlock=00000000000000, hashMerkleRoot=e0028e, nTime=1390095618, nBits=1e0ffff0, nNonce=28917698, vtx=1)
 *   CTransaction(hash=e0028e, ver=1, vin.size=1, vout.size=1, nLockTime=0)
 *     CTxIn(COutPoint(000000, -1), coinbase 04ffff001d01044c5957697265642030392f4a616e2f3230313420546865204772616e64204578706572696d656e7420476f6573204c6976653a204f76657273746f636b2e636f6d204973204e6f7720416363657074696e6720426974636f696e73)
 *     CTxOut(nValue=50.00000000, scriptPubKey=0xA9037BAC7050C479B121CF)
 *   vMerkleTree: e0028e
 */
static CBlock CreateGenesisBlock(uint32_t nTime, uint32_t nNonce, uint32_t nBits, int32_t nVersion, const CAmount& genesisReward)
{
    const char* pszTimestamp = "Lead Developer MTM HARLOW: Zumy.co Launched";
    const CScript genesisOutputScript = CScript() << ParseHex("") << OP_CHECKSIG;
    return CreateGenesisBlock(pszTimestamp, genesisOutputScript, nTime, nNonce, nBits, nVersion, genesisReward);
}

/**
 * Main network
 */

class CMainParams : public CChainParams {
public:
    CMainParams() {
        strNetworkID = "main";
        consensus.nHardForkOne = 250000; // block
        consensus.nHardForkTwo = 375000; // block
        consensus.nMasternodePaymentsStartBlock = 10801; // Masternode Payments begin on block 10800 .
        consensus.nInstantSendKeepLock = 24;
        consensus.nBudgetPaymentsStartBlock = 20545; // Zumy doesn't use budget payments.
        consensus.nBudgetPaymentsCycleBlocks = 20545; // Blocks per month aprox
        consensus.nBudgetPaymentsWindowBlocks = 100;
        consensus.nBudgetProposalEstablishingTime = 24 * 60 * 60;
        consensus.nSuperblockStartBlock = 20546; // Zumy doesn't have superblocks.
        consensus.nSuperblockCycle = 20545; // 675 (Blocks per day) x 365.25 (Days per Year) / 12 = 20545
        consensus.nGovernanceMinQuorum = 10;
        consensus.nGovernanceFilterElements = 20000;
        consensus.nMasternodeMinimumConfirmations = 15;
        consensus.nMajorityEnforceBlockUpgrade = 750;
        consensus.nMajorityRejectBlockOutdated = 950;
        consensus.nMajorityWindow = 1000;
        consensus.powLimit = uint256S("0000ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        consensus.nPowTargetTimespan = 24 * 60 * 60; // Zumy: 24 hours
        consensus.nPowTargetSpacing = 2 * 64; // Zumy: 128 seconds
        consensus.nPowMaxAdjustDown = 32; // Zumy: 32% adjustment down
        consensus.nPowMaxAdjustUp = 16; // Zumy: 16% adjustment up
        consensus.nPowAveragingWindow = 17;
        assert(maxUint/UintToArith256(consensus.powLimit) >= consensus.nPowAveragingWindow);
        consensus.fPowAllowMinDifficultyBlocks = false;
        consensus.fPowNoRetargeting = false;
        consensus.nRuleChangeActivationThreshold = 321; // 95% of nMinerConfirmationWindow
        consensus.nMinerConfirmationWindow = 338; // nPowTargetTimespan / nPowTargetSpacing
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 28;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = 1199145601; // January 1, 2008
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = 1230767999; // December 31, 2008

        // Deployment of BIP68, BIP112, and BIP113.
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].bit = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nStartTime = 1542700106; //
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nTimeout = 1542700106; // 

        /**
         * The message start string is designed to be unlikely to occur in normal data.
         * The characters are rarely used upper ASCII, not valid as UTF-8, and produce
         * a large 32-bit integer with any alignment.
         */
        pchMessageStart[0] = 0x2f;
        pchMessageStart[1] = 0x32;
        pchMessageStart[2] = 0x45;
        pchMessageStart[3] = 0x51;
        vAlertPubKey = ParseHex("0480f5d3fc40921d940ac3b941c6ba3bc9fc6419684ff4327bbd7aa0e6c94ee609627e77d5208272b49ede4a373474a766b133376825193bc9db8f35d52ff92807");
        nDefaultPort = 32000;
        nMaxTipAge = 24 * 60 * 60;
        nPruneAfterHeight = 20545;
        startNewChain = false;

        genesis = CreateGenesisBlock(1543504836, 103043, UintToArith256(consensus.powLimit).GetCompact(), 1, (1 * COIN));
        if(startNewChain == true) { MineGenesis(genesis, consensus.powLimit, true); }

        consensus.hashGenesisBlock = genesis.GetHash();

        if(!startNewChain) {
            assert(consensus.hashGenesisBlock == uint256S("0x0000a1df5d2885908898ea64db0a0f893a5752beafad0e5503e4c22fe6a090a2"));
            assert(genesis.hashMerkleRoot == uint256S("0x14e595d32df8b7327c3a00a2ffaffc9468a13699f57ae98b31109439a7b05255"));
	}

        vSeeds.push_back(CDNSSeedData("194.32.79.73", "194.32.79.73")); 
        vSeeds.push_back(CDNSSeedData("194.32.76.30", "194.32.76.30"));
        vSeeds.push_back(CDNSSeedData("194.99.23.228", "194.99.23.228"));

        // Zumy addresses start with 'Z'
        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,80);
        // Zumy script addresses start with '5'
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,10);
        // Zumy private keys start with 'y'
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,140);
        // Zumy BIP32 pubkeys start with 'xpub' (Bitcoin defaults)
        base58Prefixes[EXT_PUBLIC_KEY] = boost::assign::list_of(0x04)(0x88)(0xB2)(0x1E).convert_to_container<std::vector<unsigned char> >();
        // Zumy BIP32 prvkeys start with 'xprv' (Bitcoin defaults)
        base58Prefixes[EXT_SECRET_KEY] = boost::assign::list_of(0x04)(0x88)(0xAD)(0xE4).convert_to_container<std::vector<unsigned char> >();
        // Zumy BIP44 coin type is '5'
        nExtCoinType = 5;

        vFixedSeeds = std::vector<SeedSpec6>(pnSeed6_main, pnSeed6_main + ARRAYLEN(pnSeed6_main));

        fMiningRequiresPeers = true;
        fDefaultConsistencyChecks = false;
        fRequireStandard = true;
        fMineBlocksOnDemand = false;
        fTestnetToBeDeprecatedFieldRPC = false;

        nPoolMaxTransactions = 3;
        nFulfilledRequestExpireTime = 60 * 60; // fulfilled requests expire in 1 hour

        strSporkKeyOld = "04244e071357b9b970e501d45181797f1fd675f19c62fb92252d3a63e31c95f94b488d95e9704b6e2985d76a6b05b4f0fa4b22027e734064f86c63480a75965a32";
        strSporkKey = "0480f5d3fc40921d940ac3b941c6ba3bc9fc6419684ff4327bbd7aa0e6c94ee609627e77d5208272b49ede4a373474a766b133376825193bc9db8f35d52ff92807";
		nEnforceNewSporkKey = 1548028800; //!> Sporks signed after tbis must use the new spork key
		nRejectOldSporkKey = 1548028100; //!> Reject old spork key after this
        strMasternodePaymentsPubKey = "0480f5d3fc40921d940ac3b941c6ba3bc9fc6419684ff4327bbd7aa0e6c94ee609627e77d5208272b49ede4a373474a766b133376825193bc9db8f35d52ff92807";

        checkpointData = (CCheckpointData) {
            boost::assign::map_list_of
            (  0, uint256S("0x0000a1df5d2885908898ea64db0a0f893a5752beafad0e5503e4c22fe6a090a2")),
            1543504836, // * UNIX timestamp of last checkpoint block
            0,          // * total number of transactions between genesis and last checkpoint
            //   (the tx=... number in the SetBestChain debug.log lines)
            2000        // * estimated number of transactions per day after checkpoint
        };

	consensus.nZumyBeginsBlock =  1800;
	consensus.nDevPhaseTotalBlocks = 250000; // extended from 65,000 to 250,000
	consensus.nIntPhaseTotalBlocks = 125000;
    consensus.nPhase1LastBlock = 375000;
	consensus.nPhase2LastBlock = 1125000;
	consensus.nPhase3LastBlock = 1375000;
	consensus.nPhase4LastBlock = 2000000;
	consensus.nPhase5LastBlock = 999999999;
    }
};
static CMainParams mainParams;

/**
 * Testnet (v3)
 */
class CTestNetParams : public CChainParams {
public:
    CTestNetParams() {
        strNetworkID = "test";
        consensus.nHardForkOne = 5000;
        consensus.nHardForkTwo = 30000;
        consensus.nMasternodePaymentsStartBlock = 0;
        consensus.nInstantSendKeepLock = 24;
        consensus.nBudgetPaymentsStartBlock = 200;
        consensus.nBudgetPaymentsCycleBlocks = 50;
        consensus.nBudgetPaymentsWindowBlocks = 10;
        consensus.nBudgetProposalEstablishingTime = 60 * 20;
        consensus.nSuperblockStartBlock = 0;
        consensus.nSuperblockCycle = 24; // 
        consensus.nGovernanceMinQuorum = 1;
        consensus.nGovernanceFilterElements = 500;
        consensus.nMasternodeMinimumConfirmations = 1;
        consensus.nMajorityEnforceBlockUpgrade = 510;
        consensus.nMajorityRejectBlockOutdated = 750;
        consensus.nMajorityWindow = 1000;
        consensus.powLimit = uint256S("00ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        consensus.nPowAveragingWindow = 17;
        consensus.nPowTargetTimespan = 24 * 60 * 60; // Zumy: 24 hours
        consensus.nPowTargetSpacing = 10; // Zumy Testnet: 10 seconds
        consensus.nPowMaxAdjustDown = 32; // Zumy: 32% adjustment down
        consensus.nPowMaxAdjustUp = 16; // Zumy: 16% adjustment up
        assert(maxUint/UintToArith256(consensus.powLimit) >= consensus.nPowAveragingWindow);
        consensus.fPowAllowMinDifficultyBlocks = true;
        consensus.fPowNoRetargeting = false;
        consensus.nRuleChangeActivationThreshold = 254; // 75% of nMinerConfirmationWindow
        consensus.nMinerConfirmationWindow = 338; // nPowTargetTimespan / nPowTargetSpacing
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 28;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = 1542700106; // January 1, 2008
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = 1542700106; // December 31, 2008

        // Deployment of BIP68, BIP112, and BIP113.
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].bit = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nStartTime = 1542700106; // June 17th 2017 10:10:00
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nTimeout = 1542700106; // June 17th 2018 12:00:00

        pchMessageStart[0] = 0x1f;
        pchMessageStart[1] = 0x22;
        pchMessageStart[2] = 0x05;
        pchMessageStart[3] = 0x30;
        vAlertPubKey = ParseHex("0480f5d3fc40921d940ac3b941c6ba3bc9fc6419684ff4327bbd7aa0e6c94ee609627e77d5208272b49ede4a373474a766b133376825193bc9db8f35d52ff92807");
        nDefaultPort = 31400;
        nMaxTipAge = 24 * 60 * 64;
        nPruneAfterHeight = 100;
        startNewChain = false;

        genesis = CreateGenesisBlock(1543504892,38, UintToArith256(consensus.powLimit).GetCompact(), 1, (1 * COIN));
        if(startNewChain == true) {
            MineGenesis(genesis, consensus.powLimit, true);
        }

        consensus.hashGenesisBlock = genesis.GetHash();

        if(!startNewChain)
            assert(consensus.hashGenesisBlock == uint256S("0x00ca5b1b383175ddd612ead5d8b61ffca932991f1421c26a31b358c731818f75"));

        vFixedSeeds.clear();
        vSeeds.clear();
        //vSeeds.push_back(CDNSSeedData("",  ""));
        //vSeeds.push_back(CDNSSeedData("", ""));

        // Testnet Zumy addresses start with 'C'
        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,28);
        // Testnet Zumy script addresses start with '8' or '9'
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,10);
        // Testnet private keys start with '9' or 'c' (Bitcoin defaults)
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,158);
        // Testnet Zumy BIP32 pubkeys start with 'tpub' (Bitcoin defaults)
        base58Prefixes[EXT_PUBLIC_KEY] = boost::assign::list_of(0x04)(0x35)(0x87)(0xCF).convert_to_container<std::vector<unsigned char> >();
        // Testnet Zumy BIP32 prvkeys start with 'tprv' (Bitcoin defaults)
        base58Prefixes[EXT_SECRET_KEY] = boost::assign::list_of(0x04)(0x35)(0x83)(0x94).convert_to_container<std::vector<unsigned char> >();
        // Testnet Zumy BIP44 coin type is '1' (All coin's testnet default)
        nExtCoinType = 1;

        vFixedSeeds = std::vector<SeedSpec6>(pnSeed6_test, pnSeed6_test + ARRAYLEN(pnSeed6_test));

        fMiningRequiresPeers = true;
        fDefaultConsistencyChecks = false;
        fRequireStandard = false;
        fMineBlocksOnDemand = false;
        fTestnetToBeDeprecatedFieldRPC = true;

        nPoolMaxTransactions = 3;
        nFulfilledRequestExpireTime = 5 * 60; // fulfilled requests expire in 5 minutes
        strSporkKeyOld = "04244e071357b9b970e501d45181797f1fd675f19c62fb92252d3a63e31c95f94b488d95e9704b6e2985d76a6b05b4f0fa4b22027e734064f86c63480a75965a32";
        strSporkKey = "0480f5d3fc40921d940ac3b941c6ba3bc9fc6419684ff4327bbd7aa0e6c94ee609627e77d5208272b49ede4a373474a766b133376825193bc9db8f35d52ff92807";
		nEnforceNewSporkKey = 1548028800; //!> Sporks signed after tbis must use the new spork key
		nRejectOldSporkKey = 1548028100; //!> Reject old spork key after this
        strMasternodePaymentsPubKey = "0480f5d3fc40921d940ac3b941c6ba3bc9fc6419684ff4327bbd7aa0e6c94ee609627e77d5208272b49ede4a373474a766b133376825193bc9db8f35d52ff92807";

        checkpointData = (CCheckpointData) {
            boost::assign::map_list_of
            (  0, uint256S("0x00ca5b1b383175ddd612ead5d8b61ffca932991f1421c26a31b358c731818f75")),
            1543504892, // * UNIX timestamp of last checkpoint block
            0,    // * total number of transactions between genesis and last checkpoint
            //   (the tx=... number in the SetBestChain debug.log lines)
            1000        // * estimated number of transactions per day after checkpoint
        };

	consensus.nIntPhaseTotalBlocks = 5000;
  consensus.nPhase1LastBlock = 30000;
	consensus.nPhase2LastBlock = 60000;
	consensus.nPhase3LastBlock = 80000;
	consensus.nPhase4LastBlock = 105000;
    }
};
static CTestNetParams testNetParams;

/**
 * Regression test
 */
class CRegTestParams : public CChainParams {
public:
    CRegTestParams() {
        strNetworkID = "regtest";
        consensus.nMasternodePaymentsStartBlock = 0;
        consensus.nInstantSendKeepLock = 24;
        consensus.nBudgetPaymentsStartBlock = 1000;
        consensus.nBudgetPaymentsCycleBlocks = 50;
        consensus.nBudgetPaymentsWindowBlocks = 10;
        consensus.nBudgetProposalEstablishingTime = 60 * 20;
        consensus.nSuperblockStartBlock = 0;
        consensus.nSuperblockCycle = 10;
        consensus.nGovernanceMinQuorum = 1;
        consensus.nGovernanceFilterElements = 100;
        consensus.nMasternodeMinimumConfirmations = 1;
        consensus.nMajorityEnforceBlockUpgrade = 750;
        consensus.nMajorityRejectBlockOutdated = 950;
        consensus.nMajorityWindow = 1000;
        consensus.powLimit = uint256S("00ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        consensus.nPowTargetTimespan = 24 * 60 * 60; // Zumy: 24 hours
        consensus.nPowTargetSpacing = 2 * 64; // Zumy: 256 seconds
        consensus.nPowMaxAdjustDown = 32; // Zumy: 32% adjustment down
        consensus.nPowMaxAdjustUp = 16; // Zumy: 16% adjustment up
        assert(maxUint/UintToArith256(consensus.powLimit) >= consensus.nPowAveragingWindow);
        consensus.fPowAllowMinDifficultyBlocks = true;
        consensus.fPowNoRetargeting = true;
        consensus.nRuleChangeActivationThreshold = 254; // 75% of nMinerConfirmationWindow
        consensus.nMinerConfirmationWindow = 338; // Faster than normal for regtest (144 instead of 2016)
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 28;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = 999999999999ULL;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].bit = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nStartTime = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nTimeout = 999999999999ULL;

        pchMessageStart[0] = 0x1f;
        pchMessageStart[1] = 0x22;
        pchMessageStart[2] = 0x05;
        pchMessageStart[3] = 0x2f;
        nMaxTipAge = 24 * 60 * 64;
        nDefaultPort = 31500;
        nPruneAfterHeight = 100;
        startNewChain = false;

        genesis = CreateGenesisBlock(1543504892, 38, UintToArith256(consensus.powLimit).GetCompact(), 1, (1 * COIN));
        if(startNewChain == true) {
            MineGenesis(genesis, consensus.powLimit, true);
        }

        consensus.hashGenesisBlock = genesis.GetHash();

        if(!startNewChain)
            assert(consensus.hashGenesisBlock == uint256S("0x00ca5b1b383175ddd612ead5d8b61ffca932991f1421c26a31b358c731818f75"));

        vFixedSeeds.clear(); //! Regtest mode doesn't have any fixed seeds.
        vSeeds.clear();  //! Regtest mode doesn't have any DNS seeds.

        fMiningRequiresPeers = false;
        fDefaultConsistencyChecks = true;
        fRequireStandard = false;
        fMineBlocksOnDemand = true;
        fTestnetToBeDeprecatedFieldRPC = false;

        nFulfilledRequestExpireTime = 5 * 60; // fulfilled requests expire in 5 minutes
        checkpointData = (CCheckpointData) {
            boost::assign::map_list_of
            (  0, uint256S("0x00ca5b1b383175ddd612ead5d8b61ffca932991f1421c26a31b358c731818f75")),
            1543480541, // * UNIX timestamp of last checkpoint block
            0,    // * total number of transactions between genesis and last checkpoint
            //   (the tx=... number in the SetBestChain debug.log lines)
            500        // * estimated number of transactions per day after checkpoint
        };
        // Regtest Zumy addresses start with 'C'
        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,28);
        // Regtest Zumy script addresses start with '8' or '9'
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,19);
        // Regtest private keys start with '9' or 'c' (Bitcoin defaults)
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,239);
        // Regtest Zumy BIP32 pubkeys start with 'tpub' (Bitcoin defaults)
        base58Prefixes[EXT_PUBLIC_KEY] = boost::assign::list_of(0x04)(0x35)(0x87)(0xCF).convert_to_container<std::vector<unsigned char> >();
        // Regtest Zumy BIP32 prvkeys start with 'tprv' (Bitcoin defaults)
        base58Prefixes[EXT_SECRET_KEY] = boost::assign::list_of(0x04)(0x35)(0x83)(0x94).convert_to_container<std::vector<unsigned char> >();
        // Regtest Zumy BIP44 coin type is '1' (All coin's testnet default)
        nExtCoinType = 1;

	consensus.nIntPhaseTotalBlocks = 125000;
  consensus.nPhase1LastBlock = 375000;
	consensus.nPhase2LastBlock = 1125000;
	consensus.nPhase3LastBlock = 1375000;
	consensus.nPhase4LastBlock = 2000000;
    }
};
static CRegTestParams regTestParams;

static CChainParams *pCurrentParams = 0;

const CChainParams &Params() {
    assert(pCurrentParams);
    return *pCurrentParams;
}

CChainParams& Params(const std::string& chain)
{
    if (chain == CBaseChainParams::MAIN)
        return mainParams;
    else if (chain == CBaseChainParams::TESTNET)
        return testNetParams;
    else if (chain == CBaseChainParams::REGTEST)
        return regTestParams;
    else
        throw std::runtime_error(strprintf("%s: Unknown chain %s.", __func__, chain));
}

void SelectParams(const std::string& network)
{
    SelectBaseParams(network);
    pCurrentParams = &Params(network);
}
