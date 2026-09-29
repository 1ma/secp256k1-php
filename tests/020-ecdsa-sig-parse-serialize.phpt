--TEST--
secp256k1_ecdsa_signature parse and serialize roundtrip
--EXTENSIONS--
secp256k1
--FILE--
<?php

// Create a valid compact signature (64 bytes of R||S)
// Use a known valid signature: sign a message first to get one
$seckey = str_repeat("\x01", 32);
$msg32 = hash('sha256', 'test message', true);

// We don't have secp256k1_ecdsa_sign yet, so craft a compact sig manually
// R = valid 32-byte scalar, S = valid 32-byte scalar
$r = hex2bin("79BE667EF9DCBBAC55A06295CE870B07029BFCDB2DCE28D959F2815B16F81798");
$s = hex2bin("0000000000000000000000000000000000000000000000000000000000000001");
$compact = $r . $s;

// Parse compact
$sig = secp256k1_ecdsa_signature_parse_compact($compact);
var_dump($sig instanceof secp256k1_ecdsa_signature);

// Serialize compact roundtrip
$out = secp256k1_ecdsa_signature_serialize_compact($sig);
var_dump($out === $compact);
var_dump(strlen($out) === 64);

// Serialize to DER
$der = secp256k1_ecdsa_signature_serialize_der($sig);
var_dump(strlen($der) > 0 && strlen($der) <= 72);

// Parse DER back and compare
$sig2 = secp256k1_ecdsa_signature_parse_der($der);
var_dump($sig2 instanceof secp256k1_ecdsa_signature);
var_dump(secp256k1_ecdsa_signature_serialize_compact($sig2) === $compact);

// Cross-format: compact -> DER -> compact
$sig3 = secp256k1_ecdsa_signature_parse_der($der);
$compact2 = secp256k1_ecdsa_signature_serialize_compact($sig3);
var_dump($compact2 === $compact);

?>
--EXPECT--
bool(true)
bool(true)
bool(true)
bool(true)
bool(true)
bool(true)
bool(true)
