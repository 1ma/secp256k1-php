#include "secp256k1_module.h"

PHP_FUNCTION(secp256k1_ec_seckey_verify)
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

	RETURN_BOOL(secp256k1_ec_seckey_verify(secp256k1_ctx, (const unsigned char *)seckey));
}

PHP_FUNCTION(secp256k1_ec_pubkey_create)
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

	zend_object *obj = secp256k1_pubkey_create_object(secp256k1_pubkey_ce);
	secp256k1_pubkey_obj *intern = secp256k1_pubkey_from_obj(obj);

	if (!secp256k1_ec_pubkey_create(secp256k1_ctx, &intern->pubkey, (const unsigned char *)seckey)) {
		zend_object_release(obj);
		RETURN_FALSE;
	}

	RETURN_OBJ(obj);
}

PHP_FUNCTION(secp256k1_ec_pubkey_parse)
{
	char *input;
	size_t input_len;

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_STRING(input, input_len)
	ZEND_PARSE_PARAMETERS_END();

	zend_object *obj = secp256k1_pubkey_create_object(secp256k1_pubkey_ce);
	secp256k1_pubkey_obj *intern = secp256k1_pubkey_from_obj(obj);

	if (!secp256k1_ec_pubkey_parse(secp256k1_ctx, &intern->pubkey, (const unsigned char *)input, input_len)) {
		zend_object_release(obj);
		RETURN_FALSE;
	}

	RETURN_OBJ(obj);
}

PHP_FUNCTION(secp256k1_ec_pubkey_serialize)
{
	zval *pubkey_zval;
	zend_long flags = SECP256K1_EC_COMPRESSED;
	unsigned char output[65];
	size_t outputlen = sizeof(output);

	ZEND_PARSE_PARAMETERS_START(1, 2)
		Z_PARAM_OBJECT_OF_CLASS(pubkey_zval, secp256k1_pubkey_ce)
		Z_PARAM_OPTIONAL
		Z_PARAM_LONG(flags)
	ZEND_PARSE_PARAMETERS_END();

	if (flags != SECP256K1_EC_COMPRESSED && flags != SECP256K1_EC_UNCOMPRESSED) {
		zend_argument_value_error(2, "must be SECP256K1_EC_COMPRESSED or SECP256K1_EC_UNCOMPRESSED");
		RETURN_THROWS();
	}

	secp256k1_pubkey_obj *intern = secp256k1_pubkey_from_obj(Z_OBJ_P(pubkey_zval));

	secp256k1_ec_pubkey_serialize(secp256k1_ctx, output, &outputlen, &intern->pubkey, (unsigned int)flags);

	RETVAL_STRINGL((char *)output, outputlen);
	explicit_bzero(output, sizeof(output));
}

PHP_FUNCTION(secp256k1_ecdsa_signature_parse_compact)
{
	char *sig64;
	size_t sig64_len;

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_STRING(sig64, sig64_len)
	ZEND_PARSE_PARAMETERS_END();

	if (sig64_len != 64) {
		zend_argument_value_error(1, "must be exactly 64 bytes");
		RETURN_THROWS();
	}

	zend_object *obj = secp256k1_ecdsa_sig_create_object(secp256k1_ecdsa_sig_ce);
	secp256k1_ecdsa_sig_obj *intern = secp256k1_ecdsa_sig_from_obj(obj);

	if (!secp256k1_ecdsa_signature_parse_compact(secp256k1_ctx, &intern->sig, (const unsigned char *)sig64)) {
		zend_object_release(obj);
		RETURN_FALSE;
	}

	RETURN_OBJ(obj);
}

PHP_FUNCTION(secp256k1_ecdsa_signature_parse_der)
{
	char *der;
	size_t der_len;

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_STRING(der, der_len)
	ZEND_PARSE_PARAMETERS_END();

	zend_object *obj = secp256k1_ecdsa_sig_create_object(secp256k1_ecdsa_sig_ce);
	secp256k1_ecdsa_sig_obj *intern = secp256k1_ecdsa_sig_from_obj(obj);

	if (!secp256k1_ecdsa_signature_parse_der(secp256k1_ctx, &intern->sig, (const unsigned char *)der, der_len)) {
		zend_object_release(obj);
		RETURN_FALSE;
	}

	RETURN_OBJ(obj);
}

PHP_FUNCTION(secp256k1_ecdsa_signature_serialize_compact)
{
	zval *sig_zval;
	unsigned char output[64];

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_OBJECT_OF_CLASS(sig_zval, secp256k1_ecdsa_sig_ce)
	ZEND_PARSE_PARAMETERS_END();

	secp256k1_ecdsa_sig_obj *intern = secp256k1_ecdsa_sig_from_obj(Z_OBJ_P(sig_zval));

	secp256k1_ecdsa_signature_serialize_compact(secp256k1_ctx, output, &intern->sig);

	RETVAL_STRINGL((char *)output, 64);
	explicit_bzero(output, sizeof(output));
}

PHP_FUNCTION(secp256k1_ecdsa_signature_serialize_der)
{
	zval *sig_zval;
	/* DER: 6 bytes overhead + up to 33 bytes each for R and S (ecdsa_impl.h) */
	unsigned char output[72];
	size_t outputlen = sizeof(output);

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_OBJECT_OF_CLASS(sig_zval, secp256k1_ecdsa_sig_ce)
	ZEND_PARSE_PARAMETERS_END();

	secp256k1_ecdsa_sig_obj *intern = secp256k1_ecdsa_sig_from_obj(Z_OBJ_P(sig_zval));

	secp256k1_ecdsa_signature_serialize_der(secp256k1_ctx, output, &outputlen, &intern->sig);

	RETVAL_STRINGL((char *)output, outputlen);
	explicit_bzero(output, sizeof(output));
}

PHP_FUNCTION(secp256k1_ecdsa_signature_normalize)
{
	zval *sig_zval;

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_OBJECT_OF_CLASS_EX(sig_zval, secp256k1_ecdsa_sig_ce, 0, 1)
	ZEND_PARSE_PARAMETERS_END();

	secp256k1_ecdsa_sig_obj *intern = secp256k1_ecdsa_sig_from_obj(Z_OBJ_P(sig_zval));

	RETURN_BOOL(secp256k1_ecdsa_signature_normalize(secp256k1_ctx, &intern->sig, &intern->sig));
}

PHP_FUNCTION(secp256k1_ecdsa_sign)
{
	char *msghash32, *seckey32;
	size_t msghash32_len, seckey32_len;

	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_STRING(msghash32, msghash32_len)
		Z_PARAM_STRING(seckey32, seckey32_len)
	ZEND_PARSE_PARAMETERS_END();

	if (msghash32_len != 32) {
		zend_argument_value_error(1, "must be exactly 32 bytes");
		RETURN_THROWS();
	}

	if (seckey32_len != 32) {
		zend_argument_value_error(2, "must be exactly 32 bytes");
		RETURN_THROWS();
	}

	zend_object *obj = secp256k1_ecdsa_sig_create_object(secp256k1_ecdsa_sig_ce);
	secp256k1_ecdsa_sig_obj *intern = secp256k1_ecdsa_sig_from_obj(obj);

	if (!secp256k1_ecdsa_sign(secp256k1_ctx, &intern->sig, (const unsigned char *)msghash32, (const unsigned char *)seckey32, NULL, NULL)) {
		zend_object_release(obj);
		RETURN_FALSE;
	}

	RETURN_OBJ(obj);
}

PHP_FUNCTION(secp256k1_ecdsa_verify)
{
	zval *sig_zval, *pubkey_zval;
	char *msghash32;
	size_t msghash32_len;

	ZEND_PARSE_PARAMETERS_START(3, 3)
		Z_PARAM_OBJECT_OF_CLASS(sig_zval, secp256k1_ecdsa_sig_ce)
		Z_PARAM_STRING(msghash32, msghash32_len)
		Z_PARAM_OBJECT_OF_CLASS(pubkey_zval, secp256k1_pubkey_ce)
	ZEND_PARSE_PARAMETERS_END();

	if (msghash32_len != 32) {
		zend_argument_value_error(2, "must be exactly 32 bytes");
		RETURN_THROWS();
	}

	secp256k1_ecdsa_sig_obj *sig_intern = secp256k1_ecdsa_sig_from_obj(Z_OBJ_P(sig_zval));
	secp256k1_pubkey_obj *pubkey_intern = secp256k1_pubkey_from_obj(Z_OBJ_P(pubkey_zval));

	RETURN_BOOL(secp256k1_ecdsa_verify(secp256k1_ctx, &sig_intern->sig, (const unsigned char *)msghash32, &pubkey_intern->pubkey));
}

PHP_FUNCTION(secp256k1_ec_seckey_negate)
{
	char *seckey;
	size_t seckey_len;
	unsigned char buf[32];

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_STRING(seckey, seckey_len)
	ZEND_PARSE_PARAMETERS_END();

	if (seckey_len != 32) {
		zend_argument_value_error(1, "must be exactly 32 bytes");
		RETURN_THROWS();
	}

	memcpy(buf, seckey, 32);

	if (!secp256k1_ec_seckey_negate(secp256k1_ctx, buf)) {
		explicit_bzero(buf, sizeof(buf));
		RETURN_FALSE;
	}

	RETVAL_STRINGL((char *)buf, 32);
	explicit_bzero(buf, sizeof(buf));
}

PHP_FUNCTION(secp256k1_ec_seckey_tweak_add)
{
	char *seckey, *tweak;
	size_t seckey_len, tweak_len;
	unsigned char buf[32];

	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_STRING(seckey, seckey_len)
		Z_PARAM_STRING(tweak, tweak_len)
	ZEND_PARSE_PARAMETERS_END();

	if (seckey_len != 32) {
		zend_argument_value_error(1, "must be exactly 32 bytes");
		RETURN_THROWS();
	}

	if (tweak_len != 32) {
		zend_argument_value_error(2, "must be exactly 32 bytes");
		RETURN_THROWS();
	}

	memcpy(buf, seckey, 32);

	if (!secp256k1_ec_seckey_tweak_add(secp256k1_ctx, buf, (const unsigned char *)tweak)) {
		explicit_bzero(buf, sizeof(buf));
		RETURN_FALSE;
	}

	RETVAL_STRINGL((char *)buf, 32);
	explicit_bzero(buf, sizeof(buf));
}

PHP_FUNCTION(secp256k1_ec_seckey_tweak_mul)
{
	char *seckey, *tweak;
	size_t seckey_len, tweak_len;
	unsigned char buf[32];

	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_STRING(seckey, seckey_len)
		Z_PARAM_STRING(tweak, tweak_len)
	ZEND_PARSE_PARAMETERS_END();

	if (seckey_len != 32) {
		zend_argument_value_error(1, "must be exactly 32 bytes");
		RETURN_THROWS();
	}

	if (tweak_len != 32) {
		zend_argument_value_error(2, "must be exactly 32 bytes");
		RETURN_THROWS();
	}

	memcpy(buf, seckey, 32);

	if (!secp256k1_ec_seckey_tweak_mul(secp256k1_ctx, buf, (const unsigned char *)tweak)) {
		explicit_bzero(buf, sizeof(buf));
		RETURN_FALSE;
	}

	RETVAL_STRINGL((char *)buf, 32);
	explicit_bzero(buf, sizeof(buf));
}

PHP_FUNCTION(secp256k1_ec_pubkey_negate)
{
	zval *pubkey_zval;

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_OBJECT_OF_CLASS_EX(pubkey_zval, secp256k1_pubkey_ce, 0, 1)
	ZEND_PARSE_PARAMETERS_END();

	secp256k1_pubkey_obj *intern = secp256k1_pubkey_from_obj(Z_OBJ_P(pubkey_zval));
	int ret = secp256k1_ec_pubkey_negate(secp256k1_ctx, &intern->pubkey);
	(void)ret;
}

PHP_FUNCTION(secp256k1_ec_pubkey_tweak_add)
{
	zval *pubkey_zval;
	char *tweak;
	size_t tweak_len;

	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_OBJECT_OF_CLASS_EX(pubkey_zval, secp256k1_pubkey_ce, 0, 1)
		Z_PARAM_STRING(tweak, tweak_len)
	ZEND_PARSE_PARAMETERS_END();

	if (tweak_len != 32) {
		zend_argument_value_error(2, "must be exactly 32 bytes");
		RETURN_THROWS();
	}

	secp256k1_pubkey_obj *intern = secp256k1_pubkey_from_obj(Z_OBJ_P(pubkey_zval));
	secp256k1_pubkey tmp = intern->pubkey;

	if (!secp256k1_ec_pubkey_tweak_add(secp256k1_ctx, &tmp, (const unsigned char *)tweak)) {
		RETURN_FALSE;
	}

	intern->pubkey = tmp;
	RETURN_TRUE;
}

PHP_FUNCTION(secp256k1_ec_pubkey_tweak_mul)
{
	zval *pubkey_zval;
	char *tweak;
	size_t tweak_len;

	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_OBJECT_OF_CLASS_EX(pubkey_zval, secp256k1_pubkey_ce, 0, 1)
		Z_PARAM_STRING(tweak, tweak_len)
	ZEND_PARSE_PARAMETERS_END();

	if (tweak_len != 32) {
		zend_argument_value_error(2, "must be exactly 32 bytes");
		RETURN_THROWS();
	}

	secp256k1_pubkey_obj *intern = secp256k1_pubkey_from_obj(Z_OBJ_P(pubkey_zval));
	secp256k1_pubkey tmp = intern->pubkey;

	if (!secp256k1_ec_pubkey_tweak_mul(secp256k1_ctx, &tmp, (const unsigned char *)tweak)) {
		RETURN_FALSE;
	}

	intern->pubkey = tmp;
	RETURN_TRUE;
}

PHP_FUNCTION(secp256k1_ec_pubkey_combine)
{
	HashTable *pubkeys_ht;
	zval *entry;
	size_t n, i;
	const secp256k1_pubkey **pubkey_ptrs;

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ARRAY_HT(pubkeys_ht)
	ZEND_PARSE_PARAMETERS_END();

	n = zend_hash_num_elements(pubkeys_ht);
	if (n == 0) {
		zend_argument_value_error(1, "must not be empty");
		RETURN_THROWS();
	}

	pubkey_ptrs = emalloc(sizeof(secp256k1_pubkey *) * n);
	i = 0;

	ZEND_HASH_FOREACH_VAL(pubkeys_ht, entry) {
		if (Z_TYPE_P(entry) != IS_OBJECT || !instanceof_function(Z_OBJCE_P(entry), secp256k1_pubkey_ce)) {
			efree(pubkey_ptrs);
			zend_argument_type_error(1, "must contain only secp256k1_pubkey objects");
			RETURN_THROWS();
		}
		pubkey_ptrs[i++] = &secp256k1_pubkey_from_obj(Z_OBJ_P(entry))->pubkey;
	} ZEND_HASH_FOREACH_END();

	zend_object *obj = secp256k1_pubkey_create_object(secp256k1_pubkey_ce);
	secp256k1_pubkey_obj *intern = secp256k1_pubkey_from_obj(obj);

	if (!secp256k1_ec_pubkey_combine(secp256k1_ctx, &intern->pubkey, pubkey_ptrs, n)) {
		efree(pubkey_ptrs);
		zend_object_release(obj);
		RETURN_FALSE;
	}

	efree(pubkey_ptrs);
	RETURN_OBJ(obj);
}

PHP_FUNCTION(secp256k1_ec_pubkey_cmp)
{
	zval *pubkey1_zval, *pubkey2_zval;

	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_OBJECT_OF_CLASS(pubkey1_zval, secp256k1_pubkey_ce)
		Z_PARAM_OBJECT_OF_CLASS(pubkey2_zval, secp256k1_pubkey_ce)
	ZEND_PARSE_PARAMETERS_END();

	secp256k1_pubkey_obj *pk1 = secp256k1_pubkey_from_obj(Z_OBJ_P(pubkey1_zval));
	secp256k1_pubkey_obj *pk2 = secp256k1_pubkey_from_obj(Z_OBJ_P(pubkey2_zval));

	int result = secp256k1_ec_pubkey_cmp(secp256k1_ctx, &pk1->pubkey, &pk2->pubkey);

	RETURN_LONG(result > 0 ? 1 : (result < 0 ? -1 : 0));
}

PHP_FUNCTION(secp256k1_tagged_sha256)
{
	char *tag, *msg;
	size_t tag_len, msg_len;
	unsigned char hash[32];

	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_STRING(tag, tag_len)
		Z_PARAM_STRING(msg, msg_len)
	ZEND_PARSE_PARAMETERS_END();

	int ok = secp256k1_tagged_sha256(secp256k1_ctx, hash, (const unsigned char *)tag, tag_len, (const unsigned char *)msg, msg_len);
	(void)ok;

	RETVAL_STRINGL((char *)hash, 32);
	explicit_bzero(hash, sizeof(hash));
}
