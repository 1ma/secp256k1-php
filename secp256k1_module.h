#ifndef SECP256K1_CORE_H
#define SECP256K1_CORE_H

#define SECP256K1_OPAQUE_TYPE(name, ctype, field)						\
	typedef struct { ctype field; zend_object std; } name##_obj;		\
	zend_object *name##_create_object(zend_class_entry *ce);			\
	static inline name##_obj *name##_from_obj(zend_object *obj) {		\
		return (name##_obj *)((char *)obj - offsetof(name##_obj, std));	\
	}

#include "php.h"
#include <secp256k1.h>

ZEND_BEGIN_MODULE_GLOBALS(secp256k1)
	secp256k1_context *ctx;
ZEND_END_MODULE_GLOBALS(secp256k1)
ZEND_EXTERN_MODULE_GLOBALS(secp256k1)

#define SECP256K1_G(v) ZEND_MODULE_GLOBALS_ACCESSOR(secp256k1, v)

extern zend_class_entry *secp256k1_pubkey_ce;
extern zend_class_entry *secp256k1_ecdsa_sig_ce;
SECP256K1_OPAQUE_TYPE(secp256k1_pubkey, secp256k1_pubkey, pubkey)
SECP256K1_OPAQUE_TYPE(secp256k1_ecdsa_sig, secp256k1_ecdsa_signature, sig)

#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#ifdef HAVE_SECP256K1_EXTRAKEYS
# include <secp256k1_extrakeys.h>
extern zend_class_entry *secp256k1_xonly_pubkey_ce;
extern zend_class_entry *secp256k1_keypair_ce;
SECP256K1_OPAQUE_TYPE(secp256k1_xonly_pubkey, secp256k1_xonly_pubkey, xonly_pubkey)
SECP256K1_OPAQUE_TYPE(secp256k1_keypair, secp256k1_keypair, keypair)
#endif

#ifdef HAVE_SECP256K1_RECOVERY
# include <secp256k1_recovery.h>
extern zend_class_entry *secp256k1_ecdsa_recoverable_sig_ce;
SECP256K1_OPAQUE_TYPE(secp256k1_ecdsa_recoverable_sig, secp256k1_ecdsa_recoverable_signature, sig)
#endif

#ifdef HAVE_SECP256K1_ELLSWIFT
# define SECP256K1_ELLSWIFT_XDH_HASH_BIP324 0
# define SECP256K1_ELLSWIFT_XDH_HASH_PREFIX 1
#endif

#endif /* SECP256K1_CORE_H */
