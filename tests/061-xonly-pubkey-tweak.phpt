--TEST--
secp256k1_xonly_pubkey_tweak_add and tweak_add_check
--EXTENSIONS--
secp256k1
--SKIPIF--
<?php
if (!function_exists('secp256k1_xonly_pubkey_parse')) die('skip extrakeys module not available');
?>
--FILE--
<?php

$sk = str_repeat("\x01", 32);
$pk = secp256k1_ec_pubkey_create($sk);
$xonly = secp256k1_xonly_pubkey_from_pubkey($pk, $parity);
$tweak = str_repeat("\x02", 32);

// tweak_add returns a regular pubkey
$tweaked_pk = secp256k1_xonly_pubkey_tweak_add($xonly, $tweak);
var_dump($tweaked_pk instanceof secp256k1_pubkey);

// convert tweaked pubkey to xonly to get serialization and parity for check
$tweaked_xonly = secp256k1_xonly_pubkey_from_pubkey($tweaked_pk, $tweaked_parity);
$tweaked_ser = secp256k1_xonly_pubkey_serialize($tweaked_xonly);

// tweak_add_check verifies the relationship
var_dump(secp256k1_xonly_pubkey_tweak_add_check($tweaked_ser, $tweaked_parity, $xonly, $tweak));

// wrong tweak fails check
$wrong_tweak = str_repeat("\x03", 32);
var_dump(secp256k1_xonly_pubkey_tweak_add_check($tweaked_ser, $tweaked_parity, $xonly, $wrong_tweak));

// wrong parity fails check
var_dump(secp256k1_xonly_pubkey_tweak_add_check($tweaked_ser, $tweaked_parity ^ 1, $xonly, $tweak));

// tweak_add that cancels the xonly pubkey returns false
$effective_sk = $parity === 0 ? $sk : secp256k1_ec_seckey_negate($sk);
$cancel_tweak = secp256k1_ec_seckey_negate($effective_sk);
var_dump(secp256k1_xonly_pubkey_tweak_add($xonly, $cancel_tweak));

// tweak_add with invalid tweak length throws
try {
    secp256k1_xonly_pubkey_tweak_add($xonly, "short");
} catch (ValueError $e) {
    echo $e->getMessage() . "\n";
}

// tweak_add_check with invalid lengths throws
try {
    secp256k1_xonly_pubkey_tweak_add_check("short", 0, $xonly, $tweak);
} catch (ValueError $e) {
    echo $e->getMessage() . "\n";
}

try {
    secp256k1_xonly_pubkey_tweak_add_check($tweaked_ser, 0, $xonly, "short");
} catch (ValueError $e) {
    echo $e->getMessage() . "\n";
}

?>
--EXPECT--
bool(true)
bool(true)
bool(false)
bool(false)
bool(false)
secp256k1_xonly_pubkey_tweak_add(): Argument #2 ($tweak32) must be exactly 32 bytes
secp256k1_xonly_pubkey_tweak_add_check(): Argument #1 ($tweaked_pubkey32) must be exactly 32 bytes
secp256k1_xonly_pubkey_tweak_add_check(): Argument #4 ($tweak32) must be exactly 32 bytes
