--TEST--
secp256k1_schnorrsig sign and verify
--EXTENSIONS--
secp256k1
--SKIPIF--
<?php
if (!function_exists('secp256k1_schnorrsig_sign32')) die('skip schnorrsig module not available');
?>
--FILE--
<?php

$sk = str_repeat("\x01", 32);
$kp = secp256k1_keypair_create($sk);
$xonly = secp256k1_keypair_xonly_pub($kp, $parity);
$msg = hash("sha256", "test message", true);

// sign32 + verify roundtrip
$sig = secp256k1_schnorrsig_sign32($msg, $kp);
var_dump(is_string($sig) && strlen($sig) === 64);
var_dump(secp256k1_schnorrsig_verify($sig, $msg, $xonly));

// verify fails with wrong message
var_dump(secp256k1_schnorrsig_verify($sig, hash("sha256", "wrong", true), $xonly));

// verify fails with wrong key
$sk2 = str_repeat("\x02", 32);
$kp2 = secp256k1_keypair_create($sk2);
$xonly2 = secp256k1_keypair_xonly_pub($kp2, $p2);
var_dump(secp256k1_schnorrsig_verify($sig, $msg, $xonly2));

// verify fails with modified signature
$bad_sig = $sig;
$bad_sig[0] = chr(ord($bad_sig[0]) ^ 0x01);
var_dump(secp256k1_schnorrsig_verify($bad_sig, $msg, $xonly));

// aux_rand32 = null works (deterministic)
$sig_no_aux = secp256k1_schnorrsig_sign32($msg, $kp, null);
var_dump(secp256k1_schnorrsig_verify($sig_no_aux, $msg, $xonly));

// aux_rand32 with 32 bytes works and produces different signature
$sig_with_aux = secp256k1_schnorrsig_sign32($msg, $kp, str_repeat("\xff", 32));
var_dump(secp256k1_schnorrsig_verify($sig_with_aux, $msg, $xonly));
var_dump($sig_no_aux !== $sig_with_aux);

// sign_custom with 32-byte message
$sig_custom = secp256k1_schnorrsig_sign_custom($msg, $kp);
var_dump(secp256k1_schnorrsig_verify($sig_custom, $msg, $xonly));

// sign_custom with non-32-byte message
$long_msg = "this is a longer message for schnorr";
$sig_long = secp256k1_schnorrsig_sign_custom($long_msg, $kp);
var_dump(secp256k1_schnorrsig_verify($sig_long, $long_msg, $xonly));

// sign_custom with empty message
$sig_empty = secp256k1_schnorrsig_sign_custom("", $kp);
var_dump(secp256k1_schnorrsig_verify($sig_empty, "", $xonly));

// invalid msg length for sign32
try {
    secp256k1_schnorrsig_sign32("short", $kp);
} catch (ValueError $e) {
    echo $e->getMessage() . "\n";
}

// invalid aux_rand32 length
try {
    secp256k1_schnorrsig_sign32($msg, $kp, "short");
} catch (ValueError $e) {
    echo $e->getMessage() . "\n";
}

// invalid sig length for verify
try {
    secp256k1_schnorrsig_verify("short", $msg, $xonly);
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
bool(true)
bool(true)
bool(true)
bool(true)
bool(true)
bool(true)
secp256k1_schnorrsig_sign32(): Argument #1 ($msghash32) must be exactly 32 bytes
secp256k1_schnorrsig_sign32(): Argument #3 ($aux_rand32) must be exactly 32 bytes
secp256k1_schnorrsig_verify(): Argument #1 ($sig64) must be exactly 64 bytes
