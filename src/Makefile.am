lib_LTLIBRARIES = \
	libsqrl.la

libsqrl_la_SOURCES = \
			aes.cpp \
			gcm.cpp \
			rdrand.cpp \
			realtime.cpp \
			util.cpp   \
			SqrlAction.cpp \
			SqrlActionChangePassword.cpp \
			SqrlActionDisable.cpp \
			SqrlActionEnable.cpp \
			SqrlActionGenerate.cpp \
			SqrlActionIdent.cpp \
			SqrlActionLock.cpp \
			SqrlActionRekey.cpp \
			SqrlActionRemove.cpp \
			SqrlActionRescue.cpp \
			SqrlActionSave.cpp \
			SqrlBase56.cpp \
			SqrlBase56Check.cpp \
			SqrlBase64.cpp \
			SqrlBlock.cpp \
			SqrlClient.cpp \
			SqrlClientAsync.cpp \
			SqrlCrypt.cpp \
			SqrlEnScrypt.cpp \
			SqrlEncoder.cpp \
			SqrlEntropy.cpp \
			SqrlIdentityAction.cpp \
			SqrlKeySet.cpp \
			SqrlServer.cpp \
			SqrlSiteAction.cpp \
			SqrlStorage.cpp \
			SqrlUri.cpp \
			SqrlUrlEncode.cpp \
			SqrlUser.cpp \
			SqrlUser_storage.cpp 


libsqrl_la_LDFLAGS = \
	$(AM_LDFLAGS) \
	-export-dynamic \
	$(LIBTOOL_EXTRA_FLAGS)

libsqrl_la_CPPFLAGS = \
   	$(LTDLINCL) \
	-I$(top_srcdir)/include \
	-I$(top_srcdir)/include/sqrl \
	-I$(top_srcdir)/libsodium/src/libsodium/crypto_pwhash/scryptsalsa208sha256 \
	-I$(top_builddir)/include/sqrl

