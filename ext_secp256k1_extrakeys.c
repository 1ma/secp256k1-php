#include "secp256k1_module.h"

PHP_FUNCTION(secp256k1_xonly_pubkey_parse)
{
	char *input;
	size_t input_len;

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_STRING(input, input_len)
	ZEND_PARSE_PARAMETERS_END();

	if (input_len != 32) {
		zend_argument_value_error(1, "must be exactly 32 bytes");
		RETURN_THROWS();
	}

	zend_object *obj = secp256k1_xonly_pubkey_create_object(secp256k1_xonly_pubkey_ce);
	secp256k1_xonly_pubkey_obj *intern = secp256k1_xonly_pubkey_from_obj(obj);

	if (!secp256k1_xonly_pubkey_parse(secp256k1_ctx, &intern->xonly_pubkey, (const unsigned char *)input)) {
		zend_object_release(obj);
		RETURN_FALSE;
	}

	RETURN_OBJ(obj);
}

PHP_FUNCTION(secp256k1_xonly_pubkey_serialize)
{
	zval *pubkey_zval;
	unsigned char output[32];

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_OBJECT_OF_CLASS(pubkey_zval, secp256k1_xonly_pubkey_ce)
	ZEND_PARSE_PARAMETERS_END();

	secp256k1_xonly_pubkey_obj *intern = secp256k1_xonly_pubkey_from_obj(Z_OBJ_P(pubkey_zval));

	secp256k1_xonly_pubkey_serialize(secp256k1_ctx, output, &intern->xonly_pubkey);

	RETURN_STRINGL((char *)output, 32);
}

PHP_FUNCTION(secp256k1_xonly_pubkey_cmp)
{
	zval *pk1_zval, *pk2_zval;

	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_OBJECT_OF_CLASS(pk1_zval, secp256k1_xonly_pubkey_ce)
		Z_PARAM_OBJECT_OF_CLASS(pk2_zval, secp256k1_xonly_pubkey_ce)
	ZEND_PARSE_PARAMETERS_END();

	secp256k1_xonly_pubkey_obj *pk1 = secp256k1_xonly_pubkey_from_obj(Z_OBJ_P(pk1_zval));
	secp256k1_xonly_pubkey_obj *pk2 = secp256k1_xonly_pubkey_from_obj(Z_OBJ_P(pk2_zval));

	int result = secp256k1_xonly_pubkey_cmp(secp256k1_ctx, &pk1->xonly_pubkey, &pk2->xonly_pubkey);

	RETURN_LONG(result > 0 ? 1 : (result < 0 ? -1 : 0));
}

PHP_FUNCTION(secp256k1_xonly_pubkey_from_pubkey)
{
	zval *pubkey_zval, *parity_zval;

	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_OBJECT_OF_CLASS(pubkey_zval, secp256k1_pubkey_ce)
		Z_PARAM_ZVAL(parity_zval)
	ZEND_PARSE_PARAMETERS_END();

	secp256k1_pubkey_obj *pubkey_intern = secp256k1_pubkey_from_obj(Z_OBJ_P(pubkey_zval));

	zend_object *obj = secp256k1_xonly_pubkey_create_object(secp256k1_xonly_pubkey_ce);
	secp256k1_xonly_pubkey_obj *xonly_intern = secp256k1_xonly_pubkey_from_obj(obj);

	int parity;
	int ok = secp256k1_xonly_pubkey_from_pubkey(secp256k1_ctx, &xonly_intern->xonly_pubkey, &parity, &pubkey_intern->pubkey);
	(void)ok;

	ZEND_TRY_ASSIGN_REF_LONG(parity_zval, parity);
	RETURN_OBJ(obj);
}

PHP_FUNCTION(secp256k1_xonly_pubkey_tweak_add)
{
	zval *pubkey_zval;
	char *tweak;
	size_t tweak_len;

	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_OBJECT_OF_CLASS(pubkey_zval, secp256k1_xonly_pubkey_ce)
		Z_PARAM_STRING(tweak, tweak_len)
	ZEND_PARSE_PARAMETERS_END();

	if (tweak_len != 32) {
		zend_argument_value_error(2, "must be exactly 32 bytes");
		RETURN_THROWS();
	}

	secp256k1_xonly_pubkey_obj *intern = secp256k1_xonly_pubkey_from_obj(Z_OBJ_P(pubkey_zval));

	zend_object *obj = secp256k1_pubkey_create_object(secp256k1_pubkey_ce);
	secp256k1_pubkey_obj *result = secp256k1_pubkey_from_obj(obj);

	if (!secp256k1_xonly_pubkey_tweak_add(secp256k1_ctx, &result->pubkey, &intern->xonly_pubkey, (const unsigned char *)tweak)) {
		zend_object_release(obj);
		RETURN_FALSE;
	}

	RETURN_OBJ(obj);
}

PHP_FUNCTION(secp256k1_xonly_pubkey_tweak_add_check)
{
	char *tweaked_pubkey32, *tweak32;
	size_t tweaked_len, tweak_len;
	zend_long tweaked_pk_parity;
	zval *internal_zval;

	ZEND_PARSE_PARAMETERS_START(4, 4)
		Z_PARAM_STRING(tweaked_pubkey32, tweaked_len)
		Z_PARAM_LONG(tweaked_pk_parity)
		Z_PARAM_OBJECT_OF_CLASS(internal_zval, secp256k1_xonly_pubkey_ce)
		Z_PARAM_STRING(tweak32, tweak_len)
	ZEND_PARSE_PARAMETERS_END();

	if (tweaked_len != 32) {
		zend_argument_value_error(1, "must be exactly 32 bytes");
		RETURN_THROWS();
	}

	if (tweak_len != 32) {
		zend_argument_value_error(4, "must be exactly 32 bytes");
		RETURN_THROWS();
	}

	secp256k1_xonly_pubkey_obj *intern = secp256k1_xonly_pubkey_from_obj(Z_OBJ_P(internal_zval));

	RETURN_BOOL(secp256k1_xonly_pubkey_tweak_add_check(
		secp256k1_ctx,
		(const unsigned char *)tweaked_pubkey32,
		(int)tweaked_pk_parity,
		&intern->xonly_pubkey,
		(const unsigned char *)tweak32
	));
}

/* keypair functions */

PHP_FUNCTION(secp256k1_keypair_create)
{
	char *seckey;
	size_t seckey_len;

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_STRING(seckey, seckey_len)
	ZEND_PARSE_PARAMETERS_END();

	if (seckey_len != 32) {
		zend_argument_value_error(1, "must be exactly 32 bytes");
		RETURN_THROWS();
	}

	zend_object *obj = secp256k1_keypair_create_object(secp256k1_keypair_ce);
	secp256k1_keypair_obj *intern = secp256k1_keypair_from_obj(obj);

	if (!secp256k1_keypair_create(secp256k1_ctx, &intern->keypair, (const unsigned char *)seckey)) {
		zend_object_release(obj);
		RETURN_FALSE;
	}

	RETURN_OBJ(obj);
}

PHP_FUNCTION(secp256k1_keypair_pub)
{
	zval *keypair_zval;

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_OBJECT_OF_CLASS(keypair_zval, secp256k1_keypair_ce)
	ZEND_PARSE_PARAMETERS_END();

	secp256k1_keypair_obj *keypair_intern = secp256k1_keypair_from_obj(Z_OBJ_P(keypair_zval));

	zend_object *obj = secp256k1_pubkey_create_object(secp256k1_pubkey_ce);
	secp256k1_pubkey_obj *pubkey_intern = secp256k1_pubkey_from_obj(obj);

	int ok = secp256k1_keypair_pub(secp256k1_ctx, &pubkey_intern->pubkey, &keypair_intern->keypair);
	(void)ok;

	RETURN_OBJ(obj);
}

PHP_FUNCTION(secp256k1_keypair_xonly_pub)
{
	zval *keypair_zval, *parity_zval;

	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_OBJECT_OF_CLASS(keypair_zval, secp256k1_keypair_ce)
		Z_PARAM_ZVAL(parity_zval)
	ZEND_PARSE_PARAMETERS_END();

	secp256k1_keypair_obj *keypair_intern = secp256k1_keypair_from_obj(Z_OBJ_P(keypair_zval));

	zend_object *obj = secp256k1_xonly_pubkey_create_object(secp256k1_xonly_pubkey_ce);
	secp256k1_xonly_pubkey_obj *xonly_intern = secp256k1_xonly_pubkey_from_obj(obj);

	int parity;
	int ok = secp256k1_keypair_xonly_pub(secp256k1_ctx, &xonly_intern->xonly_pubkey, &parity, &keypair_intern->keypair);
	(void)ok;

	ZEND_TRY_ASSIGN_REF_LONG(parity_zval, parity);
	RETURN_OBJ(obj);
}

PHP_FUNCTION(secp256k1_keypair_sec)
{
	zval *keypair_zval;
	unsigned char seckey[32];

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_OBJECT_OF_CLASS(keypair_zval, secp256k1_keypair_ce)
	ZEND_PARSE_PARAMETERS_END();

	secp256k1_keypair_obj *intern = secp256k1_keypair_from_obj(Z_OBJ_P(keypair_zval));

	int ok = secp256k1_keypair_sec(secp256k1_ctx, seckey, &intern->keypair);
	(void)ok;

	RETVAL_STRINGL((char *)seckey, 32);
	explicit_bzero(seckey, sizeof(seckey));
}

PHP_FUNCTION(secp256k1_keypair_xonly_tweak_add)
{
	zval *keypair_zval;
	char *tweak;
	size_t tweak_len;

	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_OBJECT_OF_CLASS_EX(keypair_zval, secp256k1_keypair_ce, 0, 1)
		Z_PARAM_STRING(tweak, tweak_len)
	ZEND_PARSE_PARAMETERS_END();

	if (tweak_len != 32) {
		zend_argument_value_error(2, "must be exactly 32 bytes");
		RETURN_THROWS();
	}

	secp256k1_keypair_obj *intern = secp256k1_keypair_from_obj(Z_OBJ_P(keypair_zval));

	if (!secp256k1_keypair_xonly_tweak_add(secp256k1_ctx, &intern->keypair, (const unsigned char *)tweak)) {
		RETURN_FALSE;
	}

	RETURN_TRUE;
}
