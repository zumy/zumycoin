// Copyright (c) 2009-2018 Satoshi Nakamoto
// Copyright (c) 2009-2018 The Bitcoin Developers
// Copyright (c) 2014-2018 The Dash Core Developers
// Copyright (c) 2017-2018 Zumy Developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef ZUMY_VERSION_H
#define ZUMY_VERSION_H

/**
 * network protocol versioning
 */

static const int PROTOCOL_VERSION = 80004;

//! initial proto version, to be increased after version/verack negotiation
static const int INIT_PROTO_VERSION = 219;

//! In this version, 'getheaders' was introduced.
static const int GETHEADERS_VERSION = 80001;

//! disconnect from peers older than this proto version
static const int MIN_PEER_PROTO_VERSION = 80004;

//! nTime field added to CAddress, starting with this version;
//! if possible, avoid requesting addresses nodes older than this
static const int CADDR_TIME_VERSION = 31401;

//! only request blocks from nodes outside this range of versions
static const int NOBLKS_VERSION_START = 32001;
static const int NOBLKS_VERSION_END = 80004;

//! BIP 0031, pong message, is enabled for all versions AFTER this one
static const int BIP0031_VERSION = 80004;

//! "mempool" command, enhanced "getdata" behavior starts with this version
static const int MEMPOOL_GD_VERSION = 80004;

//! "filter*" commands are disabled without NODE_BLOOM after and including this version
static const int NO_BLOOM_VERSION = 70001;

//! "sendheaders" command and announcing blocks with headers starts with this version
static const int SENDHEADERS_VERSION = 80004;

#endif // ZUMY_VERSION_H