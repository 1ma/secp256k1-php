--TEST--
secp256k1_musig key aggregation tweaking
--EXTENSIONS--
secp256k1
--SKIPIF--
<?php if (!function_exists('secp256k1_musig_pubkey_agg')) die('skip musig not available'); ?>
--FILE--
<?php

$pk1 = secp256k1_ec_pubkey_create(str_repeat("\x01", 32));
$pk2 = secp256k1_ec_pubkey_create(str_repeat("\x02", 32));
$tweak = str_repeat("\xAB", 32);

// EC tweak: output matches ec_pubkey_tweak_add on the aggregate pubkey
$cache = secp256k1_musig_pubkey_agg([$pk1, $pk2]);
$tweaked_pk = secp256k1_musig_pubkey_ec_tweak_add($cache, $tweak);
var_dump($tweaked_pk instanceof secp256k1_pubkey);

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

?>
--EXPECT--
bool(true)
bool(true)
bool(true)
bool(true)
secp256k1_musig_pubkey_ec_tweak_add(): Argument #2 ($tweak32) must be exactly 32 bytes
secp256k1_musig_pubkey_xonly_tweak_add(): Argument #2 ($tweak32) must be exactly 32 bytes
