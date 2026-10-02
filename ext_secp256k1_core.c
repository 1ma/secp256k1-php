/* secp256k1 extension for PHP */

#include "php.h"
#include "ext/standard/info.h"
#if PHP_VERSION_ID >= 80400
# include "ext/random/php_random_csprng.h"
#else
# include "ext/random/php_random.h"
#endif
#include "php_secp256k1.h"
#include "ext_secp256k1_core.h"

#include "secp256k1_arginfo.h"

secp256k1_context *secp256k1_ctx = NULL;

zend_class_entry *secp256k1_pubkey_ce;
zend_class_entry *secp256k1_ecdsa_sig_ce;
#ifdef HAVE_SECP256K1_EXTRAKEYS
zend_class_entry *secp256k1_xonly_pubkey_ce;
zend_class_entry *secp256k1_keypair_ce;
#endif

static zend_object_handlers secp256k1_pubkey_handlers;
static zend_object_handlers secp256k1_ecdsa_sig_handlers;
#ifdef HAVE_SECP256K1_EXTRAKEYS
static zend_object_handlers secp256k1_xonly_pubkey_handlers;
static zend_object_handlers secp256k1_keypair_handlers;
#endif

zend_object *secp256k1_pubkey_create_object(zend_class_entry *ce)
{
	secp256k1_pubkey_obj *intern = zend_object_alloc(sizeof(secp256k1_pubkey_obj), ce);

	zend_object_std_init(&intern->std, ce);
	intern->std.handlers = &secp256k1_pubkey_handlers;

	return &intern->std;
}

zend_object *secp256k1_ecdsa_sig_create_object(zend_class_entry *ce)
{
	secp256k1_ecdsa_sig_obj *intern = zend_object_alloc(sizeof(secp256k1_ecdsa_sig_obj), ce);

	zend_object_std_init(&intern->std, ce);
	intern->std.handlers = &secp256k1_ecdsa_sig_handlers;

	return &intern->std;
}

#ifdef HAVE_SECP256K1_EXTRAKEYS
zend_object *secp256k1_xonly_pubkey_create_object(zend_class_entry *ce)
{
	secp256k1_xonly_pubkey_obj *intern = zend_object_alloc(sizeof(secp256k1_xonly_pubkey_obj), ce);

	zend_object_std_init(&intern->std, ce);
	intern->std.handlers = &secp256k1_xonly_pubkey_handlers;

	return &intern->std;
}

zend_object *secp256k1_keypair_create_object(zend_class_entry *ce)
{
	secp256k1_keypair_obj *intern = zend_object_alloc(sizeof(secp256k1_keypair_obj), ce);

	zend_object_std_init(&intern->std, ce);
	intern->std.handlers = &secp256k1_keypair_handlers;

	return &intern->std;
}
#endif

static zend_object *secp256k1_pubkey_deny_new(zend_class_entry *ce)
{
	zend_throw_error(NULL, "Cannot instantiate %s directly, use secp256k1_ec_pubkey_create() or secp256k1_ec_pubkey_parse()", ZSTR_VAL(ce->name));
	return zend_objects_new(ce);
}

static zend_object *secp256k1_ecdsa_sig_deny_new(zend_class_entry *ce)
{
	zend_throw_error(NULL, "Cannot instantiate %s directly, use secp256k1_ecdsa_signature_parse_compact() or secp256k1_ecdsa_signature_parse_der()", ZSTR_VAL(ce->name));
	return zend_objects_new(ce);
}

#ifdef HAVE_SECP256K1_EXTRAKEYS
static zend_object *secp256k1_xonly_pubkey_deny_new(zend_class_entry *ce)
{
	zend_throw_error(NULL, "Cannot instantiate %s directly, use secp256k1_xonly_pubkey_parse() or secp256k1_xonly_pubkey_from_pubkey()", ZSTR_VAL(ce->name));
	return zend_objects_new(ce);
}

static zend_object *secp256k1_keypair_deny_new(zend_class_entry *ce)
{
	zend_throw_error(NULL, "Cannot instantiate %s directly, use secp256k1_keypair_create()", ZSTR_VAL(ce->name));
	return zend_objects_new(ce);
}
#endif

static void secp256k1_pubkey_free_object(zend_object *obj)
{
	secp256k1_pubkey_obj *intern = secp256k1_pubkey_from_obj(obj);

	explicit_bzero(&intern->pubkey, sizeof(secp256k1_pubkey));
	zend_object_std_dtor(&intern->std);
}

static void secp256k1_ecdsa_sig_free_object(zend_object *obj)
{
	secp256k1_ecdsa_sig_obj *intern = secp256k1_ecdsa_sig_from_obj(obj);

	explicit_bzero(&intern->sig, sizeof(secp256k1_ecdsa_signature));
	zend_object_std_dtor(&intern->std);
}

#ifdef HAVE_SECP256K1_EXTRAKEYS
static void secp256k1_xonly_pubkey_free_object(zend_object *obj)
{
	secp256k1_xonly_pubkey_obj *intern = secp256k1_xonly_pubkey_from_obj(obj);

	explicit_bzero(&intern->xonly_pubkey, sizeof(secp256k1_xonly_pubkey));
	zend_object_std_dtor(&intern->std);
}

static void secp256k1_keypair_free_object(zend_object *obj)
{
	secp256k1_keypair_obj *intern = secp256k1_keypair_from_obj(obj);

	explicit_bzero(&intern->keypair, sizeof(secp256k1_keypair));
	zend_object_std_dtor(&intern->std);
}
#endif

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

#ifdef HAVE_SECP256K1_EXTRAKEYS
	secp256k1_xonly_pubkey_ce = register_class_secp256k1_xonly_pubkey();
	secp256k1_xonly_pubkey_ce->create_object = secp256k1_xonly_pubkey_deny_new;

	memcpy(&secp256k1_xonly_pubkey_handlers, zend_get_std_object_handlers(), sizeof(zend_object_handlers));
	secp256k1_xonly_pubkey_handlers.offset = offsetof(secp256k1_xonly_pubkey_obj, std);
	secp256k1_xonly_pubkey_handlers.free_obj = secp256k1_xonly_pubkey_free_object;
	secp256k1_xonly_pubkey_handlers.clone_obj = NULL;

	secp256k1_keypair_ce = register_class_secp256k1_keypair();
	secp256k1_keypair_ce->create_object = secp256k1_keypair_deny_new;

	memcpy(&secp256k1_keypair_handlers, zend_get_std_object_handlers(), sizeof(zend_object_handlers));
	secp256k1_keypair_handlers.offset = offsetof(secp256k1_keypair_obj, std);
	secp256k1_keypair_handlers.free_obj = secp256k1_keypair_free_object;
	secp256k1_keypair_handlers.clone_obj = NULL;
#endif

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
	php_info_print_table_row(2, "secp256k1 version", SECP256K1_LIB_VERSION);
#ifdef HAVE_SECP256K1_ECDH
	php_info_print_table_row(2, "ecdh module", "enabled");
#else
	php_info_print_table_row(2, "ecdh module", "disabled");
#endif
	php_info_print_table_row(2, "recovery module", "disabled");
#ifdef HAVE_SECP256K1_EXTRAKEYS
	php_info_print_table_row(2, "extrakeys module", "enabled");
#else
	php_info_print_table_row(2, "extrakeys module", "disabled");
#endif
	php_info_print_table_row(2, "schnorrsig module", "disabled");
	php_info_print_table_row(2, "musig module", "disabled");
	php_info_print_table_row(2, "ellswift module", "disabled");
	php_info_print_table_row(2, "silentpayments module", "disabled");
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
