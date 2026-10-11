#include "secp256k1_module.h"

PHP_FUNCTION(secp256k1_musig_pubkey_agg)
{
	HashTable *pubkeys_ht;
	zval *entry;
	size_t n_pubkeys, i;

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ARRAY_HT(pubkeys_ht)
	ZEND_PARSE_PARAMETERS_END();

	n_pubkeys = zend_hash_num_elements(pubkeys_ht);
	if (n_pubkeys == 0) {
		zend_argument_value_error(1, "must not be empty");
		RETURN_THROWS();
	}

	const secp256k1_pubkey **pubkey_ptrs = emalloc(sizeof(secp256k1_pubkey *) * n_pubkeys);

	i = 0;
	ZEND_HASH_FOREACH_VAL(pubkeys_ht, entry) {
		if (Z_TYPE_P(entry) != IS_OBJECT || !instanceof_function(Z_OBJCE_P(entry), secp256k1_pubkey_ce)) {
			efree(pubkey_ptrs);
			zend_argument_type_error(1, "must contain only secp256k1_pubkey objects");
			RETURN_THROWS();
		}
		pubkey_ptrs[i++] = &secp256k1_pubkey_from_obj(Z_OBJ_P(entry))->pubkey;
	} ZEND_HASH_FOREACH_END();

	secp256k1_musig_keyagg_cache cache;

	RETVAL_FALSE;
	if (secp256k1_musig_pubkey_agg(SECP256K1_G(ctx), NULL, &cache, pubkey_ptrs, n_pubkeys)) {
		zend_object *obj = secp256k1_musig_keyagg_cache_create_object(secp256k1_musig_keyagg_cache_ce);
		secp256k1_musig_keyagg_cache_from_obj(obj)->cache = cache;
		RETVAL_OBJ(obj);
	}

	efree(pubkey_ptrs);
}

PHP_FUNCTION(secp256k1_musig_pubkey_get)
{
	zval *cache_zval;

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_OBJECT_OF_CLASS(cache_zval, secp256k1_musig_keyagg_cache_ce)
	ZEND_PARSE_PARAMETERS_END();

	secp256k1_musig_keyagg_cache_obj *cache_intern = secp256k1_musig_keyagg_cache_from_obj(Z_OBJ_P(cache_zval));
	secp256k1_pubkey pubkey;

	RETVAL_FALSE;
	if (secp256k1_musig_pubkey_get(SECP256K1_G(ctx), &pubkey, &cache_intern->cache)) {
		zend_object *obj = secp256k1_pubkey_create_object(secp256k1_pubkey_ce);
		secp256k1_pubkey_from_obj(obj)->pubkey = pubkey;
		RETVAL_OBJ(obj);
	}
}

PHP_FUNCTION(secp256k1_musig_pubkey_ec_tweak_add)
{
	zval *cache_zval;
	char *tweak;
	size_t tweak_len;

	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_OBJECT_OF_CLASS(cache_zval, secp256k1_musig_keyagg_cache_ce)
		Z_PARAM_STRING(tweak, tweak_len)
	ZEND_PARSE_PARAMETERS_END();

	if (tweak_len != 32) {
		zend_argument_value_error(2, "must be exactly 32 bytes");
		RETURN_THROWS();
	}

	secp256k1_musig_keyagg_cache_obj *cache_intern = secp256k1_musig_keyagg_cache_from_obj(Z_OBJ_P(cache_zval));
	secp256k1_pubkey output_pubkey;

	RETVAL_FALSE;
	if (secp256k1_musig_pubkey_ec_tweak_add(SECP256K1_G(ctx), &output_pubkey, &cache_intern->cache, (const unsigned char *)tweak)) {
		zend_object *obj = secp256k1_pubkey_create_object(secp256k1_pubkey_ce);
		secp256k1_pubkey_from_obj(obj)->pubkey = output_pubkey;
		RETVAL_OBJ(obj);
	}
}

PHP_FUNCTION(secp256k1_musig_pubkey_xonly_tweak_add)
{
	zval *cache_zval;
	char *tweak;
	size_t tweak_len;

	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_OBJECT_OF_CLASS(cache_zval, secp256k1_musig_keyagg_cache_ce)
		Z_PARAM_STRING(tweak, tweak_len)
	ZEND_PARSE_PARAMETERS_END();

	if (tweak_len != 32) {
		zend_argument_value_error(2, "must be exactly 32 bytes");
		RETURN_THROWS();
	}

	secp256k1_musig_keyagg_cache_obj *cache_intern = secp256k1_musig_keyagg_cache_from_obj(Z_OBJ_P(cache_zval));
	secp256k1_pubkey output_pubkey;

	RETVAL_FALSE;
	if (secp256k1_musig_pubkey_xonly_tweak_add(SECP256K1_G(ctx), &output_pubkey, &cache_intern->cache, (const unsigned char *)tweak)) {
		zend_object *obj = secp256k1_pubkey_create_object(secp256k1_pubkey_ce);
		secp256k1_pubkey_from_obj(obj)->pubkey = output_pubkey;
		RETVAL_OBJ(obj);
	}
}

PHP_FUNCTION(secp256k1_musig_pubnonce_parse)
{
	char *input;
	size_t input_len;

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_STRING(input, input_len)
	ZEND_PARSE_PARAMETERS_END();

	if (input_len != 66) {
		zend_argument_value_error(1, "must be exactly 66 bytes");
		RETURN_THROWS();
	}

	secp256k1_musig_pubnonce pubnonce;

	RETVAL_FALSE;
	if (secp256k1_musig_pubnonce_parse(SECP256K1_G(ctx), &pubnonce, (const unsigned char *)input)) {
		zend_object *obj = secp256k1_musig_pubnonce_create_object(secp256k1_musig_pubnonce_ce);
		secp256k1_musig_pubnonce_from_obj(obj)->pubnonce = pubnonce;
		RETVAL_OBJ(obj);
	}
}

PHP_FUNCTION(secp256k1_musig_pubnonce_serialize)
{
	zval *nonce_zval;
	unsigned char output[66];

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_OBJECT_OF_CLASS(nonce_zval, secp256k1_musig_pubnonce_ce)
	ZEND_PARSE_PARAMETERS_END();

	secp256k1_musig_pubnonce_obj *intern = secp256k1_musig_pubnonce_from_obj(Z_OBJ_P(nonce_zval));

	secp256k1_musig_pubnonce_serialize(SECP256K1_G(ctx), output, &intern->pubnonce);

	RETVAL_STRINGL((char *)output, 66);
	explicit_bzero(output, sizeof(output));
}

PHP_FUNCTION(secp256k1_musig_aggnonce_parse)
{
	char *input;
	size_t input_len;

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_STRING(input, input_len)
	ZEND_PARSE_PARAMETERS_END();

	if (input_len != 66) {
		zend_argument_value_error(1, "must be exactly 66 bytes");
		RETURN_THROWS();
	}

	secp256k1_musig_aggnonce aggnonce;

	RETVAL_FALSE;
	if (secp256k1_musig_aggnonce_parse(SECP256K1_G(ctx), &aggnonce, (const unsigned char *)input)) {
		zend_object *obj = secp256k1_musig_aggnonce_create_object(secp256k1_musig_aggnonce_ce);
		secp256k1_musig_aggnonce_from_obj(obj)->aggnonce = aggnonce;
		RETVAL_OBJ(obj);
	}
}

PHP_FUNCTION(secp256k1_musig_aggnonce_serialize)
{
	zval *nonce_zval;
	unsigned char output[66];

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_OBJECT_OF_CLASS(nonce_zval, secp256k1_musig_aggnonce_ce)
	ZEND_PARSE_PARAMETERS_END();

	secp256k1_musig_aggnonce_obj *intern = secp256k1_musig_aggnonce_from_obj(Z_OBJ_P(nonce_zval));

	secp256k1_musig_aggnonce_serialize(SECP256K1_G(ctx), output, &intern->aggnonce);

	RETVAL_STRINGL((char *)output, 66);
	explicit_bzero(output, sizeof(output));
}

PHP_FUNCTION(secp256k1_musig_partial_sig_parse)
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

	secp256k1_musig_partial_sig partial_sig;

	RETVAL_FALSE;
	if (secp256k1_musig_partial_sig_parse(SECP256K1_G(ctx), &partial_sig, (const unsigned char *)input)) {
		zend_object *obj = secp256k1_musig_partial_sig_create_object(secp256k1_musig_partial_sig_ce);
		secp256k1_musig_partial_sig_from_obj(obj)->partial_sig = partial_sig;
		RETVAL_OBJ(obj);
	}
}

PHP_FUNCTION(secp256k1_musig_partial_sig_serialize)
{
	zval *sig_zval;
	unsigned char output[32];

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_OBJECT_OF_CLASS(sig_zval, secp256k1_musig_partial_sig_ce)
	ZEND_PARSE_PARAMETERS_END();

	secp256k1_musig_partial_sig_obj *intern = secp256k1_musig_partial_sig_from_obj(Z_OBJ_P(sig_zval));

	secp256k1_musig_partial_sig_serialize(SECP256K1_G(ctx), output, &intern->partial_sig);

	RETVAL_STRINGL((char *)output, 32);
	explicit_bzero(output, sizeof(output));
}
