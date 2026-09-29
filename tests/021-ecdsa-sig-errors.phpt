--TEST--
secp256k1_ecdsa_signature parse errors and opaque class restrictions
--EXTENSIONS--
secp256k1
--FILE--
<?php

// Wrong length for compact
try {
    secp256k1_ecdsa_signature_parse_compact(str_repeat("\x01", 63));
} catch (ValueError $e) {
    echo $e->getMessage() . "\n";
}

// Invalid compact (R and S above curve order)
var_dump(secp256k1_ecdsa_signature_parse_compact(str_repeat("\xFF", 64)));

// Invalid DER
var_dump(secp256k1_ecdsa_signature_parse_der("not a der signature"));

// Cannot instantiate directly
try {
    new secp256k1_ecdsa_signature();
} catch (Error $e) {
    echo $e->getMessage() . "\n";
}

// Cannot serialize
$r = hex2bin("79BE667EF9DCBBAC55A06295CE870B07029BFCDB2DCE28D959F2815B16F81798");
$s = hex2bin("0000000000000000000000000000000000000000000000000000000000000001");
$sig = secp256k1_ecdsa_signature_parse_compact($r . $s);
try {
    serialize($sig);
} catch (Exception $e) {
    echo $e->getMessage() . "\n";
}

// Cannot clone
try {
    clone $sig;
} catch (Error $e) {
    echo $e->getMessage() . "\n";
}

?>
--EXPECT--
secp256k1_ecdsa_signature_parse_compact(): Argument #1 ($sig64) must be exactly 64 bytes
bool(false)
bool(false)
Cannot instantiate secp256k1_ecdsa_signature directly, use secp256k1_ecdsa_signature_parse_compact() or secp256k1_ecdsa_signature_parse_der()
Serialization of 'secp256k1_ecdsa_signature' is not allowed
Trying to clone an uncloneable object of class secp256k1_ecdsa_signature
