--TEST--
secp256k1_musig key aggregation and tweaking
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

// EC tweak: output matches ec_pubkey_tweak_add on the aggregate pubkey
$tweak = str_repeat("\xAB", 32);
$cache_ec = secp256k1_musig_pubkey_agg([$pk1, $pk2]);
$agg_pk_before = secp256k1_musig_pubkey_get($cache_ec);
$tweaked_pk = secp256k1_musig_pubkey_ec_tweak_add($cache_ec, $tweak);
var_dump($tweaked_pk instanceof secp256k1_pubkey);

// Verify equivalence with ec_pubkey_tweak_add
$agg_pk_copy = secp256k1_musig_pubkey_get(secp256k1_musig_pubkey_agg([$pk1, $pk2]));
secp256k1_ec_pubkey_tweak_add($agg_pk_copy, $tweak);
var_dump(secp256k1_ec_pubkey_cmp($tweaked_pk, $agg_pk_copy) === 0);

// X-only tweak: verify with xonly_pubkey_tweak_add_check
$cache_xonly = secp256k1_musig_pubkey_agg([$pk1, $pk2]);
$agg_xonly = secp256k1_xonly_pubkey_from_pubkey(secp256k1_musig_pubkey_get($cache_xonly), $parity);
$agg_xonly_ser = secp256k1_xonly_pubkey_serialize($agg_xonly);
$tweaked_pk = secp256k1_musig_pubkey_xonly_tweak_add($cache_xonly, $tweak);
var_dump($tweaked_pk instanceof secp256k1_pubkey);
$tweaked_xonly = secp256k1_xonly_pubkey_from_pubkey($tweaked_pk, $tweaked_parity);
$tweaked_ser = secp256k1_xonly_pubkey_serialize($tweaked_xonly);
var_dump(secp256k1_xonly_pubkey_tweak_add_check($tweaked_ser, $tweaked_parity, $agg_xonly, $tweak));

// Error: wrong tweak length
try {
    secp256k1_musig_pubkey_ec_tweak_add($cache, "short");
} catch (ValueError $e) {
    echo $e->getMessage() . "\n";
}

try {
    secp256k1_musig_pubkey_xonly_tweak_add($cache, "short");
} catch (ValueError $e) {
    echo $e->getMessage() . "\n";
}

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

// Parse errors: wrong input length
try {
    secp256k1_musig_pubnonce_parse("short");
} catch (ValueError $e) {
    echo $e->getMessage() . "\n";
}

try {
    secp256k1_musig_aggnonce_parse("short");
} catch (ValueError $e) {
    echo $e->getMessage() . "\n";
}

try {
    secp256k1_musig_partial_sig_parse("short");
} catch (ValueError $e) {
    echo $e->getMessage() . "\n";
}

// Opaque: cannot instantiate
$opaque_classes = [
    'secp256k1_musig_keyagg_cache',
    'secp256k1_musig_secnonce',
    'secp256k1_musig_pubnonce',
    'secp256k1_musig_aggnonce',
    'secp256k1_musig_session',
    'secp256k1_musig_partial_sig',
];
foreach ($opaque_classes as $class) {
    try {
        new $class();
    } catch (Error $e) {
        echo $e->getMessage() . "\n";
    }
}

?>
--EXPECT--
bool(true)
bool(true)
bool(true)
bool(true)
bool(true)
bool(true)
bool(true)
bool(true)
bool(true)
secp256k1_musig_pubkey_ec_tweak_add(): Argument #2 ($tweak32) must be exactly 32 bytes
secp256k1_musig_pubkey_xonly_tweak_add(): Argument #2 ($tweak32) must be exactly 32 bytes
secp256k1_musig_pubkey_agg(): Argument #1 ($pubkeys) must not be empty
secp256k1_musig_pubkey_agg(): Argument #1 ($pubkeys) must contain only secp256k1_pubkey objects
secp256k1_musig_pubnonce_parse(): Argument #1 ($in66) must be exactly 66 bytes
secp256k1_musig_aggnonce_parse(): Argument #1 ($in66) must be exactly 66 bytes
secp256k1_musig_partial_sig_parse(): Argument #1 ($in32) must be exactly 32 bytes
Cannot instantiate secp256k1_musig_keyagg_cache directly, use secp256k1_musig_pubkey_agg()
Cannot instantiate secp256k1_musig_secnonce directly, use secp256k1_musig_nonce_gen() or secp256k1_musig_nonce_gen_counter()
Cannot instantiate secp256k1_musig_pubnonce directly, use secp256k1_musig_nonce_gen() or secp256k1_musig_nonce_gen_counter()
Cannot instantiate secp256k1_musig_aggnonce directly, use secp256k1_musig_nonce_agg()
Cannot instantiate secp256k1_musig_session directly, use secp256k1_musig_nonce_process()
Cannot instantiate secp256k1_musig_partial_sig directly, use secp256k1_musig_partial_sign()
