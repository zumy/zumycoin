DIST_SUBDIRS = secp256k1 univalue
CFLAGS_FOR_BUILD = -Wall -Wextra -Wno-unused-function

AM_LDFLAGS = $(PTHREAD_CFLAGS) $(LIBTOOL_LDFLAGS) $(HARDENED_LDFLAGS)
AM_CXXFLAGS = $(HARDENED_CXXFLAGS)
AM_CPPFLAGS = $(HARDENED_CPPFLAGS)
EXTRA_LIBRARIES =

ZUMY_CONFIG_INCLUDES=-I$(builddir)/config
ZUMY_INCLUDES=-I$(builddir) -I$(builddir)/obj $(BOOST_CPPFLAGS) $(LEVELDB_CPPFLAGS) $(CRYPTO_CFLAGS) $(SSL_CFLAGS)

ZUMY_INCLUDES += -I$(srcdir)/secp256k1/include
ZUMY_INCLUDES += -I$(srcdir)/univalue/include

LIBZUMY_SERVER=libzumy_server.a
LIBZUMY_WALLET=libzumy_wallet.a
LIBZUMY_COMMON=libzumy_common.a
LIBZUMY_CLI=libzumy_cli.a
LIBZUMY_UTIL=libzumy_util.a
LIBZUMY_CRYPTO=crypto/libzumy_crypto.a
LIBZUMYQT=qt/libzumyqt.a
LIBSECP256K1=secp256k1/libsecp256k1.la
LIBUNIVALUE=univalue/libunivalue.la

$(LIBSECP256K1): $(wildcard secp256k1/src/*) $(wildcard secp256k1/include/*)
	$(AM_V_at)$(MAKE) $(AM_MAKEFLAGS) -C $(@D) $(@F)

$(LIBUNIVALUE): $(wildcard univalue/lib/*) $(wildcard univalue/include/*)
	$(AM_V_at)$(MAKE) $(AM_MAKEFLAGS) -C $(@D) $(@F)

# Make is not made aware of per-object dependencies to avoid limiting building parallelization
# But to build the less dependent modules first, we manually select their order here:
EXTRA_LIBRARIES += \
  crypto/libzumy_crypto.a \
  libzumy_util.a \
  libzumy_common.a \
  libzumy_server.a \
  libzumy_cli.a
if ENABLE_WALLET
ZUMY_INCLUDES += $(BDB_CPPFLAGS)
EXTRA_LIBRARIES += libzumy_wallet.a
endif
if ENABLE_ZMQ
EXTRA_LIBRARIES += libzumy_zmq.a
endif

if BUILD_ZUMY_LIBS
lib_LTLIBRARIES = libzumyconsensus.la
LIBZUMY_CONSENSUS=libzumyconsensus.la
else
LIBZUMY_CONSENSUS=
endif

bin_PROGRAMS =
TESTS =
BENCHMARKS =

if BUILD_ZUMYD
  bin_PROGRAMS += zumyd
endif

if BUILD_ZUMY_UTILS
  bin_PROGRAMS += zumy-cli zumy-tx
endif

.PHONY: FORCE check-symbols check-security
# zumy #
ZUMY_CORE_H = \
  activemasternode.h \
  addressindex.h \
  spentindex.h \
  addrman.h \
  alert.h \
  amount.h \
  arith_uint256.h \
  base58.h \
  bip39_english.h \
  bip39.h \
  bloom.h \
  cachemap.h \
  cachemultimap.h \
  chain.h \
  chainparams.h \
  chainparamsbase.h \
  chainparamsseeds.h \
  checkpoints.h \
  checkqueue.h \
  clientversion.h \
  coincontrol.h \
  coins.h \
  compat.h \
  compat/byteswap.h \
  compat/endian.h \
  compat/sanity.h \
  compressor.h \
  consensus/consensus.h \
  consensus/merkle.h \
  consensus/params.h \
  consensus/validation.h \
  core_io.h \
  core_memusage.h \
  dbwrapper.h \
  masternode.h \
  masternode-payments.h \
  masternode-sync.h \
  masternodeman.h \
  masternodeconfig.h \
  flat-database.h \
  governance.h \
  governance-classes.h \
  governance-exceptions.h \
  governance-object.h \
  governance-vote.h \
  governance-votedb.h \
  hash.h \
  hdchain.h \
  httprpc.h \
  httpserver.h \
  indirectmap.h \
  init.h \
  instantsend.h \
  key.h \
  keepass.h \
  keystore.h \
  limitedmap.h \
  main.h \
  memusage.h \
  merkleblock.h \
  messagesigner.h \
  miner.h \
  net.h \
  netbase.h \
  netfulfilledman.h \
  noui.h \
  policy/fees.h \
  policy/policy.h \
  policy/rbf.h \
  pow.h \
  prevector.h \
  primitives/block.h \
  primitives/transaction.h \
  privatesend.h \
  psnotificationinterface.h \
  privatesend-relay.h \
  protocol.h \
  pubkey.h \
  random.h \
  reverselock.h \
  rpcclient.h \
  rpcprotocol.h \
  rpcserver.h \
  scheduler.h \
  script/interpreter.h \
  script/script.h \
  script/script_error.h \
  script/sigcache.h \
  script/sign.h \
  script/standard.h \
  serialize.h \
  spork.h \
  streams.h \
  support/allocators/secure.h \
  support/allocators/zeroafterfree.h \
  support/cleanse.h \
  support/lockedpool.h \
  sync.h \
  threadsafety.h \
  timedata.h \
  tinyformat.h \
  torcontrol.h \
  txdb.h \
  txmempool.h \
  ui_interface.h \
  uint256.h \
  undo.h \
  util.h \
  utilmoneystr.h \
  utilstrencodings.h \
  utiltime.h \
  validationinterface.h \
  version.h \
  versionbits.h \
  wallet/crypter.h \
  wallet/db.h \
  wallet/wallet.h \
  wallet/wallet_ismine.h \
  wallet/walletdb.h \
  zmq/zmqabstractnotifier.h \
  zmq/zmqconfig.h\
  zmq/zmqnotificationinterface.h \
  zmq/zmqpublishnotifier.h

obj/build.h: FORCE
	@$(MKDIR_P) $(builddir)/obj
	@$(top_srcdir)/share/genbuild.sh $(abs_top_builddir)/src/obj/build.h \
	  $(abs_top_srcdir)
libzumy_util_a-clientversion.$(OBJEXT): obj/build.h

# server: shared between zumyd and zumy-qt
libzumy_server_a_CPPFLAGS = $(AM_CPPFLAGS) $(ZUMY_INCLUDES) $(MINIUPNPC_CPPFLAGS) $(EVENT_CFLAGS) $(EVENT_PTHREADS_CFLAGS)
libzumy_server_a_CXXFLAGS = $(AM_CXXFLAGS) $(PIE_FLAGS)
libzumy_server_a_SOURCES = \
  addrman.cpp \
  alert.cpp \
  bloom.cpp \
  chain.cpp \
  checkpoints.cpp \
  dbwrapper.cpp \
  governance.cpp \
  governance-classes.cpp \
  governance-object.cpp \
  governance-vote.cpp \
  governance-votedb.cpp \
  httprpc.cpp \
  httpserver.cpp \
  init.cpp \
  main.cpp \
  merkleblock.cpp \
  messagesigner.cpp \
  miner.cpp \
  net.cpp \
  netfulfilledman.cpp \
  noui.cpp \
  policy/fees.cpp \
  policy/policy.cpp \
  pow.cpp \
  rest.cpp \
  rpcblockchain.cpp \
  rpcmasternode.cpp \
  rpcgovernance.cpp \
  rpcmining.cpp \
  rpcmisc.cpp \
  rpcnet.cpp \
  rpcrawtransaction.cpp \
  rpcserver.cpp \
  script/sigcache.cpp \
  sendalert.cpp \
  timedata.cpp \
  torcontrol.cpp \
  txdb.cpp \
  txmempool.cpp \
  ui_interface.cpp \
  validationinterface.cpp \
  versionbits.cpp \
  $(ZUMY_CORE_H)

if ENABLE_ZMQ
LIBZUMY_ZMQ=libzumy_zmq.a

libzumy_zmq_a_CPPFLAGS = $(ZUMY_INCLUDES) $(ZMQ_CFLAGS)
libzumy_zmq_a_CXXFLAGS = $(AM_CXXFLAGS) $(PIE_FLAGS)
libzumy_zmq_a_SOURCES = \
  zmq/zmqabstractnotifier.cpp \
  zmq/zmqnotificationinterface.cpp \
  zmq/zmqpublishnotifier.cpp
endif


# wallet: shared between zumyd and zumy-qt, but only linked
# when wallet enabled
libzumy_wallet_a_CPPFLAGS = $(AM_CPPFLAGS) $(ZUMY_INCLUDES)
libzumy_wallet_a_CXXFLAGS = $(AM_CXXFLAGS) $(PIE_FLAGS)
libzumy_wallet_a_SOURCES = \
  activemasternode.cpp \
  masternode.cpp \
  masternode-payments.cpp \
  masternode-sync.cpp \
  masternodeconfig.cpp \
  masternodeman.cpp \
  instantsend.cpp \
  keepass.cpp \
  policy/rbf.cpp \
  privatesend.cpp \
  privatesend-relay.cpp \
  psnotificationinterface.cpp \
  wallet/crypter.cpp \
  wallet/db.cpp \
  wallet/rpcdump.cpp \
  wallet/rpcwallet.cpp \
  wallet/wallet.cpp \
  wallet/wallet_ismine.cpp \
  wallet/walletdb.cpp \
  $(ZUMY_CORE_H)

# crypto primitives library
crypto_libzumy_crypto_a_CPPFLAGS = $(AM_CPPFLAGS) $(ZUMY_CONFIG_INCLUDES) $(PIC_FLAGS)
crypto_libzumy_crypto_a_CXXFLAGS = $(AM_CXXFLAGS) $(PIE_FLAGS) $(PIC_FLAGS)
crypto_libzumy_crypto_a_SOURCES = \
  crypto/common.h \
  crypto/argon2d/argon2.h \
  crypto/argon2d/core.h \
  crypto/argon2d/encoding.h \
  crypto/argon2d/thread.h \
  crypto/argon2d/argon2.c \
  crypto/argon2d/core.c \
  crypto/argon2d/encoding.c \
  crypto/argon2d/opt.c \
  crypto/argon2d/thread.c \
  crypto/blake2/blake2b.c \
  crypto/blake2/blake2.h \
  crypto/blake2/blake2-impl.h \
  crypto/blake2/blamka-round-opt.h \
  crypto/blake2/blamka-round-ref.h \
  crypto/hmac_sha256.cpp \
  crypto/hmac_sha256.h \
  crypto/hmac_sha512.cpp \
  crypto/hmac_sha512.h \
  crypto/ripemd160.cpp \
  crypto/ripemd160.h \
  crypto/sha1.cpp \
  crypto/sha1.h \
  crypto/sha256.cpp \
  crypto/sha256.h \
  crypto/sha512.h \
  crypto/sha512.cpp \
  crypto/sha512.h

# common: shared between zumyd, and zumy-qt and non-server tools
libzumy_common_a_CPPFLAGS = $(AM_CPPFLAGS) $(ZUMY_INCLUDES)
libzumy_common_a_CXXFLAGS = $(AM_CXXFLAGS) $(PIE_FLAGS)
libzumy_common_a_SOURCES = \
  amount.cpp \
  arith_uint256.cpp \
  base58.cpp \
  bip39.cpp \
  chainparams.cpp \
  coins.cpp \
  compressor.cpp \
  consensus/merkle.cpp \
  core_read.cpp \
  core_write.cpp \
  hash.cpp \
  hdchain.cpp \
  key.cpp \
  keystore.cpp \
  netbase.cpp \
  primitives/block.cpp \
  primitives/transaction.cpp \
  protocol.cpp \
  pubkey.cpp \
  scheduler.cpp \
  script/interpreter.cpp \
  script/script.cpp \
  script/script_error.cpp \
  script/sign.cpp \
  script/standard.cpp \
  spork.cpp \
  $(ZUMY_CORE_H)

# util: shared between all executables.
# This library *must* be included to make sure that the glibc
# backward-compatibility objects and their sanity checks are linked.
libzumy_util_a_CPPFLAGS = $(AM_CPPFLAGS) $(ZUMY_INCLUDES)
libzumy_util_a_CXXFLAGS = $(AM_CXXFLAGS) $(PIE_FLAGS)
libzumy_util_a_SOURCES = \
  chainparamsbase.cpp \
  clientversion.cpp \
  compat/glibc_sanity.cpp \
  compat/glibcxx_sanity.cpp \
  compat/strnlen.cpp \
  random.cpp \
  rpcprotocol.cpp \
  support/cleanse.cpp \
  support/lockedpool.cpp \
  sync.cpp \
  uint256.cpp \
  util.cpp \
  utilmoneystr.cpp \
  utilstrencodings.cpp \
  utiltime.cpp \
  $(ZUMY_CORE_H)

if GLIBC_BACK_COMPAT
libzumy_util_a_SOURCES += compat/glibc_compat.cpp
endif

# cli: shared between zumy-cli and zumy-qt
libzumy_cli_a_CPPFLAGS = $(AM_CPPFLAGS) $(ZUMY_INCLUDES)
libzumy_cli_a_CXXFLAGS = $(AM_CXXFLAGS) $(PIE_FLAGS)
libzumy_cli_a_SOURCES = \
  rpcclient.cpp \
  $(ZUMY_CORE_H)

nodist_libzumy_util_a_SOURCES = $(srcdir)/obj/build.h
#

# zumyd binary #
zumyd_SOURCES = zumyd.cpp
zumyd_CPPFLAGS = $(AM_CPPFLAGS) $(ZUMY_INCLUDES)
zumyd_CXXFLAGS = $(AM_CXXFLAGS) $(PIE_FLAGS)
zumyd_LDFLAGS = $(RELDFLAGS) $(AM_LDFLAGS) $(LIBTOOL_APP_LDFLAGS)

if TARGET_WINDOWS
zumyd_SOURCES += zumyd-res.rc
endif

zumyd_LDADD = \
  $(LIBZUMY_SERVER) \
  $(LIBZUMY_COMMON) \
  $(LIBUNIVALUE) \
  $(LIBZUMY_UTIL) \
  $(LIBZUMY_CRYPTO) \
  $(LIBLEVELDB) \
  $(LIBMEMENV) \
  $(LIBSECP256K1)

if ENABLE_ZMQ
zumyd_LDADD += $(LIBZUMY_ZMQ) $(ZMQ_LIBS)
endif

if ENABLE_WALLET
zumyd_LDADD += libzumy_wallet.a
endif

zumyd_LDADD += $(BOOST_LIBS) $(BDB_LIBS) $(SSL_LIBS) $(CRYPTO_LIBS) $(MINIUPNPC_LIBS) $(EVENT_PTHREADS_LIBS) $(EVENT_LIBS)

# zumy-cli binary #
zumy_cli_SOURCES = zumy-cli.cpp
zumy_cli_CPPFLAGS = $(AM_CPPFLAGS) $(ZUMY_INCLUDES) $(EVENT_CFLAGS)
zumy_cli_CXXFLAGS = $(AM_CXXFLAGS) $(PIE_FLAGS)
zumy_cli_LDFLAGS = $(RELDFLAGS) $(AM_LDFLAGS) $(LIBTOOL_APP_LDFLAGS)

if TARGET_WINDOWS
zumy_cli_SOURCES += zumy-cli-res.rc
endif

zumy_cli_LDADD = \
  $(LIBZUMY_CLI) \
  $(LIBUNIVALUE) \
  $(LIBZUMY_UTIL)
zumy_cli_LDADD += $(BOOST_LIBS) $(SSL_LIBS) $(CRYPTO_LIBS) $(EVENT_LIBS)
#

# zumy-tx binary #
zumy_tx_SOURCES = zumy-tx.cpp
zumy_tx_CPPFLAGS = $(AM_CPPFLAGS) $(ZUMY_INCLUDES)
zumy_tx_CXXFLAGS = $(AM_CXXFLAGS) $(PIE_FLAGS)
zumy_tx_LDFLAGS = $(RELDFLAGS) $(AM_LDFLAGS) $(LIBTOOL_APP_LDFLAGS)

if TARGET_WINDOWS
zumy_tx_SOURCES += zumy-tx-res.rc
endif

zumy_tx_LDADD = \
  $(LIBUNIVALUE) \
  $(LIBZUMY_COMMON) \
  $(LIBZUMY_UTIL) \
  $(LIBZUMY_CRYPTO) \
  $(LIBSECP256K1)

zumy_tx_LDADD += $(BOOST_LIBS) $(CRYPTO_LIBS)
#

# zumyconsensus library #
if BUILD_ZUMY_LIBS
include_HEADERS = script/zumyconsensus.h
libzumyconsensus_la_SOURCES = \
  crypto/hmac_sha512.cpp \
  crypto/ripemd160.cpp \
  crypto/sha1.cpp \
  crypto/sha256.cpp \
  crypto/sha512.cpp \
  hash.cpp \
  primitives/transaction.cpp \
  pubkey.cpp \
  script/zumyconsensus.cpp \
  script/interpreter.cpp \
  script/script.cpp \
  uint256.cpp \
  utilstrencodings.cpp

if GLIBC_BACK_COMPAT
  libzumyconsensus_la_SOURCES += compat/glibc_compat.cpp
endif

libzumyconsensus_la_LDFLAGS = $(AM_LDFLAGS) -no-undefined $(RELDFLAGS)
libzumyconsensus_la_LIBADD = $(LIBSECP256K1)
libzumyconsensus_la_CPPFLAGS = $(AM_CPPFLAGS) -I$(builddir)/obj -I$(srcdir)/secp256k1/include -DBUILD_ZUMY_INTERNAL
libzumyconsensus_la_CXXFLAGS = $(AM_CXXFLAGS) $(PIE_FLAGS)

endif
#

CLEANFILES = leveldb/libleveldb.a leveldb/libmemenv.a
CLEANFILES += *.gcda *.gcno
CLEANFILES += compat/*.gcda compat/*.gcno
CLEANFILES += consensus/*.gcda consensus/*.gcno
CLEANFILES += crypto/*.gcda crypto/*.gcno
CLEANFILES += policy/*.gcda policy/*.gcno
CLEANFILES += primitives/*.gcda primitives/*.gcno
CLEANFILES += script/*.gcda script/*.gcno
CLEANFILES += support/*.gcda support/*.gcno
CLEANFILES += univalue/*.gcda univalue/*.gcno
CLEANFILES += wallet/*.gcda wallet/*.gcno
CLEANFILES += wallet/test/*.gcda wallet/test/*.gcno
CLEANFILES += zmq/*.gcda zmq/*.gcno

DISTCLEANFILES = obj/build.h

EXTRA_DIST = leveldb

clean-local:
	-$(MAKE) -C leveldb clean
	-$(MAKE) -C secp256k1 clean
	-$(MAKE) -C univalue clean
	-rm -f leveldb/*/*.gcda leveldb/*/*.gcno leveldb/helpers/memenv/*.gcda leveldb/helpers/memenv/*.gcno
	-rm -f config/zumy-config.* config/stamp-h1

.rc.o:
	@test -f $(WINDRES)
	$(AM_V_GEN) $(WINDRES) -DWINDRES_PREPROC -i $< -o $@

.mm.o:
	$(AM_V_CXX) $(OBJCXX) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) \
	  $(CPPFLAGS) $(AM_CXXFLAGS) $(QT_INCLUDES) $(AM_CXXFLAGS) $(PIE_FLAGS) $(CXXFLAGS) -c -o $@ $<

check-symbols: $(bin_PROGRAMS)
if GLIBC_BACK_COMPAT
	@echo "Checking glibc back compat..."
	$(AM_V_at) READELF=$(READELF) CPPFILT=$(CPPFILT) $(top_srcdir)/contrib/devtools/symbol-check.py < $(bin_PROGRAMS)
endif

check-security: $(bin_PROGRAMS)
if HARDEN
	@echo "Checking binary security..."
	$(AM_V_at) READELF=$(READELF) OBJDUMP=$(OBJDUMP) $(top_srcdir)/contrib/devtools/security-check.py < $(bin_PROGRAMS)
endif

%.pb.cc %.pb.h: %.proto
	@test -f $(PROTOC)
	$(AM_V_GEN) $(PROTOC) --cpp_out=$(@D) --proto_path=$(abspath $(<D) $<)

if EMBEDDED_LEVELDB
include Makefile.leveldb.include
endif

if ENABLE_TESTS
include Makefile.test.include
endif

if ENABLE_BENCH
include Makefile.bench.include
endif

if ENABLE_QT
include Makefile.qt.include
endif

if ENABLE_QT_TESTS
include Makefile.qttest.include
endif
