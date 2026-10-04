#include "secp256k1_module.h"

PHP_FUNCTION(secp256k1_ellswift_encode)
{
	zval *pubkey_zval;
	char *rnd;
	size_t rnd_len;
	unsigned char ell64[64];

	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_OBJECT_OF_CLASS(pubkey_zval, secp256k1_pubkey_ce)
		Z_PARAM_STRING(rnd, rnd_len)
	ZEND_PARSE_PARAMETERS_END();

	if (rnd_len != 32) {
		zend_argument_value_error(2, "must be exactly 32 bytes");
		RETURN_THROWS();
	}

	secp256k1_pubkey_obj *intern = secp256k1_pubkey_from_obj(Z_OBJ_P(pubkey_zval));

	secp256k1_ellswift_encode(secp256k1_ctx, ell64, &intern->pubkey, (const unsigned char *)rnd);

	RETVAL_STRINGL((char *)ell64, 64);
	explicit_bzero(ell64, sizeof(ell64));
}

PHP_FUNCTION(secp256k1_ellswift_decode)
{
	char *ell;
	size_t ell_len;

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_STRING(ell, ell_len)
	ZEND_PARSE_PARAMETERS_END();

	if (ell_len != 64) {
		zend_argument_value_error(1, "must be exactly 64 bytes");
		RETURN_THROWS();
	}

	zend_object *obj = secp256k1_pubkey_create_object(secp256k1_pubkey_ce);
	secp256k1_pubkey_obj *intern = secp256k1_pubkey_from_obj(obj);

	secp256k1_ellswift_decode(secp256k1_ctx, &intern->pubkey, (const unsigned char *)ell);

	RETURN_OBJ(obj);
}

PHP_FUNCTION(secp256k1_ellswift_create)
{
	char *seckey, *aux = NULL;
	size_t seckey_len, aux_len;
	unsigned char ell64[64];

	ZEND_PARSE_PARAMETERS_START(1, 2)
		Z_PARAM_STRING(seckey, seckey_len)
		Z_PARAM_OPTIONAL
		Z_PARAM_STRING_OR_NULL(aux, aux_len)
	ZEND_PARSE_PARAMETERS_END();

	if (seckey_len != 32) {
		zend_argument_value_error(1, "must be exactly 32 bytes");
		RETURN_THROWS();
	}

	if (aux != NULL && aux_len != 32) {
		zend_argument_value_error(2, "must be exactly 32 bytes");
		RETURN_THROWS();
	}

	if (!secp256k1_ellswift_create(secp256k1_ctx, ell64, (const unsigned char *)seckey, (const unsigned char *)aux)) {
		explicit_bzero(ell64, sizeof(ell64));
		RETURN_FALSE;
	}

	RETVAL_STRINGL((char *)ell64, 64);
	explicit_bzero(ell64, sizeof(ell64));
}

PHP_FUNCTION(secp256k1_ellswift_xdh)
{
	char *ell_a, *ell_b, *seckey, *prefix = NULL;
	size_t ell_a_len, ell_b_len, seckey_len, prefix_len;
	bool party;
	zend_long hashfn = 0;
	unsigned char output[32];
	secp256k1_ellswift_xdh_hash_function hashfp;
	void *data = NULL;

	ZEND_PARSE_PARAMETERS_START(4, 6)
		Z_PARAM_STRING(ell_a, ell_a_len)
		Z_PARAM_STRING(ell_b, ell_b_len)
		Z_PARAM_STRING(seckey, seckey_len)
		Z_PARAM_BOOL(party)
		Z_PARAM_OPTIONAL
		Z_PARAM_LONG(hashfn)
		Z_PARAM_STRING_OR_NULL(prefix, prefix_len)
	ZEND_PARSE_PARAMETERS_END();

	if (ell_a_len != 64) {
		zend_argument_value_error(1, "must be exactly 64 bytes");
		RETURN_THROWS();
	}

	if (ell_b_len != 64) {
		zend_argument_value_error(2, "must be exactly 64 bytes");
		RETURN_THROWS();
	}

	if (seckey_len != 32) {
		zend_argument_value_error(3, "must be exactly 32 bytes");
		RETURN_THROWS();
	}

	switch (hashfn) {
		case SECP256K1_ELLSWIFT_XDH_HASH_BIP324:
			hashfp = secp256k1_ellswift_xdh_hash_function_bip324;
			break;
		case SECP256K1_ELLSWIFT_XDH_HASH_PREFIX:
			if (prefix == NULL) {
				zend_argument_value_error(6, "is required when using SECP256K1_ELLSWIFT_XDH_HASH_PREFIX");
				RETURN_THROWS();
			}
			if (prefix_len != 64) {
				zend_argument_value_error(6, "must be exactly 64 bytes");
				RETURN_THROWS();
			}
			hashfp = secp256k1_ellswift_xdh_hash_function_prefix;
			data = prefix;
			break;
		default:
			zend_argument_value_error(5, "must be SECP256K1_ELLSWIFT_XDH_HASH_BIP324 or SECP256K1_ELLSWIFT_XDH_HASH_PREFIX");
			RETURN_THROWS();
	}

	if (!secp256k1_ellswift_xdh(secp256k1_ctx, output,
			(const unsigned char *)ell_a, (const unsigned char *)ell_b,
			(const unsigned char *)seckey, party, hashfp, data)) {
		explicit_bzero(output, sizeof(output));
		RETURN_FALSE;
	}

	RETVAL_STRINGL((char *)output, 32);
	explicit_bzero(output, sizeof(output));
}
