/* This is a generated file, edit the .stub.php file instead.
 * Stub hash: 9bf6ed727f5e6a1a2b95957c88441096eff1c976 */

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
	ZEND_FE_END
};

static void register_secp256k1_symbols(int module_number)
{
	REGISTER_LONG_CONSTANT("SECP256K1_EC_COMPRESSED", SECP256K1_EC_COMPRESSED, CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("SECP256K1_EC_UNCOMPRESSED", SECP256K1_EC_UNCOMPRESSED, CONST_PERSISTENT);
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
