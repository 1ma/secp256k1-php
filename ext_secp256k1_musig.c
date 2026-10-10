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
