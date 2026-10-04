#include "secp256k1_module.h"

#include <secp256k1_ecdh.h>

PHP_FUNCTION(secp256k1_ecdh)
{
	zval *pubkey_zval;
	char *seckey;
	size_t seckey_len;
	unsigned char output[32];

	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_OBJECT_OF_CLASS(pubkey_zval, secp256k1_pubkey_ce)
		Z_PARAM_STRING(seckey, seckey_len)
	ZEND_PARSE_PARAMETERS_END();

	if (seckey_len != 32) {
		zend_argument_value_error(2, "must be exactly 32 bytes");
		RETURN_THROWS();
	}

	secp256k1_pubkey_obj *intern = secp256k1_pubkey_from_obj(Z_OBJ_P(pubkey_zval));

	if (!secp256k1_ecdh(SECP256K1_G(ctx), output, &intern->pubkey, (const unsigned char *)seckey, NULL, NULL)) {
		explicit_bzero(output, sizeof(output));
		RETURN_FALSE;
	}

	RETVAL_STRINGL((char *)output, 32);
	explicit_bzero(output, sizeof(output));
}
