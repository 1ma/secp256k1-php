--TEST--
secp256k1_keypair_xonly_tweak_add
--EXTENSIONS--
secp256k1
--SKIPIF--
<?php
if (!function_exists('secp256k1_keypair_create')) die('skip extrakeys module not available');
?>
--FILE--
<?php

$sk = str_repeat("\x01", 32);
$kp = secp256k1_keypair_create($sk);
$tweak = str_repeat("\x03", 32);

// get original xonly pubkey for comparison
$xonly_before = secp256k1_keypair_xonly_pub($kp, $parity_before);
$ser_before = secp256k1_xonly_pubkey_serialize($xonly_before);

// tweak modifies keypair in-place, returns true
var_dump(secp256k1_keypair_xonly_tweak_add($kp, $tweak));

// xonly pubkey changed after tweak
$xonly_after = secp256k1_keypair_xonly_pub($kp, $parity_after);
$ser_after = secp256k1_xonly_pubkey_serialize($xonly_after);
var_dump($ser_before !== $ser_after);

// the tweaked keypair's xonly must match xonly_pubkey_tweak_add result
$tweaked_pk = secp256k1_xonly_pubkey_tweak_add($xonly_before, $tweak);
$tweaked_xonly = secp256k1_xonly_pubkey_from_pubkey($tweaked_pk, $tweaked_parity);
var_dump(secp256k1_xonly_pubkey_serialize($tweaked_xonly) === $ser_after);

// tweak_add_check passes with the tweaked keypair's pubkey
var_dump(secp256k1_xonly_pubkey_tweak_add_check($ser_after, $parity_after, $xonly_before, $tweak));

// cancelling tweak (effective_sk + tweak = 0) returns false
$kp2 = secp256k1_keypair_create($sk);
secp256k1_keypair_xonly_pub($kp2, $p);
$effective_sk = $p === 0 ? $sk : secp256k1_ec_seckey_negate($sk);
$cancel_tweak = secp256k1_ec_seckey_negate($effective_sk);
var_dump(secp256k1_keypair_xonly_tweak_add($kp2, $cancel_tweak));

// invalid tweak length throws
try {
    secp256k1_keypair_xonly_tweak_add($kp, "short");
} catch (ValueError $e) {
    echo $e->getMessage() . "\n";
}

?>
--EXPECT--
bool(true)
bool(true)
bool(true)
bool(true)
bool(false)
secp256k1_keypair_xonly_tweak_add(): Argument #2 ($tweak32) must be exactly 32 bytes
