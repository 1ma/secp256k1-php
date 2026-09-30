--TEST--
secp256k1_ecdsa_sign() and secp256k1_ecdsa_verify() roundtrip
--EXTENSIONS--
secp256k1
--FILE--
<?php

$seckey = str_repeat("\x01", 32);
$pubkey = secp256k1_ec_pubkey_create($seckey);
$msghash = hash('sha256', 'test message', true);

// Sign
$sig = secp256k1_ecdsa_sign($msghash, $seckey);
var_dump($sig instanceof secp256k1_ecdsa_signature);

// Verify with correct pubkey
var_dump(secp256k1_ecdsa_verify($sig, $msghash, $pubkey));

// Verify with wrong message
$wrong_msg = hash('sha256', 'wrong message', true);
var_dump(secp256k1_ecdsa_verify($sig, $wrong_msg, $pubkey));

// Verify with wrong pubkey
$other_seckey = str_repeat("\x02", 32);
$other_pubkey = secp256k1_ec_pubkey_create($other_seckey);
var_dump(secp256k1_ecdsa_verify($sig, $msghash, $other_pubkey));

// Signature is already normalized (low-S) when produced by secp256k1_ecdsa_sign
$changed = secp256k1_ecdsa_signature_normalize($sig);
var_dump($changed);

// Sign with invalid seckey returns false
var_dump(secp256k1_ecdsa_sign($msghash, str_repeat("\x00", 32)));

// Wrong msghash length
try {
    secp256k1_ecdsa_sign("short", $seckey);
} catch (ValueError $e) {
    echo $e->getMessage() . "\n";
}

// Wrong seckey length
try {
    secp256k1_ecdsa_sign($msghash, "short");
} catch (ValueError $e) {
    echo $e->getMessage() . "\n";
}

// Wrong msghash length on verify
try {
    secp256k1_ecdsa_verify($sig, "short", $pubkey);
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
bool(false)
secp256k1_ecdsa_sign(): Argument #1 ($msghash32) must be exactly 32 bytes
secp256k1_ecdsa_sign(): Argument #2 ($seckey32) must be exactly 32 bytes
secp256k1_ecdsa_verify(): Argument #2 ($msghash32) must be exactly 32 bytes
