// Copyright (c) 2009-2018 Satoshi Nakamoto
// Copyright (c) 2009-2018 The Bitcoin Developers
// Copyright (c) 2014-2018 The Dash Core Developers
// Copyright (c) 2017-2018 Zumy Developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef ZUMY_CORE_MEMUSAGE_H
#define ZUMY_CORE_MEMUSAGE_H

#include "memusage.h"

#include "primitives/block.h"
#include "primitives/transaction.h"

static inline size_t RecursiveZumyUsage(const CScript& script) {
    return memusage::ZumyUsage(*static_cast<const CScriptBase*>(&script));
}

static inline size_t RecursiveZumyUsage(const COutPoint& out) {
    return 0;
}

static inline size_t RecursiveZumyUsage(const CTxIn& in) {
    return RecursiveZumyUsage(in.scriptSig) + RecursiveZumyUsage(in.prevout);
}

static inline size_t RecursiveZumyUsage(const CTxOut& out) {
    return RecursiveZumyUsage(out.scriptPubKey);
}

static inline size_t RecursiveZumyUsage(const CTransaction& tx) {
    size_t mem = memusage::ZumyUsage(tx.vin) + memusage::ZumyUsage(tx.vout);
    for (std::vector<CTxIn>::const_iterator it = tx.vin.begin(); it != tx.vin.end(); it++) {
        mem += RecursiveZumyUsage(*it);
    }
    for (std::vector<CTxOut>::const_iterator it = tx.vout.begin(); it != tx.vout.end(); it++) {
        mem += RecursiveZumyUsage(*it);
    }
    return mem;
}

static inline size_t RecursiveZumyUsage(const CMutableTransaction& tx) {
    size_t mem = memusage::ZumyUsage(tx.vin) + memusage::ZumyUsage(tx.vout);
    for (std::vector<CTxIn>::const_iterator it = tx.vin.begin(); it != tx.vin.end(); it++) {
        mem += RecursiveZumyUsage(*it);
    }
    for (std::vector<CTxOut>::const_iterator it = tx.vout.begin(); it != tx.vout.end(); it++) {
        mem += RecursiveZumyUsage(*it);
    }
    return mem;
}

static inline size_t RecursiveZumyUsage(const CBlock& block) {
    size_t mem = memusage::ZumyUsage(block.vtx);
    for (std::vector<CTransaction>::const_iterator it = block.vtx.begin(); it != block.vtx.end(); it++) {
        mem += RecursiveZumyUsage(*it);
    }
    return mem;
}

static inline size_t RecursiveZumyUsage(const CBlockLocator& locator) {
    return memusage::ZumyUsage(locator.vHave);
}

#endif // ZUMY_CORE_MEMUSAGE_H
