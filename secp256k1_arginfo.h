/* This is a generated file, edit the .stub.php file instead.
 * Stub hash: 3f31ade5477bc0e1dc3e4ef3c0f43228b9b45997 */

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_secp256k1_ec_seckey_verify, 0, 1, _IS_BOOL, 0)
	ZEND_ARG_TYPE_INFO(0, seckey32, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_FUNCTION(secp256k1_ec_seckey_verify);

static const zend_function_entry ext_functions[] = {
	ZEND_FE(secp256k1_ec_seckey_verify, arginfo_secp256k1_ec_seckey_verify)
	ZEND_FE_END
};
