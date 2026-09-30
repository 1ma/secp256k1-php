--TEST--
secp256k1_ecdsa_signature_normalize() normalizes high-S signatures in place
--EXTENSIONS--
secp256k1
--FILE--
<?php

// X coordinate of the secp256k1 generator point G, used as a valid 32-byte scalar for R
$r = hex2bin("79BE667EF9DCBBAC55A06295CE870B07029BFCDB2DCE28D959F2815B16F81798");

// Low-S value (S = 1): already normalized
$low_s = hex2bin("0000000000000000000000000000000000000000000000000000000000000001");
$sig = secp256k1_ecdsa_signature_parse_compact($r . $low_s);

$before = secp256k1_ecdsa_signature_serialize_compact($sig);
$changed = secp256k1_ecdsa_signature_normalize($sig);
$after = secp256k1_ecdsa_signature_serialize_compact($sig);

var_dump($changed);           // false: was already normalized
var_dump($before === $after); // true: unchanged

// High-S value (S = order - 1, which is > order/2)
// order = FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEBAAEDCE6AF48A03BBFD25E8CD0364141
// order - 1 = FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEBAAEDCE6AF48A03BBFD25E8CD0364140
$high_s = hex2bin("FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEBAAEDCE6AF48A03BBFD25E8CD0364140");
$sig2 = secp256k1_ecdsa_signature_parse_compact($r . $high_s);

$before2 = secp256k1_ecdsa_signature_serialize_compact($sig2);
$changed2 = secp256k1_ecdsa_signature_normalize($sig2);
$after2 = secp256k1_ecdsa_signature_serialize_compact($sig2);

var_dump($changed2);            // true: was not normalized, had high-S
var_dump($before2 === $after2); // false: signature was modified

// After normalization, the S should be low (order - high_s = 1)
$normalized_s = substr($after2, 32);
var_dump(bin2hex($normalized_s) === "0000000000000000000000000000000000000000000000000000000000000001");

// Normalizing again should be a no-op
$changed3 = secp256k1_ecdsa_signature_normalize($sig2);
var_dump($changed3); // false: already normalized now

?>
--EXPECT--
bool(false)
bool(true)
bool(true)
bool(false)
bool(true)
bool(false)
