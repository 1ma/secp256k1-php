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

extern secp256k1_context *secp256k1_ctx;

extern zend_class_entry *secp256k1_pubkey_ce;
extern zend_class_entry *secp256k1_ecdsa_sig_ce;
#ifdef HAVE_SECP256K1_EXTRAKEYS
extern zend_class_entry *secp256k1_xonly_pubkey_ce;
extern zend_class_entry *secp256k1_keypair_ce;
#endif

zend_object *secp256k1_pubkey_create_object(zend_class_entry *ce);
zend_object *secp256k1_ecdsa_sig_create_object(zend_class_entry *ce);
#ifdef HAVE_SECP256K1_EXTRAKEYS
zend_object *secp256k1_xonly_pubkey_create_object(zend_class_entry *ce);
zend_object *secp256k1_keypair_create_object(zend_class_entry *ce);
#endif

typedef struct {
	secp256k1_pubkey pubkey;
	zend_object std;
} secp256k1_pubkey_obj;

typedef struct {
	secp256k1_ecdsa_signature sig;
	zend_object std;
} secp256k1_ecdsa_sig_obj;

#ifdef HAVE_SECP256K1_EXTRAKEYS
typedef struct {
	secp256k1_xonly_pubkey xonly_pubkey;
	zend_object std;
} secp256k1_xonly_pubkey_obj;

typedef struct {
	secp256k1_keypair keypair;
	zend_object std;
} secp256k1_keypair_obj;
#endif

static inline secp256k1_pubkey_obj *secp256k1_pubkey_from_obj(zend_object *obj)
{
	return (secp256k1_pubkey_obj *)((char *)obj - offsetof(secp256k1_pubkey_obj, std));
}

static inline secp256k1_ecdsa_sig_obj *secp256k1_ecdsa_sig_from_obj(zend_object *obj)
{
	return (secp256k1_ecdsa_sig_obj *)((char *)obj - offsetof(secp256k1_ecdsa_sig_obj, std));
}

#ifdef HAVE_SECP256K1_EXTRAKEYS
static inline secp256k1_xonly_pubkey_obj *secp256k1_xonly_pubkey_from_obj(zend_object *obj)
{
	return (secp256k1_xonly_pubkey_obj *)((char *)obj - offsetof(secp256k1_xonly_pubkey_obj, std));
}

static inline secp256k1_keypair_obj *secp256k1_keypair_from_obj(zend_object *obj)
{
	return (secp256k1_keypair_obj *)((char *)obj - offsetof(secp256k1_keypair_obj, std));
}
#endif

#endif /* SECP256K1_CORE_H */
