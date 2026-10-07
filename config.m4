PHP_ARG_WITH([secp256k1],
  [for secp256k1 support],
  [AS_HELP_STRING([--with-secp256k1],
    [Include secp256k1 support])])

AS_VAR_IF([PHP_SECP256K1], [no],, [
  PKG_CHECK_MODULES([LIBSECP256K1], [libsecp256k1 >= 0.2.0])

  SECP256K1_LIB_VERSION=$($PKG_CONFIG --modversion libsecp256k1)
  AC_DEFINE_UNQUOTED([SECP256K1_LIB_VERSION], ["$SECP256K1_LIB_VERSION"],
    [Version of the libsecp256k1 library detected at configure time.])

  PHP_EVAL_INCLINE([$LIBSECP256K1_CFLAGS])
  PHP_EVAL_LIBLINE([$LIBSECP256K1_LIBS], [SECP256K1_SHARED_LIBADD])

  PHP_SECP256K1_SOURCES="secp256k1_module.c ext_secp256k1.c"

  PHP_CHECK_LIBRARY([secp256k1], [secp256k1_ecdh],
    [AC_DEFINE([HAVE_SECP256K1_ECDH], [1],
      [Define to 1 if libsecp256k1 has the ECDH module.])
     PHP_SECP256K1_SOURCES="$PHP_SECP256K1_SOURCES ext_secp256k1_ecdh.c"],
    [],
    [$LIBSECP256K1_LIBS])

  PHP_CHECK_LIBRARY([secp256k1], [secp256k1_xonly_pubkey_parse],
    [AC_DEFINE([HAVE_SECP256K1_EXTRAKEYS], [1],
      [Define to 1 if libsecp256k1 has the extrakeys module.])
     PHP_SECP256K1_SOURCES="$PHP_SECP256K1_SOURCES ext_secp256k1_extrakeys.c"],
    [],
    [$LIBSECP256K1_LIBS])

  PHP_CHECK_LIBRARY([secp256k1], [secp256k1_ecdsa_sign_recoverable],
    [AC_DEFINE([HAVE_SECP256K1_RECOVERY], [1],
      [Define to 1 if libsecp256k1 has the recovery module.])
     PHP_SECP256K1_SOURCES="$PHP_SECP256K1_SOURCES ext_secp256k1_recovery.c"],
    [],
    [$LIBSECP256K1_LIBS])

  PHP_CHECK_LIBRARY([secp256k1], [secp256k1_schnorrsig_sign32],
    [AC_DEFINE([HAVE_SECP256K1_SCHNORRSIG], [1],
      [Define to 1 if libsecp256k1 has the schnorrsig module.])
     PHP_SECP256K1_SOURCES="$PHP_SECP256K1_SOURCES ext_secp256k1_schnorrsig.c"],
    [],
    [$LIBSECP256K1_LIBS])

  PHP_CHECK_LIBRARY([secp256k1], [secp256k1_ellswift_encode],
    [AC_DEFINE([HAVE_SECP256K1_ELLSWIFT], [1],
      [Define to 1 if libsecp256k1 has the ellswift module.])
     PHP_SECP256K1_SOURCES="$PHP_SECP256K1_SOURCES ext_secp256k1_ellswift.c"],
    [],
    [$LIBSECP256K1_LIBS])

  PHP_CHECK_LIBRARY([secp256k1], [secp256k1_silentpayments_recipient_label_create],
    [AC_DEFINE([HAVE_SECP256K1_SILENTPAYMENTS], [1],
      [Define to 1 if libsecp256k1 has the silentpayments module.])
     PHP_SECP256K1_SOURCES="$PHP_SECP256K1_SOURCES ext_secp256k1_silentpayments.c"],
    [],
    [$LIBSECP256K1_LIBS])

  PHP_SUBST([SECP256K1_SHARED_LIBADD])

  AC_DEFINE([HAVE_SECP256K1], [1],
    [Define to 1 if the PHP extension 'secp256k1' is available.])

  PHP_NEW_EXTENSION([secp256k1],
    [$PHP_SECP256K1_SOURCES],
    [$ext_shared],,
    [-DZEND_ENABLE_STATIC_TSRMLS_CACHE=1])
])
