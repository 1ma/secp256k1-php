--TEST--
secp256k1_musig_pubkey_agg() and secp256k1_musig_pubkey_get()
--EXTENSIONS--
secp256k1
--SKIPIF--
<?php if (!function_exists('secp256k1_musig_pubkey_agg')) die('skip musig not available'); ?>
--FILE--
<?php

$pk1 = secp256k1_ec_pubkey_create(str_repeat("\x01", 32));
$pk2 = secp256k1_ec_pubkey_create(str_repeat("\x02", 32));

// Basic aggregation
$cache = secp256k1_musig_pubkey_agg([$pk1, $pk2]);
var_dump($cache instanceof secp256k1_musig_keyagg_cache);

// pubkey_get returns a valid pubkey
$agg_pk = secp256k1_musig_pubkey_get($cache);
var_dump($agg_pk instanceof secp256k1_pubkey);

// pubkey_get result serializes to 33 bytes
$ser = secp256k1_ec_pubkey_serialize($agg_pk, SECP256K1_EC_COMPRESSED);
var_dump(strlen($ser) === 33);

// Different order produces different agg_pk
$cache_rev = secp256k1_musig_pubkey_agg([$pk2, $pk1]);
$agg_pk_rev = secp256k1_musig_pubkey_get($cache_rev);
var_dump(secp256k1_ec_pubkey_cmp($agg_pk, $agg_pk_rev) !== 0);

// Single pubkey aggregation works
$cache_single = secp256k1_musig_pubkey_agg([$pk1]);
var_dump($cache_single instanceof secp256k1_musig_keyagg_cache);

// Error: empty array
try {
    secp256k1_musig_pubkey_agg([]);
} catch (ValueError $e) {
    echo $e->getMessage() . "\n";
}

// Error: wrong type in array
try {
    secp256k1_musig_pubkey_agg([$pk1, "not a pubkey"]);
} catch (TypeError $e) {
    echo $e->getMessage() . "\n";
}

// Opaque: cannot instantiate
try {
    new secp256k1_musig_keyagg_cache();
} catch (Error $e) {
    echo $e->getMessage() . "\n";
}

?>
--EXPECT--
bool(true)
bool(true)
bool(true)
bool(true)
bool(true)
secp256k1_musig_pubkey_agg(): Argument #1 ($pubkeys) must not be empty
secp256k1_musig_pubkey_agg(): Argument #1 ($pubkeys) must contain only secp256k1_pubkey objects
Cannot instantiate secp256k1_musig_keyagg_cache directly, use secp256k1_musig_pubkey_agg()
