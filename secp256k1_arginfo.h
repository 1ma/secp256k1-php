/* This is a generated file, edit the .stub.php file instead.
 * Stub hash: 1238747aa3a2961f2047b47b6ce3ef0f63673f1b */

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_secp256k1_ec_seckey_verify, 0, 1, _IS_BOOL, 0)
	ZEND_ARG_TYPE_INFO(0, seckey32, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_TYPE_MASK_EX(arginfo_secp256k1_ec_pubkey_create, 0, 1, secp256k1_pubkey, MAY_BE_FALSE)
	ZEND_ARG_TYPE_INFO(0, seckey32, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_TYPE_MASK_EX(arginfo_secp256k1_ec_pubkey_parse, 0, 1, secp256k1_pubkey, MAY_BE_FALSE)
	ZEND_ARG_TYPE_INFO(0, pubkey, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_secp256k1_ec_pubkey_serialize, 0, 1, IS_STRING, 0)
	ZEND_ARG_OBJ_INFO(0, pubkey, secp256k1_pubkey, 0)
	ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(0, flags, IS_LONG, 0, "SECP256K1_EC_COMPRESSED")
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_TYPE_MASK_EX(arginfo_secp256k1_ecdsa_signature_parse_compact, 0, 1, secp256k1_ecdsa_signature, MAY_BE_FALSE)
	ZEND_ARG_TYPE_INFO(0, sig64, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_TYPE_MASK_EX(arginfo_secp256k1_ecdsa_signature_parse_der, 0, 1, secp256k1_ecdsa_signature, MAY_BE_FALSE)
	ZEND_ARG_TYPE_INFO(0, der, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_secp256k1_ecdsa_signature_serialize_compact, 0, 1, IS_STRING, 0)
	ZEND_ARG_OBJ_INFO(0, sig, secp256k1_ecdsa_signature, 0)
ZEND_END_ARG_INFO()

#define arginfo_secp256k1_ecdsa_signature_serialize_der arginfo_secp256k1_ecdsa_signature_serialize_compact

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_secp256k1_ecdsa_signature_normalize, 0, 1, _IS_BOOL, 0)
	ZEND_ARG_OBJ_INFO(1, sig, secp256k1_ecdsa_signature, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_TYPE_MASK_EX(arginfo_secp256k1_ecdsa_sign, 0, 2, secp256k1_ecdsa_signature, MAY_BE_FALSE)
	ZEND_ARG_TYPE_INFO(0, msghash32, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(0, seckey32, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_secp256k1_ecdsa_verify, 0, 3, _IS_BOOL, 0)
	ZEND_ARG_OBJ_INFO(0, sig, secp256k1_ecdsa_signature, 0)
	ZEND_ARG_TYPE_INFO(0, msghash32, IS_STRING, 0)
	ZEND_ARG_OBJ_INFO(0, pubkey, secp256k1_pubkey, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_MASK_EX(arginfo_secp256k1_ec_seckey_negate, 0, 1, MAY_BE_STRING|MAY_BE_FALSE)
	ZEND_ARG_TYPE_INFO(0, seckey32, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_MASK_EX(arginfo_secp256k1_ec_seckey_tweak_add, 0, 2, MAY_BE_STRING|MAY_BE_FALSE)
	ZEND_ARG_TYPE_INFO(0, seckey32, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(0, tweak32, IS_STRING, 0)
ZEND_END_ARG_INFO()

#define arginfo_secp256k1_ec_seckey_tweak_mul arginfo_secp256k1_ec_seckey_tweak_add

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_secp256k1_ec_pubkey_negate, 0, 1, IS_VOID, 0)
	ZEND_ARG_OBJ_INFO(1, pubkey, secp256k1_pubkey, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_secp256k1_ec_pubkey_tweak_add, 0, 2, _IS_BOOL, 0)
	ZEND_ARG_OBJ_INFO(1, pubkey, secp256k1_pubkey, 0)
	ZEND_ARG_TYPE_INFO(0, tweak32, IS_STRING, 0)
ZEND_END_ARG_INFO()

#define arginfo_secp256k1_ec_pubkey_tweak_mul arginfo_secp256k1_ec_pubkey_tweak_add

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_TYPE_MASK_EX(arginfo_secp256k1_ec_pubkey_combine, 0, 1, secp256k1_pubkey, MAY_BE_FALSE)
	ZEND_ARG_TYPE_INFO(0, pubkeys, IS_ARRAY, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_secp256k1_ec_pubkey_cmp, 0, 2, IS_LONG, 0)
	ZEND_ARG_OBJ_INFO(0, pubkey1, secp256k1_pubkey, 0)
	ZEND_ARG_OBJ_INFO(0, pubkey2, secp256k1_pubkey, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_secp256k1_tagged_sha256, 0, 2, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(0, tag, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(0, msg, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_secp256k1_context_randomize, 0, 0, _IS_BOOL, 0)
ZEND_END_ARG_INFO()

#if defined(HAVE_SECP256K1_ECDH)
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_MASK_EX(arginfo_secp256k1_ecdh, 0, 2, MAY_BE_STRING|MAY_BE_FALSE)
	ZEND_ARG_OBJ_INFO(0, pubkey, secp256k1_pubkey, 0)
	ZEND_ARG_TYPE_INFO(0, seckey32, IS_STRING, 0)
ZEND_END_ARG_INFO()
#endif

#if defined(HAVE_SECP256K1_EXTRAKEYS)
ZEND_BEGIN_ARG_WITH_RETURN_OBJ_TYPE_MASK_EX(arginfo_secp256k1_xonly_pubkey_parse, 0, 1, secp256k1_xonly_pubkey, MAY_BE_FALSE)
	ZEND_ARG_TYPE_INFO(0, input32, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_secp256k1_xonly_pubkey_serialize, 0, 1, IS_STRING, 0)
	ZEND_ARG_OBJ_INFO(0, pubkey, secp256k1_xonly_pubkey, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_secp256k1_xonly_pubkey_cmp, 0, 2, IS_LONG, 0)
	ZEND_ARG_OBJ_INFO(0, pk1, secp256k1_xonly_pubkey, 0)
	ZEND_ARG_OBJ_INFO(0, pk2, secp256k1_xonly_pubkey, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_secp256k1_xonly_pubkey_from_pubkey, 0, 2, secp256k1_xonly_pubkey, 0)
	ZEND_ARG_OBJ_INFO(0, pubkey, secp256k1_pubkey, 0)
	ZEND_ARG_TYPE_INFO(1, parity, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_TYPE_MASK_EX(arginfo_secp256k1_xonly_pubkey_tweak_add, 0, 2, secp256k1_pubkey, MAY_BE_FALSE)
	ZEND_ARG_OBJ_INFO(0, pubkey, secp256k1_xonly_pubkey, 0)
	ZEND_ARG_TYPE_INFO(0, tweak32, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_secp256k1_xonly_pubkey_tweak_add_check, 0, 4, _IS_BOOL, 0)
	ZEND_ARG_TYPE_INFO(0, tweaked_pubkey32, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(0, tweaked_pk_parity, IS_LONG, 0)
	ZEND_ARG_OBJ_INFO(0, internal_pubkey, secp256k1_xonly_pubkey, 0)
	ZEND_ARG_TYPE_INFO(0, tweak32, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_TYPE_MASK_EX(arginfo_secp256k1_keypair_create, 0, 1, secp256k1_keypair, MAY_BE_FALSE)
	ZEND_ARG_TYPE_INFO(0, seckey32, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_secp256k1_keypair_pub, 0, 1, secp256k1_pubkey, 0)
	ZEND_ARG_OBJ_INFO(0, keypair, secp256k1_keypair, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_secp256k1_keypair_xonly_pub, 0, 2, secp256k1_xonly_pubkey, 0)
	ZEND_ARG_OBJ_INFO(0, keypair, secp256k1_keypair, 0)
	ZEND_ARG_TYPE_INFO(1, parity, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_secp256k1_keypair_sec, 0, 1, IS_STRING, 0)
	ZEND_ARG_OBJ_INFO(0, keypair, secp256k1_keypair, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_secp256k1_keypair_xonly_tweak_add, 0, 2, _IS_BOOL, 0)
	ZEND_ARG_OBJ_INFO(1, keypair, secp256k1_keypair, 0)
	ZEND_ARG_TYPE_INFO(0, tweak32, IS_STRING, 0)
ZEND_END_ARG_INFO()
#endif

#if defined(HAVE_SECP256K1_RECOVERY)
ZEND_BEGIN_ARG_WITH_RETURN_OBJ_TYPE_MASK_EX(arginfo_secp256k1_ecdsa_recoverable_signature_parse_compact, 0, 2, secp256k1_ecdsa_recoverable_signature, MAY_BE_FALSE)
	ZEND_ARG_TYPE_INFO(0, sig64, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(0, recid, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_secp256k1_ecdsa_recoverable_signature_serialize_compact, 0, 2, IS_STRING, 0)
	ZEND_ARG_OBJ_INFO(0, sig, secp256k1_ecdsa_recoverable_signature, 0)
	ZEND_ARG_TYPE_INFO(1, recid, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_secp256k1_ecdsa_recoverable_signature_convert, 0, 1, secp256k1_ecdsa_signature, 0)
	ZEND_ARG_OBJ_INFO(0, sig, secp256k1_ecdsa_recoverable_signature, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_TYPE_MASK_EX(arginfo_secp256k1_ecdsa_sign_recoverable, 0, 2, secp256k1_ecdsa_recoverable_signature, MAY_BE_FALSE)
	ZEND_ARG_TYPE_INFO(0, msghash32, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(0, seckey32, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_TYPE_MASK_EX(arginfo_secp256k1_ecdsa_recover, 0, 2, secp256k1_pubkey, MAY_BE_FALSE)
	ZEND_ARG_OBJ_INFO(0, sig, secp256k1_ecdsa_recoverable_signature, 0)
	ZEND_ARG_TYPE_INFO(0, msghash32, IS_STRING, 0)
ZEND_END_ARG_INFO()
#endif

#if defined(HAVE_SECP256K1_SCHNORRSIG)
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_MASK_EX(arginfo_secp256k1_schnorrsig_sign32, 0, 2, MAY_BE_STRING|MAY_BE_FALSE)
	ZEND_ARG_TYPE_INFO(0, msghash32, IS_STRING, 0)
	ZEND_ARG_OBJ_INFO(0, keypair, secp256k1_keypair, 0)
	ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(0, aux_rand32, IS_STRING, 1, "null")
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_MASK_EX(arginfo_secp256k1_schnorrsig_sign_custom, 0, 2, MAY_BE_STRING|MAY_BE_FALSE)
	ZEND_ARG_TYPE_INFO(0, msg, IS_STRING, 0)
	ZEND_ARG_OBJ_INFO(0, keypair, secp256k1_keypair, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_secp256k1_schnorrsig_verify, 0, 3, _IS_BOOL, 0)
	ZEND_ARG_TYPE_INFO(0, sig64, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(0, msg, IS_STRING, 0)
	ZEND_ARG_OBJ_INFO(0, pubkey, secp256k1_xonly_pubkey, 0)
ZEND_END_ARG_INFO()
#endif

#if defined(HAVE_SECP256K1_ELLSWIFT)
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_secp256k1_ellswift_encode, 0, 2, IS_STRING, 0)
	ZEND_ARG_OBJ_INFO(0, pubkey, secp256k1_pubkey, 0)
	ZEND_ARG_TYPE_INFO(0, rnd32, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_secp256k1_ellswift_decode, 0, 1, secp256k1_pubkey, 0)
	ZEND_ARG_TYPE_INFO(0, ell64, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_MASK_EX(arginfo_secp256k1_ellswift_create, 0, 1, MAY_BE_STRING|MAY_BE_FALSE)
	ZEND_ARG_TYPE_INFO(0, seckey32, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(0, auxrnd32, IS_STRING, 1, "null")
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_MASK_EX(arginfo_secp256k1_ellswift_xdh, 0, 4, MAY_BE_STRING|MAY_BE_FALSE)
	ZEND_ARG_TYPE_INFO(0, ell_a64, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(0, ell_b64, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(0, seckey32, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(0, party, _IS_BOOL, 0)
	ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(0, hashfn, IS_LONG, 0, "SECP256K1_ELLSWIFT_XDH_HASH_BIP324")
	ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(0, prefix64, IS_STRING, 1, "null")
ZEND_END_ARG_INFO()
#endif

ZEND_FUNCTION(secp256k1_ec_seckey_verify);
ZEND_FUNCTION(secp256k1_ec_pubkey_create);
ZEND_FUNCTION(secp256k1_ec_pubkey_parse);
ZEND_FUNCTION(secp256k1_ec_pubkey_serialize);
ZEND_FUNCTION(secp256k1_ecdsa_signature_parse_compact);
ZEND_FUNCTION(secp256k1_ecdsa_signature_parse_der);
ZEND_FUNCTION(secp256k1_ecdsa_signature_serialize_compact);
ZEND_FUNCTION(secp256k1_ecdsa_signature_serialize_der);
ZEND_FUNCTION(secp256k1_ecdsa_signature_normalize);
ZEND_FUNCTION(secp256k1_ecdsa_sign);
ZEND_FUNCTION(secp256k1_ecdsa_verify);
ZEND_FUNCTION(secp256k1_ec_seckey_negate);
ZEND_FUNCTION(secp256k1_ec_seckey_tweak_add);
ZEND_FUNCTION(secp256k1_ec_seckey_tweak_mul);
ZEND_FUNCTION(secp256k1_ec_pubkey_negate);
ZEND_FUNCTION(secp256k1_ec_pubkey_tweak_add);
ZEND_FUNCTION(secp256k1_ec_pubkey_tweak_mul);
ZEND_FUNCTION(secp256k1_ec_pubkey_combine);
ZEND_FUNCTION(secp256k1_ec_pubkey_cmp);
ZEND_FUNCTION(secp256k1_tagged_sha256);
ZEND_FUNCTION(secp256k1_context_randomize);
#if defined(HAVE_SECP256K1_ECDH)
ZEND_FUNCTION(secp256k1_ecdh);
#endif
#if defined(HAVE_SECP256K1_EXTRAKEYS)
ZEND_FUNCTION(secp256k1_xonly_pubkey_parse);
ZEND_FUNCTION(secp256k1_xonly_pubkey_serialize);
ZEND_FUNCTION(secp256k1_xonly_pubkey_cmp);
ZEND_FUNCTION(secp256k1_xonly_pubkey_from_pubkey);
ZEND_FUNCTION(secp256k1_xonly_pubkey_tweak_add);
ZEND_FUNCTION(secp256k1_xonly_pubkey_tweak_add_check);
ZEND_FUNCTION(secp256k1_keypair_create);
ZEND_FUNCTION(secp256k1_keypair_pub);
ZEND_FUNCTION(secp256k1_keypair_xonly_pub);
ZEND_FUNCTION(secp256k1_keypair_sec);
ZEND_FUNCTION(secp256k1_keypair_xonly_tweak_add);
#endif
#if defined(HAVE_SECP256K1_RECOVERY)
ZEND_FUNCTION(secp256k1_ecdsa_recoverable_signature_parse_compact);
ZEND_FUNCTION(secp256k1_ecdsa_recoverable_signature_serialize_compact);
ZEND_FUNCTION(secp256k1_ecdsa_recoverable_signature_convert);
ZEND_FUNCTION(secp256k1_ecdsa_sign_recoverable);
ZEND_FUNCTION(secp256k1_ecdsa_recover);
#endif
#if defined(HAVE_SECP256K1_SCHNORRSIG)
ZEND_FUNCTION(secp256k1_schnorrsig_sign32);
ZEND_FUNCTION(secp256k1_schnorrsig_sign_custom);
ZEND_FUNCTION(secp256k1_schnorrsig_verify);
#endif
#if defined(HAVE_SECP256K1_ELLSWIFT)
ZEND_FUNCTION(secp256k1_ellswift_encode);
ZEND_FUNCTION(secp256k1_ellswift_decode);
ZEND_FUNCTION(secp256k1_ellswift_create);
ZEND_FUNCTION(secp256k1_ellswift_xdh);
#endif

static const zend_function_entry ext_functions[] = {
	ZEND_FE(secp256k1_ec_seckey_verify, arginfo_secp256k1_ec_seckey_verify)
	ZEND_FE(secp256k1_ec_pubkey_create, arginfo_secp256k1_ec_pubkey_create)
	ZEND_FE(secp256k1_ec_pubkey_parse, arginfo_secp256k1_ec_pubkey_parse)
	ZEND_FE(secp256k1_ec_pubkey_serialize, arginfo_secp256k1_ec_pubkey_serialize)
	ZEND_FE(secp256k1_ecdsa_signature_parse_compact, arginfo_secp256k1_ecdsa_signature_parse_compact)
	ZEND_FE(secp256k1_ecdsa_signature_parse_der, arginfo_secp256k1_ecdsa_signature_parse_der)
	ZEND_FE(secp256k1_ecdsa_signature_serialize_compact, arginfo_secp256k1_ecdsa_signature_serialize_compact)
	ZEND_FE(secp256k1_ecdsa_signature_serialize_der, arginfo_secp256k1_ecdsa_signature_serialize_der)
	ZEND_FE(secp256k1_ecdsa_signature_normalize, arginfo_secp256k1_ecdsa_signature_normalize)
	ZEND_FE(secp256k1_ecdsa_sign, arginfo_secp256k1_ecdsa_sign)
	ZEND_FE(secp256k1_ecdsa_verify, arginfo_secp256k1_ecdsa_verify)
	ZEND_FE(secp256k1_ec_seckey_negate, arginfo_secp256k1_ec_seckey_negate)
	ZEND_FE(secp256k1_ec_seckey_tweak_add, arginfo_secp256k1_ec_seckey_tweak_add)
	ZEND_FE(secp256k1_ec_seckey_tweak_mul, arginfo_secp256k1_ec_seckey_tweak_mul)
	ZEND_FE(secp256k1_ec_pubkey_negate, arginfo_secp256k1_ec_pubkey_negate)
	ZEND_FE(secp256k1_ec_pubkey_tweak_add, arginfo_secp256k1_ec_pubkey_tweak_add)
	ZEND_FE(secp256k1_ec_pubkey_tweak_mul, arginfo_secp256k1_ec_pubkey_tweak_mul)
	ZEND_FE(secp256k1_ec_pubkey_combine, arginfo_secp256k1_ec_pubkey_combine)
	ZEND_FE(secp256k1_ec_pubkey_cmp, arginfo_secp256k1_ec_pubkey_cmp)
	ZEND_FE(secp256k1_tagged_sha256, arginfo_secp256k1_tagged_sha256)
	ZEND_FE(secp256k1_context_randomize, arginfo_secp256k1_context_randomize)
#if defined(HAVE_SECP256K1_ECDH)
	ZEND_FE(secp256k1_ecdh, arginfo_secp256k1_ecdh)
#endif
#if defined(HAVE_SECP256K1_EXTRAKEYS)
	ZEND_FE(secp256k1_xonly_pubkey_parse, arginfo_secp256k1_xonly_pubkey_parse)
	ZEND_FE(secp256k1_xonly_pubkey_serialize, arginfo_secp256k1_xonly_pubkey_serialize)
	ZEND_FE(secp256k1_xonly_pubkey_cmp, arginfo_secp256k1_xonly_pubkey_cmp)
	ZEND_FE(secp256k1_xonly_pubkey_from_pubkey, arginfo_secp256k1_xonly_pubkey_from_pubkey)
	ZEND_FE(secp256k1_xonly_pubkey_tweak_add, arginfo_secp256k1_xonly_pubkey_tweak_add)
	ZEND_FE(secp256k1_xonly_pubkey_tweak_add_check, arginfo_secp256k1_xonly_pubkey_tweak_add_check)
	ZEND_FE(secp256k1_keypair_create, arginfo_secp256k1_keypair_create)
	ZEND_FE(secp256k1_keypair_pub, arginfo_secp256k1_keypair_pub)
	ZEND_FE(secp256k1_keypair_xonly_pub, arginfo_secp256k1_keypair_xonly_pub)
	ZEND_FE(secp256k1_keypair_sec, arginfo_secp256k1_keypair_sec)
	ZEND_FE(secp256k1_keypair_xonly_tweak_add, arginfo_secp256k1_keypair_xonly_tweak_add)
#endif
#if defined(HAVE_SECP256K1_RECOVERY)
	ZEND_FE(secp256k1_ecdsa_recoverable_signature_parse_compact, arginfo_secp256k1_ecdsa_recoverable_signature_parse_compact)
	ZEND_FE(secp256k1_ecdsa_recoverable_signature_serialize_compact, arginfo_secp256k1_ecdsa_recoverable_signature_serialize_compact)
	ZEND_FE(secp256k1_ecdsa_recoverable_signature_convert, arginfo_secp256k1_ecdsa_recoverable_signature_convert)
	ZEND_FE(secp256k1_ecdsa_sign_recoverable, arginfo_secp256k1_ecdsa_sign_recoverable)
	ZEND_FE(secp256k1_ecdsa_recover, arginfo_secp256k1_ecdsa_recover)
#endif
#if defined(HAVE_SECP256K1_SCHNORRSIG)
	ZEND_FE(secp256k1_schnorrsig_sign32, arginfo_secp256k1_schnorrsig_sign32)
	ZEND_FE(secp256k1_schnorrsig_sign_custom, arginfo_secp256k1_schnorrsig_sign_custom)
	ZEND_FE(secp256k1_schnorrsig_verify, arginfo_secp256k1_schnorrsig_verify)
#endif
#if defined(HAVE_SECP256K1_ELLSWIFT)
	ZEND_FE(secp256k1_ellswift_encode, arginfo_secp256k1_ellswift_encode)
	ZEND_FE(secp256k1_ellswift_decode, arginfo_secp256k1_ellswift_decode)
	ZEND_FE(secp256k1_ellswift_create, arginfo_secp256k1_ellswift_create)
	ZEND_FE(secp256k1_ellswift_xdh, arginfo_secp256k1_ellswift_xdh)
#endif
	ZEND_FE_END
};

static void register_secp256k1_symbols(int module_number)
{
	REGISTER_LONG_CONSTANT("SECP256K1_EC_COMPRESSED", SECP256K1_EC_COMPRESSED, CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("SECP256K1_EC_UNCOMPRESSED", SECP256K1_EC_UNCOMPRESSED, CONST_PERSISTENT);
#if defined(HAVE_SECP256K1_ELLSWIFT)
	REGISTER_LONG_CONSTANT("SECP256K1_ELLSWIFT_XDH_HASH_BIP324", SECP256K1_ELLSWIFT_XDH_HASH_BIP324, CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("SECP256K1_ELLSWIFT_XDH_HASH_PREFIX", SECP256K1_ELLSWIFT_XDH_HASH_PREFIX, CONST_PERSISTENT);
#endif
}

static zend_class_entry *register_class_secp256k1_pubkey(void)
{
	zend_class_entry ce, *class_entry;

	INIT_CLASS_ENTRY(ce, "secp256k1_pubkey", NULL);
#if (PHP_VERSION_ID >= 80400)
	class_entry = zend_register_internal_class_with_flags(&ce, NULL, ZEND_ACC_FINAL|ZEND_ACC_NO_DYNAMIC_PROPERTIES|ZEND_ACC_NOT_SERIALIZABLE);
#else
	class_entry = zend_register_internal_class_ex(&ce, NULL);
	class_entry->ce_flags |= ZEND_ACC_FINAL|ZEND_ACC_NO_DYNAMIC_PROPERTIES|ZEND_ACC_NOT_SERIALIZABLE;
#endif

	return class_entry;
}

static zend_class_entry *register_class_secp256k1_ecdsa_signature(void)
{
	zend_class_entry ce, *class_entry;

	INIT_CLASS_ENTRY(ce, "secp256k1_ecdsa_signature", NULL);
#if (PHP_VERSION_ID >= 80400)
	class_entry = zend_register_internal_class_with_flags(&ce, NULL, ZEND_ACC_FINAL|ZEND_ACC_NO_DYNAMIC_PROPERTIES|ZEND_ACC_NOT_SERIALIZABLE);
#else
	class_entry = zend_register_internal_class_ex(&ce, NULL);
	class_entry->ce_flags |= ZEND_ACC_FINAL|ZEND_ACC_NO_DYNAMIC_PROPERTIES|ZEND_ACC_NOT_SERIALIZABLE;
#endif

	return class_entry;
}

#if defined(HAVE_SECP256K1_EXTRAKEYS)
static zend_class_entry *register_class_secp256k1_xonly_pubkey(void)
{
	zend_class_entry ce, *class_entry;

	INIT_CLASS_ENTRY(ce, "secp256k1_xonly_pubkey", NULL);
#if (PHP_VERSION_ID >= 80400)
	class_entry = zend_register_internal_class_with_flags(&ce, NULL, ZEND_ACC_FINAL|ZEND_ACC_NO_DYNAMIC_PROPERTIES|ZEND_ACC_NOT_SERIALIZABLE);
#else
	class_entry = zend_register_internal_class_ex(&ce, NULL);
	class_entry->ce_flags |= ZEND_ACC_FINAL|ZEND_ACC_NO_DYNAMIC_PROPERTIES|ZEND_ACC_NOT_SERIALIZABLE;
#endif

	return class_entry;
}
#endif

#if defined(HAVE_SECP256K1_EXTRAKEYS)
static zend_class_entry *register_class_secp256k1_keypair(void)
{
	zend_class_entry ce, *class_entry;

	INIT_CLASS_ENTRY(ce, "secp256k1_keypair", NULL);
#if (PHP_VERSION_ID >= 80400)
	class_entry = zend_register_internal_class_with_flags(&ce, NULL, ZEND_ACC_FINAL|ZEND_ACC_NO_DYNAMIC_PROPERTIES|ZEND_ACC_NOT_SERIALIZABLE);
#else
	class_entry = zend_register_internal_class_ex(&ce, NULL);
	class_entry->ce_flags |= ZEND_ACC_FINAL|ZEND_ACC_NO_DYNAMIC_PROPERTIES|ZEND_ACC_NOT_SERIALIZABLE;
#endif

	return class_entry;
}
#endif

#if defined(HAVE_SECP256K1_RECOVERY)
static zend_class_entry *register_class_secp256k1_ecdsa_recoverable_signature(void)
{
	zend_class_entry ce, *class_entry;

	INIT_CLASS_ENTRY(ce, "secp256k1_ecdsa_recoverable_signature", NULL);
#if (PHP_VERSION_ID >= 80400)
	class_entry = zend_register_internal_class_with_flags(&ce, NULL, ZEND_ACC_FINAL|ZEND_ACC_NO_DYNAMIC_PROPERTIES|ZEND_ACC_NOT_SERIALIZABLE);
#else
	class_entry = zend_register_internal_class_ex(&ce, NULL);
	class_entry->ce_flags |= ZEND_ACC_FINAL|ZEND_ACC_NO_DYNAMIC_PROPERTIES|ZEND_ACC_NOT_SERIALIZABLE;
#endif

	return class_entry;
}
#endif
