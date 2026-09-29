/* This is a generated file, edit the .stub.php file instead.
 * Stub hash: 506f9ea4ca526386e27a986d23ad89e1ccb4e55b */

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

ZEND_FUNCTION(secp256k1_ec_seckey_verify);
ZEND_FUNCTION(secp256k1_ec_pubkey_create);
ZEND_FUNCTION(secp256k1_ec_pubkey_parse);
ZEND_FUNCTION(secp256k1_ec_pubkey_serialize);

static const zend_function_entry ext_functions[] = {
	ZEND_FE(secp256k1_ec_seckey_verify, arginfo_secp256k1_ec_seckey_verify)
	ZEND_FE(secp256k1_ec_pubkey_create, arginfo_secp256k1_ec_pubkey_create)
	ZEND_FE(secp256k1_ec_pubkey_parse, arginfo_secp256k1_ec_pubkey_parse)
	ZEND_FE(secp256k1_ec_pubkey_serialize, arginfo_secp256k1_ec_pubkey_serialize)
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
