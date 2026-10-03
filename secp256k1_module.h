#ifndef SECP256K1_CORE_H
#define SECP256K1_CORE_H

#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#include "php.h"
#include <secp256k1.h>
#ifdef HAVE_SECP256K1_EXTRAKEYS
#include <secp256k1_extrakeys.h>
#endif
#ifdef HAVE_SECP256K1_RECOVERY
#include <secp256k1_recovery.h>
#endif

#define SECP256K1_OPAQUE_TYPE(name, ctype, field)						\
	typedef struct { ctype field; zend_object std; } name##_obj;		\
	zend_object *name##_create_object(zend_class_entry *ce);			\
	static inline name##_obj *name##_from_obj(zend_object *obj) {		\
		return (name##_obj *)((char *)obj - offsetof(name##_obj, std));	\
	}

extern secp256k1_context *secp256k1_ctx;

extern zend_class_entry *secp256k1_pubkey_ce;
extern zend_class_entry *secp256k1_ecdsa_sig_ce;
#ifdef HAVE_SECP256K1_EXTRAKEYS
extern zend_class_entry *secp256k1_xonly_pubkey_ce;
extern zend_class_entry *secp256k1_keypair_ce;
#endif
#ifdef HAVE_SECP256K1_RECOVERY
extern zend_class_entry *secp256k1_ecdsa_recoverable_sig_ce;
#endif

SECP256K1_OPAQUE_TYPE(secp256k1_pubkey, secp256k1_pubkey, pubkey)
SECP256K1_OPAQUE_TYPE(secp256k1_ecdsa_sig, secp256k1_ecdsa_signature, sig)
#ifdef HAVE_SECP256K1_EXTRAKEYS
SECP256K1_OPAQUE_TYPE(secp256k1_xonly_pubkey, secp256k1_xonly_pubkey, xonly_pubkey)
SECP256K1_OPAQUE_TYPE(secp256k1_keypair, secp256k1_keypair, keypair)
#endif
#ifdef HAVE_SECP256K1_RECOVERY
SECP256K1_OPAQUE_TYPE(secp256k1_ecdsa_recoverable_sig, secp256k1_ecdsa_recoverable_signature, sig)
#endif

#endif /* SECP256K1_CORE_H */
