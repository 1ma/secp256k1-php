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
#include "secp256k1_arginfo.h"

#include <secp256k1.h>

static secp256k1_context *secp256k1_ctx = NULL;

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
