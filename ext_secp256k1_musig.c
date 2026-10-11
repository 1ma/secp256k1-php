#include "secp256k1_module.h"

#if PHP_VERSION_ID >= 80400
# include "ext/random/php_random_csprng.h"
#else
# include "ext/random/php_random.h"
#endif

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

PHP_FUNCTION(secp256k1_musig_nonce_gen)
{
	zval *pubkey_zval;
	char *seckey = NULL, *msg = NULL, *extra = NULL;
	size_t seckey_len, msg_len, extra_len;
	zval *cache_zval = NULL;
	unsigned char session_secrand32[32];

	ZEND_PARSE_PARAMETERS_START(1, 5)
		Z_PARAM_OBJECT_OF_CLASS(pubkey_zval, secp256k1_pubkey_ce)
		Z_PARAM_OPTIONAL
		Z_PARAM_STRING_OR_NULL(seckey, seckey_len)
		Z_PARAM_STRING_OR_NULL(msg, msg_len)
		Z_PARAM_OBJECT_OF_CLASS_OR_NULL(cache_zval, secp256k1_musig_keyagg_cache_ce)
		Z_PARAM_STRING_OR_NULL(extra, extra_len)
	ZEND_PARSE_PARAMETERS_END();

	if (seckey && seckey_len != 32) {
		zend_argument_value_error(2, "must be exactly 32 bytes");
		RETURN_THROWS();
	}
	if (msg && msg_len != 32) {
		zend_argument_value_error(3, "must be exactly 32 bytes");
		RETURN_THROWS();
	}
	if (extra && extra_len != 32) {
		zend_argument_value_error(5, "must be exactly 32 bytes");
		RETURN_THROWS();
	}

	if (php_random_bytes_throw(session_secrand32, sizeof(session_secrand32)) != SUCCESS) {
		RETURN_FALSE;
	}

	secp256k1_pubkey_obj *pubkey_intern = secp256k1_pubkey_from_obj(Z_OBJ_P(pubkey_zval));
	const secp256k1_musig_keyagg_cache *cache_ptr = cache_zval
		? &secp256k1_musig_keyagg_cache_from_obj(Z_OBJ_P(cache_zval))->cache
		: NULL;

	secp256k1_musig_secnonce secnonce;
	secp256k1_musig_pubnonce pubnonce;

	RETVAL_FALSE;
	if (secp256k1_musig_nonce_gen(
		SECP256K1_G(ctx), &secnonce, &pubnonce, session_secrand32,
		(const unsigned char *)seckey,
		&pubkey_intern->pubkey,
		(const unsigned char *)msg,
		cache_ptr,
		(const unsigned char *)extra
	)) {
		zend_object *secnonce_obj = secp256k1_musig_secnonce_create_object(secp256k1_musig_secnonce_ce);
		secp256k1_musig_secnonce_from_obj(secnonce_obj)->secnonce = secnonce;

		zend_object *pubnonce_obj = secp256k1_musig_pubnonce_create_object(secp256k1_musig_pubnonce_ce);
		secp256k1_musig_pubnonce_from_obj(pubnonce_obj)->pubnonce = pubnonce;

		array_init_size(return_value, 2);
		add_assoc_object(return_value, "secnonce", secnonce_obj);
		add_assoc_object(return_value, "pubnonce", pubnonce_obj);
	}

	explicit_bzero(session_secrand32, sizeof(session_secrand32));
	explicit_bzero(&secnonce, sizeof(secnonce));
}

PHP_FUNCTION(secp256k1_musig_nonce_gen_counter)
{
	zend_long nonrepeating_cnt;
	zval *keypair_zval;
	char *msg = NULL, *extra = NULL;
	size_t msg_len, extra_len;
	zval *cache_zval = NULL;

	ZEND_PARSE_PARAMETERS_START(2, 5)
		Z_PARAM_LONG(nonrepeating_cnt)
		Z_PARAM_OBJECT_OF_CLASS(keypair_zval, secp256k1_keypair_ce)
		Z_PARAM_OPTIONAL
		Z_PARAM_STRING_OR_NULL(msg, msg_len)
		Z_PARAM_OBJECT_OF_CLASS_OR_NULL(cache_zval, secp256k1_musig_keyagg_cache_ce)
		Z_PARAM_STRING_OR_NULL(extra, extra_len)
	ZEND_PARSE_PARAMETERS_END();

	if (msg && msg_len != 32) {
		zend_argument_value_error(3, "must be exactly 32 bytes");
		RETURN_THROWS();
	}
	if (extra && extra_len != 32) {
		zend_argument_value_error(5, "must be exactly 32 bytes");
		RETURN_THROWS();
	}

	secp256k1_keypair_obj *keypair_intern = secp256k1_keypair_from_obj(Z_OBJ_P(keypair_zval));
	const secp256k1_musig_keyagg_cache *cache_ptr = cache_zval
		? &secp256k1_musig_keyagg_cache_from_obj(Z_OBJ_P(cache_zval))->cache
		: NULL;

	secp256k1_musig_secnonce secnonce;
	secp256k1_musig_pubnonce pubnonce;

	RETVAL_FALSE;
	if (secp256k1_musig_nonce_gen_counter(
		SECP256K1_G(ctx), &secnonce, &pubnonce,
		(uint64_t)nonrepeating_cnt,
		&keypair_intern->keypair,
		(const unsigned char *)msg,
		cache_ptr,
		(const unsigned char *)extra
	)) {
		zend_object *secnonce_obj = secp256k1_musig_secnonce_create_object(secp256k1_musig_secnonce_ce);
		secp256k1_musig_secnonce_from_obj(secnonce_obj)->secnonce = secnonce;

		zend_object *pubnonce_obj = secp256k1_musig_pubnonce_create_object(secp256k1_musig_pubnonce_ce);
		secp256k1_musig_pubnonce_from_obj(pubnonce_obj)->pubnonce = pubnonce;

		array_init_size(return_value, 2);
		add_assoc_object(return_value, "secnonce", secnonce_obj);
		add_assoc_object(return_value, "pubnonce", pubnonce_obj);
	}

	explicit_bzero(&secnonce, sizeof(secnonce));
}

PHP_FUNCTION(secp256k1_musig_nonce_agg)
{
	HashTable *pubnonces_ht;
	zval *entry;
	size_t n_pubnonces, i;

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ARRAY_HT(pubnonces_ht)
	ZEND_PARSE_PARAMETERS_END();

	n_pubnonces = zend_hash_num_elements(pubnonces_ht);
	if (n_pubnonces == 0) {
		zend_argument_value_error(1, "must not be empty");
		RETURN_THROWS();
	}

	const secp256k1_musig_pubnonce **pubnonce_ptrs = emalloc(sizeof(secp256k1_musig_pubnonce *) * n_pubnonces);

	i = 0;
	ZEND_HASH_FOREACH_VAL(pubnonces_ht, entry) {
		if (Z_TYPE_P(entry) != IS_OBJECT || !instanceof_function(Z_OBJCE_P(entry), secp256k1_musig_pubnonce_ce)) {
			efree(pubnonce_ptrs);
			zend_argument_type_error(1, "must contain only secp256k1_musig_pubnonce objects");
			RETURN_THROWS();
		}
		pubnonce_ptrs[i++] = &secp256k1_musig_pubnonce_from_obj(Z_OBJ_P(entry))->pubnonce;
	} ZEND_HASH_FOREACH_END();

	secp256k1_musig_aggnonce aggnonce;

	RETVAL_FALSE;
	if (secp256k1_musig_nonce_agg(SECP256K1_G(ctx), &aggnonce, pubnonce_ptrs, n_pubnonces)) {
		zend_object *obj = secp256k1_musig_aggnonce_create_object(secp256k1_musig_aggnonce_ce);
		secp256k1_musig_aggnonce_from_obj(obj)->aggnonce = aggnonce;
		RETVAL_OBJ(obj);
	}

	efree(pubnonce_ptrs);
}

PHP_FUNCTION(secp256k1_musig_nonce_process)
{
	zval *aggnonce_zval, *cache_zval;
	char *msg;
	size_t msg_len;

	ZEND_PARSE_PARAMETERS_START(3, 3)
		Z_PARAM_OBJECT_OF_CLASS(aggnonce_zval, secp256k1_musig_aggnonce_ce)
		Z_PARAM_STRING(msg, msg_len)
		Z_PARAM_OBJECT_OF_CLASS(cache_zval, secp256k1_musig_keyagg_cache_ce)
	ZEND_PARSE_PARAMETERS_END();

	if (msg_len != 32) {
		zend_argument_value_error(2, "must be exactly 32 bytes");
		RETURN_THROWS();
	}

	secp256k1_musig_aggnonce_obj *aggnonce_intern = secp256k1_musig_aggnonce_from_obj(Z_OBJ_P(aggnonce_zval));
	secp256k1_musig_keyagg_cache_obj *cache_intern = secp256k1_musig_keyagg_cache_from_obj(Z_OBJ_P(cache_zval));
	secp256k1_musig_session session;

	RETVAL_FALSE;
	if (secp256k1_musig_nonce_process(SECP256K1_G(ctx), &session, &aggnonce_intern->aggnonce, (const unsigned char *)msg, &cache_intern->cache)) {
		zend_object *obj = secp256k1_musig_session_create_object(secp256k1_musig_session_ce);
		secp256k1_musig_session_from_obj(obj)->session = session;
		RETVAL_OBJ(obj);
	}
}

PHP_FUNCTION(secp256k1_musig_partial_sign)
{
	zval *secnonce_zval, *keypair_zval, *cache_zval, *session_zval;

	ZEND_PARSE_PARAMETERS_START(4, 4)
		Z_PARAM_OBJECT_OF_CLASS(secnonce_zval, secp256k1_musig_secnonce_ce)
		Z_PARAM_OBJECT_OF_CLASS(keypair_zval, secp256k1_keypair_ce)
		Z_PARAM_OBJECT_OF_CLASS(cache_zval, secp256k1_musig_keyagg_cache_ce)
		Z_PARAM_OBJECT_OF_CLASS(session_zval, secp256k1_musig_session_ce)
	ZEND_PARSE_PARAMETERS_END();

	secp256k1_musig_secnonce_obj *secnonce_intern = secp256k1_musig_secnonce_from_obj(Z_OBJ_P(secnonce_zval));
	secp256k1_keypair_obj *keypair_intern = secp256k1_keypair_from_obj(Z_OBJ_P(keypair_zval));
	secp256k1_musig_keyagg_cache_obj *cache_intern = secp256k1_musig_keyagg_cache_from_obj(Z_OBJ_P(cache_zval));
	secp256k1_musig_session_obj *session_intern = secp256k1_musig_session_from_obj(Z_OBJ_P(session_zval));
	secp256k1_musig_partial_sig partial_sig;

	RETVAL_FALSE;
	if (secp256k1_musig_partial_sign(SECP256K1_G(ctx), &partial_sig, &secnonce_intern->secnonce, &keypair_intern->keypair, &cache_intern->cache, &session_intern->session)) {
		zend_object *obj = secp256k1_musig_partial_sig_create_object(secp256k1_musig_partial_sig_ce);
		secp256k1_musig_partial_sig_from_obj(obj)->partial_sig = partial_sig;
		RETVAL_OBJ(obj);
	}
}

PHP_FUNCTION(secp256k1_musig_partial_sig_verify)
{
	zval *sig_zval, *pubnonce_zval, *pubkey_zval, *cache_zval, *session_zval;

	ZEND_PARSE_PARAMETERS_START(5, 5)
		Z_PARAM_OBJECT_OF_CLASS(sig_zval, secp256k1_musig_partial_sig_ce)
		Z_PARAM_OBJECT_OF_CLASS(pubnonce_zval, secp256k1_musig_pubnonce_ce)
		Z_PARAM_OBJECT_OF_CLASS(pubkey_zval, secp256k1_pubkey_ce)
		Z_PARAM_OBJECT_OF_CLASS(cache_zval, secp256k1_musig_keyagg_cache_ce)
		Z_PARAM_OBJECT_OF_CLASS(session_zval, secp256k1_musig_session_ce)
	ZEND_PARSE_PARAMETERS_END();

	secp256k1_musig_partial_sig_obj *sig_intern = secp256k1_musig_partial_sig_from_obj(Z_OBJ_P(sig_zval));
	secp256k1_musig_pubnonce_obj *pubnonce_intern = secp256k1_musig_pubnonce_from_obj(Z_OBJ_P(pubnonce_zval));
	secp256k1_pubkey_obj *pubkey_intern = secp256k1_pubkey_from_obj(Z_OBJ_P(pubkey_zval));
	secp256k1_musig_keyagg_cache_obj *cache_intern = secp256k1_musig_keyagg_cache_from_obj(Z_OBJ_P(cache_zval));
	secp256k1_musig_session_obj *session_intern = secp256k1_musig_session_from_obj(Z_OBJ_P(session_zval));

	RETURN_BOOL(secp256k1_musig_partial_sig_verify(SECP256K1_G(ctx), &sig_intern->partial_sig, &pubnonce_intern->pubnonce, &pubkey_intern->pubkey, &cache_intern->cache, &session_intern->session));
}

PHP_FUNCTION(secp256k1_musig_partial_sig_agg)
{
	zval *session_zval;
	HashTable *sigs_ht;
	zval *entry;
	size_t n_sigs, i;

	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_OBJECT_OF_CLASS(session_zval, secp256k1_musig_session_ce)
		Z_PARAM_ARRAY_HT(sigs_ht)
	ZEND_PARSE_PARAMETERS_END();

	n_sigs = zend_hash_num_elements(sigs_ht);
	if (n_sigs == 0) {
		zend_argument_value_error(2, "must not be empty");
		RETURN_THROWS();
	}

	const secp256k1_musig_partial_sig **sig_ptrs = emalloc(sizeof(secp256k1_musig_partial_sig *) * n_sigs);

	i = 0;
	ZEND_HASH_FOREACH_VAL(sigs_ht, entry) {
		if (Z_TYPE_P(entry) != IS_OBJECT || !instanceof_function(Z_OBJCE_P(entry), secp256k1_musig_partial_sig_ce)) {
			efree(sig_ptrs);
			zend_argument_type_error(2, "must contain only secp256k1_musig_partial_sig objects");
			RETURN_THROWS();
		}
		sig_ptrs[i++] = &secp256k1_musig_partial_sig_from_obj(Z_OBJ_P(entry))->partial_sig;
	} ZEND_HASH_FOREACH_END();

	secp256k1_musig_session_obj *session_intern = secp256k1_musig_session_from_obj(Z_OBJ_P(session_zval));
	unsigned char sig64[64];

	RETVAL_FALSE;
	if (secp256k1_musig_partial_sig_agg(SECP256K1_G(ctx), sig64, &session_intern->session, sig_ptrs, n_sigs)) {
		RETVAL_STRINGL((char *)sig64, 64);
	}

	explicit_bzero(sig64, sizeof(sig64));
	efree(sig_ptrs);
}
