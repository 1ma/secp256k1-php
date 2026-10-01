--TEST--
secp256k1_ecdh() computes a shared secret
--EXTENSIONS--
secp256k1
--SKIPIF--
<?php
if (!function_exists('secp256k1_ecdh')) die('skip ECDH module not available');
?>
--FILE--
<?php

$sk1 = str_repeat("\x01", 32);
$sk2 = str_repeat("\x02", 32);
$pk1 = secp256k1_ec_pubkey_create($sk1);
$pk2 = secp256k1_ec_pubkey_create($sk2);

// Shared secret: sk1 * pk2 == sk2 * pk1
$secret1 = secp256k1_ecdh($pk2, $sk1);
$secret2 = secp256k1_ecdh($pk1, $sk2);
var_dump(strlen($secret1));
var_dump($secret1 === $secret2);

// Different keys produce different secrets
$sk3 = str_repeat("\x03", 32);
$secret3 = secp256k1_ecdh($pk2, $sk3);
var_dump($secret3 !== $secret1);

// Invalid seckey (all zeros) returns false
var_dump(secp256k1_ecdh($pk1, str_repeat("\x00", 32)));

// Invalid seckey length
try {
    secp256k1_ecdh($pk1, "short");
} catch (ValueError $e) {
    echo $e->getMessage() . "\n";
}

?>
--EXPECT--
int(32)
bool(true)
bool(true)
bool(false)
secp256k1_ecdh(): Argument #2 ($seckey32) must be exactly 32 bytes
