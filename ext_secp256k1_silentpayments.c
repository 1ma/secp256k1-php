#include "secp256k1_module.h"

PHP_FUNCTION(secp256k1_silentpayments_sender_create_outputs)
{
	char *outpoint;
	size_t outpoint_len;
	HashTable *scan_ht, *spend_ht, *keypairs_ht, *seckeys_ht;
	zval *entry;
	size_t n_recipients, n_keypairs, n_seckeys, i;

	ZEND_PARSE_PARAMETERS_START(5, 5)
		Z_PARAM_STRING(outpoint, outpoint_len)
		Z_PARAM_ARRAY_HT(scan_ht)
		Z_PARAM_ARRAY_HT(spend_ht)
		Z_PARAM_ARRAY_HT(keypairs_ht)
		Z_PARAM_ARRAY_HT(seckeys_ht)
	ZEND_PARSE_PARAMETERS_END();

	if (outpoint_len != 36) {
		zend_argument_value_error(1, "must be exactly 36 bytes");
		RETURN_THROWS();
	}

	n_recipients = zend_hash_num_elements(scan_ht);
	if (n_recipients == 0) {
		zend_argument_value_error(2, "must not be empty");
		RETURN_THROWS();
	}
	if (zend_hash_num_elements(spend_ht) != n_recipients) {
		zend_argument_value_error(3, "must have the same length as argument #2");
		RETURN_THROWS();
	}

	n_keypairs = zend_hash_num_elements(keypairs_ht);
	n_seckeys = zend_hash_num_elements(seckeys_ht);
	if (n_keypairs == 0 && n_seckeys == 0) {
		zend_argument_value_error(4, "at least one of keypairs or seckeys must be non-empty");
		RETURN_THROWS();
	}

	secp256k1_silentpayments_recipient *recipients = emalloc(sizeof(secp256k1_silentpayments_recipient) * n_recipients);
	const secp256k1_silentpayments_recipient **recipient_ptrs = emalloc(sizeof(secp256k1_silentpayments_recipient *) * n_recipients);

	i = 0;
	ZEND_HASH_FOREACH_VAL(scan_ht, entry) {
		if (Z_TYPE_P(entry) != IS_OBJECT || !instanceof_function(Z_OBJCE_P(entry), secp256k1_pubkey_ce)) {
			efree(recipients);
			efree(recipient_ptrs);
			zend_argument_type_error(2, "must contain only secp256k1_pubkey objects");
			RETURN_THROWS();
		}
		recipients[i].scan_pubkey = secp256k1_pubkey_from_obj(Z_OBJ_P(entry))->pubkey;
		recipients[i].index = i;
		i++;
	} ZEND_HASH_FOREACH_END();

	i = 0;
	ZEND_HASH_FOREACH_VAL(spend_ht, entry) {
		if (Z_TYPE_P(entry) != IS_OBJECT || !instanceof_function(Z_OBJCE_P(entry), secp256k1_pubkey_ce)) {
			efree(recipients);
			efree(recipient_ptrs);
			zend_argument_type_error(3, "must contain only secp256k1_pubkey objects");
			RETURN_THROWS();
		}
		recipients[i].spend_pubkey = secp256k1_pubkey_from_obj(Z_OBJ_P(entry))->pubkey;
		i++;
	} ZEND_HASH_FOREACH_END();

	for (i = 0; i < n_recipients; i++) {
		recipient_ptrs[i] = &recipients[i];
	}

	const secp256k1_keypair **keypair_ptrs = NULL;
	if (n_keypairs > 0) {
		keypair_ptrs = emalloc(sizeof(secp256k1_keypair *) * n_keypairs);
		i = 0;
		ZEND_HASH_FOREACH_VAL(keypairs_ht, entry) {
			if (Z_TYPE_P(entry) != IS_OBJECT || !instanceof_function(Z_OBJCE_P(entry), secp256k1_keypair_ce)) {
				efree(recipients);
				efree(recipient_ptrs);
				efree(keypair_ptrs);
				zend_argument_type_error(4, "must contain only secp256k1_keypair objects");
				RETURN_THROWS();
			}
			keypair_ptrs[i++] = &secp256k1_keypair_from_obj(Z_OBJ_P(entry))->keypair;
		} ZEND_HASH_FOREACH_END();
	}

	const unsigned char **seckey_ptrs = NULL;
	if (n_seckeys > 0) {
		seckey_ptrs = emalloc(sizeof(unsigned char *) * n_seckeys);
		i = 0;
		ZEND_HASH_FOREACH_VAL(seckeys_ht, entry) {
			if (Z_TYPE_P(entry) != IS_STRING || Z_STRLEN_P(entry) != 32) {
				efree(recipients);
				efree(recipient_ptrs);
				if (keypair_ptrs) efree(keypair_ptrs);
				efree(seckey_ptrs);
				zend_argument_value_error(5, "must contain only 32-byte strings");
				RETURN_THROWS();
			}
			seckey_ptrs[i++] = (const unsigned char *)Z_STRVAL_P(entry);
		} ZEND_HASH_FOREACH_END();
	}

	secp256k1_xonly_pubkey *generated = emalloc(sizeof(secp256k1_xonly_pubkey) * n_recipients);
	secp256k1_xonly_pubkey **generated_ptrs = emalloc(sizeof(secp256k1_xonly_pubkey *) * n_recipients);
	for (i = 0; i < n_recipients; i++) {
		generated_ptrs[i] = &generated[i];
	}

	RETVAL_FALSE;
	if (secp256k1_silentpayments_sender_create_outputs(
		SECP256K1_G(ctx),
		generated_ptrs,
		recipient_ptrs,
		n_recipients,
		(const unsigned char *)outpoint,
		keypair_ptrs,
		n_keypairs,
		seckey_ptrs,
		n_seckeys
	)) {
		array_init_size(return_value, n_recipients);
		for (i = 0; i < n_recipients; i++) {
			zend_object *obj = secp256k1_xonly_pubkey_create_object(secp256k1_xonly_pubkey_ce);
			secp256k1_xonly_pubkey_obj *intern = secp256k1_xonly_pubkey_from_obj(obj);
			intern->xonly_pubkey = generated[i];
			add_next_index_object(return_value, obj);
		}
	}

	efree(generated_ptrs);
	efree(generated);
	efree(recipient_ptrs);
	efree(recipients);
	if (keypair_ptrs) efree(keypair_ptrs);
	if (seckey_ptrs) efree(seckey_ptrs);
}
