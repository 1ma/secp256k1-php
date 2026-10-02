--TEST--
secp256k1_keypair create, pub, xonly_pub, sec
--EXTENSIONS--
secp256k1
--SKIPIF--
<?php
if (!function_exists('secp256k1_keypair_create')) die('skip extrakeys module not available');
?>
--FILE--
<?php

$sk = str_repeat("\x01", 32);

// create keypair from secret key
$kp = secp256k1_keypair_create($sk);
var_dump($kp instanceof secp256k1_keypair);

// extract public key — must match ec_pubkey_create
$pk_from_kp = secp256k1_keypair_pub($kp);
var_dump($pk_from_kp instanceof secp256k1_pubkey);
$pk_direct = secp256k1_ec_pubkey_create($sk);
var_dump(secp256k1_ec_pubkey_serialize($pk_from_kp) === secp256k1_ec_pubkey_serialize($pk_direct));

// extract xonly pubkey + parity — must match from_pubkey
$xonly_from_kp = secp256k1_keypair_xonly_pub($kp, $parity_kp);
var_dump($xonly_from_kp instanceof secp256k1_xonly_pubkey);
$xonly_from_pk = secp256k1_xonly_pubkey_from_pubkey($pk_direct, $parity_pk);
var_dump(secp256k1_xonly_pubkey_serialize($xonly_from_kp) === secp256k1_xonly_pubkey_serialize($xonly_from_pk));
var_dump($parity_kp === $parity_pk);

// extract secret key — must match original
$sk_extracted = secp256k1_keypair_sec($kp);
var_dump($sk_extracted === $sk);

// invalid seckey (all zeros) returns false
var_dump(secp256k1_keypair_create(str_repeat("\x00", 32)));

// invalid seckey length throws
try {
    secp256k1_keypair_create("short");
} catch (ValueError $e) {
    echo $e->getMessage() . "\n";
}

// cannot instantiate directly
try {
    new secp256k1_keypair();
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
bool(true)
bool(true)
bool(false)
secp256k1_keypair_create(): Argument #1 ($seckey32) must be exactly 32 bytes
Cannot instantiate secp256k1_keypair directly, use secp256k1_keypair_create()
