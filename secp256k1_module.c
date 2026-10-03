/* secp256k1 extension for PHP */

#include "php.h"
#include "ext/standard/info.h"
#if PHP_VERSION_ID >= 80400
# include "ext/random/php_random_csprng.h"
#else
# include "ext/random/php_random.h"
#endif
#include "php_secp256k1.h"
#include "secp256k1_module.h"

#include "secp256k1_arginfo.h"

secp256k1_context *secp256k1_ctx = NULL;

#define SECP256K1_OPAQUE_HANDLERS(name, ctype, field)						\
	static zend_object_handlers name##_handlers;							\
	zend_object *name##_create_object(zend_class_entry *ce) {				\
		name##_obj *intern = zend_object_alloc(sizeof(name##_obj), ce);		\
		zend_object_std_init(&intern->std, ce);								\
		intern->std.handlers = &name##_handlers;							\
		return &intern->std;												\
	}																		\
	static void name##_free_object(zend_object *obj) {						\
		name##_obj *intern = name##_from_obj(obj);							\
		explicit_bzero(&intern->field, sizeof(ctype));						\
		zend_object_std_dtor(&intern->std);									\
	}

#define SECP256K1_DENY_NEW(cls, hint)										\
	static zend_object *cls##_deny_new(zend_class_entry *_ce) {				\
		zend_throw_error(NULL,												\
			"Cannot instantiate %s directly, use %s", ZSTR_VAL(_ce->name),	\
			hint);															\
		return zend_objects_new(_ce);										\
	}

#define SECP256K1_REGISTER_CLASS(name, register_fn)							\
	name##_ce = register_fn();												\
	name##_ce->create_object = name##_deny_new;								\
	memcpy(&name##_handlers, zend_get_std_object_handlers(),				\
		sizeof(zend_object_handlers));										\
	name##_handlers.offset = offsetof(name##_obj, std);						\
	name##_handlers.free_obj = name##_free_object;							\
	name##_handlers.clone_obj = NULL

zend_class_entry *secp256k1_pubkey_ce;
zend_class_entry *secp256k1_ecdsa_sig_ce;
#ifdef HAVE_SECP256K1_EXTRAKEYS
zend_class_entry *secp256k1_xonly_pubkey_ce;
zend_class_entry *secp256k1_keypair_ce;
#endif
#ifdef HAVE_SECP256K1_RECOVERY
zend_class_entry *secp256k1_ecdsa_recoverable_sig_ce;
#endif

SECP256K1_OPAQUE_HANDLERS(secp256k1_pubkey, secp256k1_pubkey, pubkey)
SECP256K1_OPAQUE_HANDLERS(secp256k1_ecdsa_sig, secp256k1_ecdsa_signature, sig)
SECP256K1_DENY_NEW(secp256k1_pubkey,
	"secp256k1_ec_pubkey_create() or secp256k1_ec_pubkey_parse()")
SECP256K1_DENY_NEW(secp256k1_ecdsa_sig,
	"secp256k1_ecdsa_signature_parse_compact() or secp256k1_ecdsa_signature_parse_der()")

#ifdef HAVE_SECP256K1_EXTRAKEYS
SECP256K1_OPAQUE_HANDLERS(secp256k1_xonly_pubkey, secp256k1_xonly_pubkey, xonly_pubkey)
SECP256K1_OPAQUE_HANDLERS(secp256k1_keypair, secp256k1_keypair, keypair)
SECP256K1_DENY_NEW(secp256k1_xonly_pubkey,
	"secp256k1_xonly_pubkey_parse() or secp256k1_xonly_pubkey_from_pubkey()")
SECP256K1_DENY_NEW(secp256k1_keypair,
	"secp256k1_keypair_create()")
#endif

#ifdef HAVE_SECP256K1_RECOVERY
SECP256K1_OPAQUE_HANDLERS(secp256k1_ecdsa_recoverable_sig, secp256k1_ecdsa_recoverable_signature, sig)
SECP256K1_DENY_NEW(secp256k1_ecdsa_recoverable_sig,
	"secp256k1_ecdsa_recoverable_signature_parse_compact() or secp256k1_ecdsa_sign_recoverable()")
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

	SECP256K1_REGISTER_CLASS(secp256k1_pubkey, register_class_secp256k1_pubkey);
	SECP256K1_REGISTER_CLASS(secp256k1_ecdsa_sig, register_class_secp256k1_ecdsa_signature);

#ifdef HAVE_SECP256K1_EXTRAKEYS
	SECP256K1_REGISTER_CLASS(secp256k1_xonly_pubkey, register_class_secp256k1_xonly_pubkey);
	SECP256K1_REGISTER_CLASS(secp256k1_keypair, register_class_secp256k1_keypair);
#endif

#ifdef HAVE_SECP256K1_RECOVERY
	SECP256K1_REGISTER_CLASS(secp256k1_ecdsa_recoverable_sig, register_class_secp256k1_ecdsa_recoverable_signature);
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
	php_info_print_table_row(2, "ecdh module", "On");
#else
	php_info_print_table_row(2, "ecdh module", "Off");
#endif
#ifdef HAVE_SECP256K1_RECOVERY
	php_info_print_table_row(2, "recovery module", "On");
#else
	php_info_print_table_row(2, "recovery module", "Off");
#endif
#ifdef HAVE_SECP256K1_EXTRAKEYS
	php_info_print_table_row(2, "extrakeys module", "On");
#else
	php_info_print_table_row(2, "extrakeys module", "Off");
#endif
#ifdef HAVE_SECP256K1_SCHNORRSIG
	php_info_print_table_row(2, "schnorrsig module", "On");
#else
	php_info_print_table_row(2, "schnorrsig module", "Off");
#endif
	php_info_print_table_row(2, "ellswift module", "Off");
	php_info_print_table_row(2, "musig module", "Off");
	php_info_print_table_row(2, "silentpayments module", "Off");
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
