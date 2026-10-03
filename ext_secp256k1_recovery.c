#include "secp256k1_module.h"

PHP_FUNCTION(secp256k1_ecdsa_recoverable_signature_parse_compact)
{
	char *input;
	size_t input_len;
	zend_long recid;

	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_STRING(input, input_len)
		Z_PARAM_LONG(recid)
	ZEND_PARSE_PARAMETERS_END();

	if (input_len != 64) {
		zend_argument_value_error(1, "must be exactly 64 bytes");
		RETURN_THROWS();
	}

	if (recid < 0 || recid > 3) {
		zend_argument_value_error(2, "must be between 0 and 3");
		RETURN_THROWS();
	}

	zend_object *obj = secp256k1_ecdsa_recoverable_sig_create_object(secp256k1_ecdsa_recoverable_sig_ce);
	secp256k1_ecdsa_recoverable_sig_obj *intern = secp256k1_ecdsa_recoverable_sig_from_obj(obj);

	if (!secp256k1_ecdsa_recoverable_signature_parse_compact(secp256k1_ctx, &intern->sig, (const unsigned char *)input, (int)recid)) {
		zend_object_release(obj);
		RETURN_FALSE;
	}

	RETURN_OBJ(obj);
}

PHP_FUNCTION(secp256k1_ecdsa_recoverable_signature_serialize_compact)
{
	zval *sig_zval, *recid_zval;
	unsigned char output[64];
	int recid;

	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_OBJECT_OF_CLASS(sig_zval, secp256k1_ecdsa_recoverable_sig_ce)
		Z_PARAM_ZVAL(recid_zval)
	ZEND_PARSE_PARAMETERS_END();

	secp256k1_ecdsa_recoverable_sig_obj *intern = secp256k1_ecdsa_recoverable_sig_from_obj(Z_OBJ_P(sig_zval));

	int ok = secp256k1_ecdsa_recoverable_signature_serialize_compact(secp256k1_ctx, output, &recid, &intern->sig);
	(void)ok;

	ZEND_TRY_ASSIGN_REF_LONG(recid_zval, recid);
	RETURN_STRINGL((char *)output, 64);
}

PHP_FUNCTION(secp256k1_ecdsa_recoverable_signature_convert)
{
	zval *sig_zval;

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_OBJECT_OF_CLASS(sig_zval, secp256k1_ecdsa_recoverable_sig_ce)
	ZEND_PARSE_PARAMETERS_END();

	secp256k1_ecdsa_recoverable_sig_obj *intern = secp256k1_ecdsa_recoverable_sig_from_obj(Z_OBJ_P(sig_zval));

	zend_object *obj = secp256k1_ecdsa_sig_create_object(secp256k1_ecdsa_sig_ce);
	secp256k1_ecdsa_sig_obj *result = secp256k1_ecdsa_sig_from_obj(obj);

	int ok = secp256k1_ecdsa_recoverable_signature_convert(secp256k1_ctx, &result->sig, &intern->sig);
	(void)ok;

	RETURN_OBJ(obj);
}

PHP_FUNCTION(secp256k1_ecdsa_sign_recoverable)
{
	char *msg, *seckey;
	size_t msg_len, seckey_len;

	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_STRING(msg, msg_len)
		Z_PARAM_STRING(seckey, seckey_len)
	ZEND_PARSE_PARAMETERS_END();

	if (msg_len != 32) {
		zend_argument_value_error(1, "must be exactly 32 bytes");
		RETURN_THROWS();
	}

	if (seckey_len != 32) {
		zend_argument_value_error(2, "must be exactly 32 bytes");
		RETURN_THROWS();
	}

	zend_object *obj = secp256k1_ecdsa_recoverable_sig_create_object(secp256k1_ecdsa_recoverable_sig_ce);
	secp256k1_ecdsa_recoverable_sig_obj *intern = secp256k1_ecdsa_recoverable_sig_from_obj(obj);

	if (!secp256k1_ecdsa_sign_recoverable(secp256k1_ctx, &intern->sig, (const unsigned char *)msg, (const unsigned char *)seckey, NULL, NULL)) {
		zend_object_release(obj);
		RETURN_FALSE;
	}

	RETURN_OBJ(obj);
}

PHP_FUNCTION(secp256k1_ecdsa_recover)
{
	zval *sig_zval;
	char *msg;
	size_t msg_len;

	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_OBJECT_OF_CLASS(sig_zval, secp256k1_ecdsa_recoverable_sig_ce)
		Z_PARAM_STRING(msg, msg_len)
	ZEND_PARSE_PARAMETERS_END();

	if (msg_len != 32) {
		zend_argument_value_error(2, "must be exactly 32 bytes");
		RETURN_THROWS();
	}

	secp256k1_ecdsa_recoverable_sig_obj *sig_intern = secp256k1_ecdsa_recoverable_sig_from_obj(Z_OBJ_P(sig_zval));

	zend_object *obj = secp256k1_pubkey_create_object(secp256k1_pubkey_ce);
	secp256k1_pubkey_obj *pubkey_intern = secp256k1_pubkey_from_obj(obj);

	if (!secp256k1_ecdsa_recover(secp256k1_ctx, &pubkey_intern->pubkey, &sig_intern->sig, (const unsigned char *)msg)) {
		zend_object_release(obj);
		RETURN_FALSE;
	}

	RETURN_OBJ(obj);
}
