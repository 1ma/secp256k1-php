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

PHP_FUNCTION(secp256k1_silentpayments_recipient_label_create)
{
	char *scan_key;
	size_t scan_key_len;
	zend_long m;
	zval *tweak_zval;
	unsigned char label_tweak[32];

	ZEND_PARSE_PARAMETERS_START(3, 3)
		Z_PARAM_STRING(scan_key, scan_key_len)
		Z_PARAM_LONG(m)
		Z_PARAM_ZVAL(tweak_zval)
	ZEND_PARSE_PARAMETERS_END();

	if (scan_key_len != 32) {
		zend_argument_value_error(1, "must be exactly 32 bytes");
		RETURN_THROWS();
	}

	if (m < 0 || m > UINT32_MAX) {
		zend_argument_value_error(2, "must be between 0 and 2^32-1");
		RETURN_THROWS();
	}

	zend_object *obj = secp256k1_sp_label_create_object(secp256k1_sp_label_ce);
	secp256k1_sp_label_obj *intern = secp256k1_sp_label_from_obj(obj);

	RETVAL_FALSE;
	if (secp256k1_silentpayments_recipient_label_create(
		SECP256K1_G(ctx), &intern->label, label_tweak,
		(const unsigned char *)scan_key, (uint32_t)m
	)) {
		ZEND_TRY_ASSIGN_REF_STRINGL(tweak_zval, (char *)label_tweak, 32);
		RETVAL_OBJ(obj);
	} else {
		zend_object_release(obj);
	}

	explicit_bzero(label_tweak, sizeof(label_tweak));
}

PHP_FUNCTION(secp256k1_silentpayments_recipient_label_serialize)
{
	zval *label_zval;
	unsigned char out[33];

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_OBJECT_OF_CLASS(label_zval, secp256k1_sp_label_ce)
	ZEND_PARSE_PARAMETERS_END();

	secp256k1_sp_label_obj *intern = secp256k1_sp_label_from_obj(Z_OBJ_P(label_zval));

	secp256k1_silentpayments_recipient_label_serialize(
		SECP256K1_G(ctx), out, &intern->label
	);

	RETURN_STRINGL((char *)out, 33);
}

PHP_FUNCTION(secp256k1_silentpayments_recipient_label_parse)
{
	char *in;
	size_t in_len;

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_STRING(in, in_len)
	ZEND_PARSE_PARAMETERS_END();

	if (in_len != 33) {
		zend_argument_value_error(1, "must be exactly 33 bytes");
		RETURN_THROWS();
	}

	zend_object *obj = secp256k1_sp_label_create_object(secp256k1_sp_label_ce);
	secp256k1_sp_label_obj *intern = secp256k1_sp_label_from_obj(obj);

	if (!secp256k1_silentpayments_recipient_label_parse(
		SECP256K1_G(ctx), &intern->label, (const unsigned char *)in
	)) {
		zend_object_release(obj);
		RETURN_FALSE;
	}

	RETURN_OBJ(obj);
}

PHP_FUNCTION(secp256k1_silentpayments_recipient_create_labeled_spend_pubkey)
{
	zval *spend_zval, *label_zval;

	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_OBJECT_OF_CLASS(spend_zval, secp256k1_pubkey_ce)
		Z_PARAM_OBJECT_OF_CLASS(label_zval, secp256k1_sp_label_ce)
	ZEND_PARSE_PARAMETERS_END();

	secp256k1_pubkey_obj *spend_intern = secp256k1_pubkey_from_obj(Z_OBJ_P(spend_zval));
	secp256k1_sp_label_obj *label_intern = secp256k1_sp_label_from_obj(Z_OBJ_P(label_zval));

	zend_object *obj = secp256k1_pubkey_create_object(secp256k1_pubkey_ce);
	secp256k1_pubkey_obj *result = secp256k1_pubkey_from_obj(obj);

	RETVAL_FALSE;
	if (secp256k1_silentpayments_recipient_create_labeled_spend_pubkey(
		SECP256K1_G(ctx), &result->pubkey, &spend_intern->pubkey, &label_intern->label
	)) {
		RETVAL_OBJ(obj);
	} else {
		zend_object_release(obj);
	}
}

PHP_FUNCTION(secp256k1_silentpayments_recipient_prevouts_summary_create)
{
	char *outpoint;
	size_t outpoint_len;
	HashTable *xonly_ht, *pubkeys_ht;
	zval *entry;
	size_t n_xonly, n_pubkeys, i;

	ZEND_PARSE_PARAMETERS_START(3, 3)
		Z_PARAM_STRING(outpoint, outpoint_len)
		Z_PARAM_ARRAY_HT(xonly_ht)
		Z_PARAM_ARRAY_HT(pubkeys_ht)
	ZEND_PARSE_PARAMETERS_END();

	if (outpoint_len != 36) {
		zend_argument_value_error(1, "must be exactly 36 bytes");
		RETURN_THROWS();
	}

	n_xonly = zend_hash_num_elements(xonly_ht);
	n_pubkeys = zend_hash_num_elements(pubkeys_ht);
	if (n_xonly == 0 && n_pubkeys == 0) {
		zend_argument_value_error(2, "at least one of xonly_pubkeys or pubkeys must be non-empty");
		RETURN_THROWS();
	}

	const secp256k1_xonly_pubkey **xonly_ptrs = NULL;
	if (n_xonly > 0) {
		xonly_ptrs = emalloc(sizeof(secp256k1_xonly_pubkey *) * n_xonly);
		i = 0;
		ZEND_HASH_FOREACH_VAL(xonly_ht, entry) {
			if (Z_TYPE_P(entry) != IS_OBJECT || !instanceof_function(Z_OBJCE_P(entry), secp256k1_xonly_pubkey_ce)) {
				efree(xonly_ptrs);
				zend_argument_type_error(2, "must contain only secp256k1_xonly_pubkey objects");
				RETURN_THROWS();
			}
			xonly_ptrs[i++] = &secp256k1_xonly_pubkey_from_obj(Z_OBJ_P(entry))->xonly_pubkey;
		} ZEND_HASH_FOREACH_END();
	}

	const secp256k1_pubkey **pubkey_ptrs = NULL;
	if (n_pubkeys > 0) {
		pubkey_ptrs = emalloc(sizeof(secp256k1_pubkey *) * n_pubkeys);
		i = 0;
		ZEND_HASH_FOREACH_VAL(pubkeys_ht, entry) {
			if (Z_TYPE_P(entry) != IS_OBJECT || !instanceof_function(Z_OBJCE_P(entry), secp256k1_pubkey_ce)) {
				if (xonly_ptrs) efree(xonly_ptrs);
				efree(pubkey_ptrs);
				zend_argument_type_error(3, "must contain only secp256k1_pubkey objects");
				RETURN_THROWS();
			}
			pubkey_ptrs[i++] = &secp256k1_pubkey_from_obj(Z_OBJ_P(entry))->pubkey;
		} ZEND_HASH_FOREACH_END();
	}

	zend_object *obj = secp256k1_sp_prevouts_create_object(secp256k1_sp_prevouts_ce);
	secp256k1_sp_prevouts_obj *intern = secp256k1_sp_prevouts_from_obj(obj);

	RETVAL_FALSE;
	if (secp256k1_silentpayments_recipient_prevouts_summary_create(
		SECP256K1_G(ctx), &intern->summary,
		(const unsigned char *)outpoint,
		xonly_ptrs, n_xonly,
		pubkey_ptrs, n_pubkeys
	)) {
		RETVAL_OBJ(obj);
	} else {
		zend_object_release(obj);
	}

	if (xonly_ptrs) efree(xonly_ptrs);
	if (pubkey_ptrs) efree(pubkey_ptrs);
}

static const unsigned char *sp_label_lookup(const unsigned char *label33, const void *label_context)
{
	HashTable *ht = (HashTable *)label_context;
	zval *found = zend_hash_str_find(ht, (const char *)label33, 33);
	if (found && Z_TYPE_P(found) == IS_STRING && Z_STRLEN_P(found) == 32) {
		return (const unsigned char *)Z_STRVAL_P(found);
	}
	return NULL;
}

PHP_FUNCTION(secp256k1_silentpayments_recipient_scan_outputs)
{
	char *scan_key;
	size_t scan_key_len;
	zval *spend_zval, *prevouts_zval;
	HashTable *tx_outputs_ht;
	HashTable *labels_ht = NULL;
	zval *entry;
	size_t n_outputs, i;

	ZEND_PARSE_PARAMETERS_START(4, 5)
		Z_PARAM_STRING(scan_key, scan_key_len)
		Z_PARAM_OBJECT_OF_CLASS(spend_zval, secp256k1_pubkey_ce)
		Z_PARAM_OBJECT_OF_CLASS(prevouts_zval, secp256k1_sp_prevouts_ce)
		Z_PARAM_ARRAY_HT(tx_outputs_ht)
		Z_PARAM_OPTIONAL
		Z_PARAM_ARRAY_HT_OR_NULL(labels_ht)
	ZEND_PARSE_PARAMETERS_END();

	if (scan_key_len != 32) {
		zend_argument_value_error(1, "must be exactly 32 bytes");
		RETURN_THROWS();
	}

	n_outputs = zend_hash_num_elements(tx_outputs_ht);
	if (n_outputs == 0) {
		zend_argument_value_error(4, "must not be empty");
		RETURN_THROWS();
	}

	const secp256k1_xonly_pubkey **output_ptrs = emalloc(sizeof(secp256k1_xonly_pubkey *) * n_outputs);
	i = 0;
	ZEND_HASH_FOREACH_VAL(tx_outputs_ht, entry) {
		if (Z_TYPE_P(entry) != IS_OBJECT || !instanceof_function(Z_OBJCE_P(entry), secp256k1_xonly_pubkey_ce)) {
			efree(output_ptrs);
			zend_argument_type_error(4, "must contain only secp256k1_xonly_pubkey objects");
			RETURN_THROWS();
		}
		output_ptrs[i++] = &secp256k1_xonly_pubkey_from_obj(Z_OBJ_P(entry))->xonly_pubkey;
	} ZEND_HASH_FOREACH_END();

	secp256k1_pubkey_obj *spend_intern = secp256k1_pubkey_from_obj(Z_OBJ_P(spend_zval));
	secp256k1_sp_prevouts_obj *prevouts_intern = secp256k1_sp_prevouts_from_obj(Z_OBJ_P(prevouts_zval));

	secp256k1_silentpayments_found_output *found = emalloc(sizeof(secp256k1_silentpayments_found_output) * n_outputs);
	secp256k1_silentpayments_found_output **found_ptrs = emalloc(sizeof(secp256k1_silentpayments_found_output *) * n_outputs);
	for (i = 0; i < n_outputs; i++) {
		found_ptrs[i] = &found[i];
	}

	uint32_t n_found = 0;

	RETVAL_FALSE;
	if (secp256k1_silentpayments_recipient_scan_outputs(
		SECP256K1_G(ctx),
		found_ptrs, &n_found,
		output_ptrs, n_outputs,
		(const unsigned char *)scan_key,
		&prevouts_intern->summary,
		&spend_intern->pubkey,
		labels_ht ? sp_label_lookup : NULL,
		labels_ht
	)) {
		array_init_size(return_value, n_found);
		for (i = 0; i < n_found; i++) {
			zval item;
			array_init_size(&item, 4);

			zend_object *out_obj = secp256k1_xonly_pubkey_create_object(secp256k1_xonly_pubkey_ce);
			secp256k1_xonly_pubkey_obj *out_intern = secp256k1_xonly_pubkey_from_obj(out_obj);
			out_intern->xonly_pubkey = found[i].output;
			zval out_zval;
			ZVAL_OBJ(&out_zval, out_obj);
			zend_hash_str_add_new(Z_ARRVAL(item), "output", sizeof("output") - 1, &out_zval);

			zval tweak_zval;
			ZVAL_STRINGL(&tweak_zval, (char *)found[i].tweak, 32);
			zend_hash_str_add_new(Z_ARRVAL(item), "tweak", sizeof("tweak") - 1, &tweak_zval);

			zval label_found_zval;
			ZVAL_BOOL(&label_found_zval, found[i].found_with_label);
			zend_hash_str_add_new(Z_ARRVAL(item), "found_with_label", sizeof("found_with_label") - 1, &label_found_zval);

			if (found[i].found_with_label) {
				zend_object *label_obj = secp256k1_sp_label_create_object(secp256k1_sp_label_ce);
				secp256k1_sp_label_obj *label_intern = secp256k1_sp_label_from_obj(label_obj);
				label_intern->label = found[i].label;
				zval label_zval;
				ZVAL_OBJ(&label_zval, label_obj);
				zend_hash_str_add_new(Z_ARRVAL(item), "label", sizeof("label") - 1, &label_zval);
			} else {
				zval null_zval;
				ZVAL_NULL(&null_zval);
				zend_hash_str_add_new(Z_ARRVAL(item), "label", sizeof("label") - 1, &null_zval);
			}

			add_next_index_zval(return_value, &item);
		}
	}

	explicit_bzero(found, sizeof(secp256k1_silentpayments_found_output) * n_outputs);
	efree(found_ptrs);
	efree(found);
	efree(output_ptrs);
}
