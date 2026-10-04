#include "secp256k1_module.h"
#include <secp256k1_schnorrsig.h>

PHP_FUNCTION(secp256k1_schnorrsig_sign32)
{
	char *msg, *aux = NULL;
	size_t msg_len, aux_len;
	zval *keypair_zval;
	unsigned char sig[64];

	ZEND_PARSE_PARAMETERS_START(2, 3)
		Z_PARAM_STRING(msg, msg_len)
		Z_PARAM_OBJECT_OF_CLASS(keypair_zval, secp256k1_keypair_ce)
		Z_PARAM_OPTIONAL
		Z_PARAM_STRING_OR_NULL(aux, aux_len)
	ZEND_PARSE_PARAMETERS_END();

	if (msg_len != 32) {
		zend_argument_value_error(1, "must be exactly 32 bytes");
		RETURN_THROWS();
	}

	if (aux != NULL && aux_len != 32) {
		zend_argument_value_error(3, "must be exactly 32 bytes");
		RETURN_THROWS();
	}

	secp256k1_keypair_obj *keypair_intern = secp256k1_keypair_from_obj(Z_OBJ_P(keypair_zval));

	RETVAL_FALSE;
	if (secp256k1_schnorrsig_sign32(secp256k1_ctx, sig, (const unsigned char *)msg, &keypair_intern->keypair, (const unsigned char *)aux)) {
		RETVAL_STRINGL((char *)sig, 64);
	}

	explicit_bzero(sig, sizeof(sig));
}

PHP_FUNCTION(secp256k1_schnorrsig_sign_custom)
{
	char *msg = NULL;
	size_t msg_len;
	zval *keypair_zval;
	unsigned char sig[64];

	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_STRING(msg, msg_len)
		Z_PARAM_OBJECT_OF_CLASS(keypair_zval, secp256k1_keypair_ce)
	ZEND_PARSE_PARAMETERS_END();

	secp256k1_keypair_obj *keypair_intern = secp256k1_keypair_from_obj(Z_OBJ_P(keypair_zval));

	RETVAL_FALSE;
	if (secp256k1_schnorrsig_sign_custom(secp256k1_ctx, sig, (const unsigned char *)msg, msg_len, &keypair_intern->keypair, NULL)) {
		RETVAL_STRINGL((char *)sig, 64);
	}

	explicit_bzero(sig, sizeof(sig));
}

PHP_FUNCTION(secp256k1_schnorrsig_verify)
{
	char *sig, *msg;
	size_t sig_len, msg_len;
	zval *pubkey_zval;

	ZEND_PARSE_PARAMETERS_START(3, 3)
		Z_PARAM_STRING(sig, sig_len)
		Z_PARAM_STRING(msg, msg_len)
		Z_PARAM_OBJECT_OF_CLASS(pubkey_zval, secp256k1_xonly_pubkey_ce)
	ZEND_PARSE_PARAMETERS_END();

	if (sig_len != 64) {
		zend_argument_value_error(1, "must be exactly 64 bytes");
		RETURN_THROWS();
	}

	secp256k1_xonly_pubkey_obj *pubkey_intern = secp256k1_xonly_pubkey_from_obj(Z_OBJ_P(pubkey_zval));

	RETURN_BOOL(secp256k1_schnorrsig_verify(secp256k1_ctx, (const unsigned char *)sig, (const unsigned char *)msg, msg_len, &pubkey_intern->xonly_pubkey));
}
