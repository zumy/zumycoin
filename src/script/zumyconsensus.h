// Copyright (c) 2009-2018 Satoshi Nakamoto
// Copyright (c) 2009-2018 The Bitcoin Developers
// Copyright (c) 2014-2018 The Dash Core Developers
// Copyright (c) 2017-2018 Zumy Developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef ZUMY_ZUMYCONSENSUS_H
#define ZUMY_ZUMYCONSENSUS_H

#if defined(BUILD_ZUMY_INTERNAL) && defined(HAVE_CONFIG_H)
#include "config/zumy-config.h"
  #if defined(_WIN32)
    #if defined(DLL_EXPORT)
      #if defined(HAVE_FUNC_ATTRIBUTE_DLLEXPORT)
        #define EXPORT_SYMBOL __declspec(dllexport)
      #else
        #define EXPORT_SYMBOL
      #endif
    #endif
  #elif defined(HAVE_FUNC_ATTRIBUTE_VISIBILITY)
    #define EXPORT_SYMBOL __attribute__ ((visibility ("default")))
  #endif
#elif defined(MSC_VER) && !defined(STATIC_LIBZUMYCONSENSUS)
  #define EXPORT_SYMBOL __declspec(dllimport)
#endif

#ifndef EXPORT_SYMBOL
  #define EXPORT_SYMBOL
#endif

#ifdef __cplusplus
extern "C" {
#endif

#define ZUMYCONSENSUS_API_VER 0

typedef enum zumyconsensus_error_t
{
    zumyconsensus_ERR_OK = 0,
    zumyconsensus_ERR_TX_INDEX,
    zumyconsensus_ERR_TX_SIZE_MISMATCH,
    zumyconsensus_ERR_TX_DESERIALIZE,
    zumyconsensus_ERR_INVALID_FLAGS,
} zumyconsensus_error;

/** Script verification flags */
enum
{
    zumyconsensus_SCRIPT_FLAGS_VERIFY_NONE                = 0,
    zumyconsensus_SCRIPT_FLAGS_VERIFY_P2SH                = (1U << 0), // evaluate P2SH (BIP16) subscripts
    zumyconsensus_SCRIPT_FLAGS_VERIFY_DERSIG              = (1U << 2), // enforce strict DER (BIP66) compliance
    zumyconsensus_SCRIPT_FLAGS_VERIFY_CHECKLOCKTIMEVERIFY = (1U << 9), // enable CHECKLOCKTIMEVERIFY (BIP65)
    zumyconsensus_SCRIPT_FLAGS_VERIFY_CHECKSEQUENCEVERIFY = (1U << 10), // enable CHECKSEQUENCEVERIFY (BIP112)
    zumyconsensus_SCRIPT_FLAGS_VERIFY_ALL                 = zumyconsensus_SCRIPT_FLAGS_VERIFY_P2SH | zumyconsensus_SCRIPT_FLAGS_VERIFY_DERSIG |
                                                               zumyconsensus_SCRIPT_FLAGS_VERIFY_CHECKLOCKTIMEVERIFY | zumyconsensus_SCRIPT_FLAGS_VERIFY_CHECKSEQUENCEVERIFY
};

/// Returns 1 if the input nIn of the serialized transaction pointed to by
/// txTo correctly spends the scriptPubKey pointed to by scriptPubKey under
/// the additional constraints specified by flags.
/// If not NULL, err will contain an error/success code for the operation
EXPORT_SYMBOL int zumyconsensus_verify_script(const unsigned char *scriptPubKey, unsigned int scriptPubKeyLen,
                                    const unsigned char *txTo        , unsigned int txToLen,
                                    unsigned int nIn, unsigned int flags, zumyconsensus_error* err);

EXPORT_SYMBOL unsigned int zumyconsensus_version();

#ifdef __cplusplus
} // extern "C"
#endif

#undef EXPORT_SYMBOL

#endif // ZUMY_ZUMYCONSENSUS_H
