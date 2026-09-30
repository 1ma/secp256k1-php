/* secp256k1 extension for PHP */

#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#include "php.h"
#include "ext/standard/info.h"
#if PHP_VERSION_ID >= 80400
# include "ext/random/php_random_csprng.h"
#else
# include "ext/random/php_random.h"
#endif
#include "php_secp256k1.h"

#include <secp256k1.h>

#include "secp256k1_arginfo.h"

static secp256k1_context *secp256k1_ctx = NULL;

static zend_class_entry *secp256k1_pubkey_ce;
static zend_object_handlers secp256k1_pubkey_handlers;

typedef struct {
	secp256k1_pubkey pubkey;
	zend_object std;
} secp256k1_pubkey_obj;

static inline secp256k1_pubkey_obj *secp256k1_pubkey_from_obj(zend_object *obj)
{
	return (secp256k1_pubkey_obj *)((char *)obj - offsetof(secp256k1_pubkey_obj, std));
}

static zend_object *secp256k1_pubkey_create_object(zend_class_entry *ce)
{
	secp256k1_pubkey_obj *intern = zend_object_alloc(sizeof(secp256k1_pubkey_obj), ce);

	zend_object_std_init(&intern->std, ce);
	intern->std.handlers = &secp256k1_pubkey_handlers;

	return &intern->std;
}

static zend_object *secp256k1_pubkey_deny_new(zend_class_entry *ce)
{
	zend_throw_error(NULL, "Cannot instantiate %s directly, use secp256k1_ec_pubkey_create() or secp256k1_ec_pubkey_parse()", ZSTR_VAL(ce->name));
	return zend_objects_new(ce);
}

static void secp256k1_pubkey_free_object(zend_object *obj)
{
	secp256k1_pubkey_obj *intern = secp256k1_pubkey_from_obj(obj);

	explicit_bzero(&intern->pubkey, sizeof(secp256k1_pubkey));
	zend_object_std_dtor(&intern->std);
}

static zend_class_entry *secp256k1_ecdsa_sig_ce;
static zend_object_handlers secp256k1_ecdsa_sig_handlers;

typedef struct {
	secp256k1_ecdsa_signature sig;
	zend_object std;
} secp256k1_ecdsa_sig_obj;

static inline secp256k1_ecdsa_sig_obj *secp256k1_ecdsa_sig_from_obj(zend_object *obj)
{
	return (secp256k1_ecdsa_sig_obj *)((char *)obj - offsetof(secp256k1_ecdsa_sig_obj, std));
}

static zend_object *secp256k1_ecdsa_sig_create_object(zend_class_entry *ce)
{
	secp256k1_ecdsa_sig_obj *intern = zend_object_alloc(sizeof(secp256k1_ecdsa_sig_obj), ce);

	zend_object_std_init(&intern->std, ce);
	intern->std.handlers = &secp256k1_ecdsa_sig_handlers;

	return &intern->std;
}

static zend_object *secp256k1_ecdsa_sig_deny_new(zend_class_entry *ce)
{
	zend_throw_error(NULL, "Cannot instantiate %s directly, use secp256k1_ecdsa_signature_parse_compact() or secp256k1_ecdsa_signature_parse_der()", ZSTR_VAL(ce->name));
	return zend_objects_new(ce);
}

static void secp256k1_ecdsa_sig_free_object(zend_object *obj)
{
	secp256k1_ecdsa_sig_obj *intern = secp256k1_ecdsa_sig_from_obj(obj);

	explicit_bzero(&intern->sig, sizeof(secp256k1_ecdsa_signature));
	zend_object_std_dtor(&intern->std);
}

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

	RETURN_STRINGL((char *)output, outputlen);
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

	RETURN_STRINGL((char *)output, 64);
}

PHP_FUNCTION(secp256k1_ecdsa_signature_serialize_der)
{
	zval *sig_zval;
	unsigned char output[72];
	size_t outputlen = sizeof(output);

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_OBJECT_OF_CLASS(sig_zval, secp256k1_ecdsa_sig_ce)
	ZEND_PARSE_PARAMETERS_END();

	secp256k1_ecdsa_sig_obj *intern = secp256k1_ecdsa_sig_from_obj(Z_OBJ_P(sig_zval));

	secp256k1_ecdsa_signature_serialize_der(secp256k1_ctx, output, &outputlen, &intern->sig);

	RETURN_STRINGL((char *)output, outputlen);
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

PHP_MINIT_FUNCTION(secp256k1)
{
	unsigned char seed[32];
	zend_result result = FAILURE;

	secp256k1_ctx = secp256k1_context_create(SECP256K1_CONTEXT_NONE);
	if (secp256k1_ctx == NULL) {
		goto release;
	}

	if (php_random_bytes_throw(seed, sizeof(seed)) == FAILURE) {
		goto release;
	}

	if (!secp256k1_context_randomize(secp256k1_ctx, seed)) {
		goto release;
	}

	secp256k1_pubkey_ce = register_class_secp256k1_pubkey();
	secp256k1_pubkey_ce->create_object = secp256k1_pubkey_deny_new;

	memcpy(&secp256k1_pubkey_handlers, zend_get_std_object_handlers(), sizeof(zend_object_handlers));
	secp256k1_pubkey_handlers.offset = offsetof(secp256k1_pubkey_obj, std);
	secp256k1_pubkey_handlers.free_obj = secp256k1_pubkey_free_object;
	secp256k1_pubkey_handlers.clone_obj = NULL;

	secp256k1_ecdsa_sig_ce = register_class_secp256k1_ecdsa_signature();
	secp256k1_ecdsa_sig_ce->create_object = secp256k1_ecdsa_sig_deny_new;

	memcpy(&secp256k1_ecdsa_sig_handlers, zend_get_std_object_handlers(), sizeof(zend_object_handlers));
	secp256k1_ecdsa_sig_handlers.offset = offsetof(secp256k1_ecdsa_sig_obj, std);
	secp256k1_ecdsa_sig_handlers.free_obj = secp256k1_ecdsa_sig_free_object;
	secp256k1_ecdsa_sig_handlers.clone_obj = NULL;

	register_secp256k1_symbols(module_number);

	result = SUCCESS;

release:
	explicit_bzero(seed, sizeof(seed));

	if (result == FAILURE && secp256k1_ctx != NULL) {
		secp256k1_context_destroy(secp256k1_ctx);
		secp256k1_ctx = NULL;
	}

	return result;
}

PHP_MSHUTDOWN_FUNCTION(secp256k1)
{
	if (secp256k1_ctx != NULL) {
		secp256k1_context_destroy(secp256k1_ctx);
		secp256k1_ctx = NULL;
	}

	return SUCCESS;
}

PHP_RINIT_FUNCTION(secp256k1)
{
#if defined(ZTS) && defined(COMPILE_DL_SECP256K1)
	ZEND_TSRMLS_CACHE_UPDATE();
#endif

	return SUCCESS;
}

PHP_MINFO_FUNCTION(secp256k1)
{
	php_info_print_table_start();
	php_info_print_table_row(2, "secp256k1 support", "enabled");
	php_info_print_table_end();
}

zend_module_entry secp256k1_module_entry = {
	STANDARD_MODULE_HEADER,
	"secp256k1",					/* Extension name */
	ext_functions,					/* zend_function_entry */
	PHP_MINIT(secp256k1),			/* PHP_MINIT - Module initialization */
	PHP_MSHUTDOWN(secp256k1),		/* PHP_MSHUTDOWN - Module shutdown */
	PHP_RINIT(secp256k1),			/* PHP_RINIT - Request initialization */
	NULL,							/* PHP_RSHUTDOWN - Request shutdown */
	PHP_MINFO(secp256k1),			/* PHP_MINFO - Module info */
	PHP_SECP256K1_VERSION,			/* Version */
	STANDARD_MODULE_PROPERTIES
};

#ifdef COMPILE_DL_SECP256K1
# ifdef ZTS
ZEND_TSRMLS_CACHE_DEFINE()
# endif
ZEND_GET_MODULE(secp256k1)
#endif
