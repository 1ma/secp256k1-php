#ifndef SECP256K1_CORE_H
#define SECP256K1_CORE_H

#include "php.h"
#include <secp256k1.h>

extern secp256k1_context *secp256k1_ctx;

extern zend_class_entry *secp256k1_pubkey_ce;

typedef struct {
	secp256k1_pubkey pubkey;
	zend_object std;
} secp256k1_pubkey_obj;

static inline secp256k1_pubkey_obj *secp256k1_pubkey_from_obj(zend_object *obj)
{
	return (secp256k1_pubkey_obj *)((char *)obj - offsetof(secp256k1_pubkey_obj, std));
}

#endif /* SECP256K1_CORE_H */
