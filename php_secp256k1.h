/* secp256k1 extension for PHP */

#ifndef PHP_SECP256K1_H
# define PHP_SECP256K1_H

extern zend_module_entry secp256k1_module_entry;
# define phpext_secp256k1_ptr &secp256k1_module_entry

# define PHP_SECP256K1_VERSION "0.1.0"

# if defined(ZTS) && defined(COMPILE_DL_SECP256K1)
ZEND_TSRMLS_CACHE_EXTERN()
# endif

#endif	/* PHP_SECP256K1_H */
