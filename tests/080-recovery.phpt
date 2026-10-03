--TEST--
secp256k1 ECDSA recovery
--EXTENSIONS--
secp256k1
--SKIPIF--
<?php
if (!function_exists('secp256k1_ecdsa_sign_recoverable')) die('skip recovery module not available');
?>
--FILE--
<?php

$sk = str_repeat("\x01", 32);
$pk = secp256k1_ec_pubkey_create($sk);
$msg = hash("sha256", "test message", true);

// sign_recoverable + recover roundtrip
$rsig = secp256k1_ecdsa_sign_recoverable($msg, $sk);
var_dump($rsig instanceof secp256k1_ecdsa_recoverable_signature);

$recovered = secp256k1_ecdsa_recover($rsig, $msg);
var_dump($recovered instanceof secp256k1_pubkey);
var_dump(secp256k1_ec_pubkey_serialize($recovered) === secp256k1_ec_pubkey_serialize($pk));

// serialize_compact + parse_compact roundtrip
$compact = secp256k1_ecdsa_recoverable_signature_serialize_compact($rsig, $recid);
var_dump(strlen($compact) === 64);
var_dump($recid >= 0 && $recid <= 3);

$parsed = secp256k1_ecdsa_recoverable_signature_parse_compact($compact, $recid);
var_dump($parsed instanceof secp256k1_ecdsa_recoverable_signature);

// recovered key from reparsed sig matches
$recovered2 = secp256k1_ecdsa_recover($parsed, $msg);
var_dump(secp256k1_ec_pubkey_serialize($recovered2) === secp256k1_ec_pubkey_serialize($pk));

// convert to normal signature + verify (already low-S from sign_recoverable)
$normal = secp256k1_ecdsa_recoverable_signature_convert($rsig);
var_dump($normal instanceof secp256k1_ecdsa_signature);
var_dump(secp256k1_ecdsa_verify($normal, $msg, $pk));

// recover with wrong message fails
$wrong_msg = hash("sha256", "wrong", true);
$bad_recover = secp256k1_ecdsa_recover($rsig, $wrong_msg);
var_dump($bad_recover instanceof secp256k1_pubkey);
var_dump(secp256k1_ec_pubkey_serialize($bad_recover) !== secp256k1_ec_pubkey_serialize($pk));

// sign_recoverable with invalid key returns false
var_dump(secp256k1_ecdsa_sign_recoverable($msg, str_repeat("\x00", 32)));

// direct instantiation throws
try {
    new secp256k1_ecdsa_recoverable_signature();
} catch (\Error $e) {
    echo $e->getMessage() . "\n";
}

// parse_compact with invalid recid
try {
    secp256k1_ecdsa_recoverable_signature_parse_compact($compact, -1);
} catch (ValueError $e) {
    echo $e->getMessage() . "\n";
}

try {
    secp256k1_ecdsa_recoverable_signature_parse_compact($compact, 4);
} catch (ValueError $e) {
    echo $e->getMessage() . "\n";
}

// parse_compact with invalid signature data returns false
var_dump(secp256k1_ecdsa_recoverable_signature_parse_compact(str_repeat("\xff", 64), 0));

// sign_recoverable with invalid seckey length
try {
    secp256k1_ecdsa_sign_recoverable($msg, "short");
} catch (ValueError $e) {
    echo $e->getMessage() . "\n";
}

// recover with fabricated invalid signature returns false
$fake_rsig = secp256k1_ecdsa_recoverable_signature_parse_compact(str_repeat("\x01", 64), 2);
if ($fake_rsig !== false) {
    var_dump(secp256k1_ecdsa_recover($fake_rsig, $msg));
}

// parse_compact with invalid sig length
try {
    secp256k1_ecdsa_recoverable_signature_parse_compact("short", 0);
} catch (ValueError $e) {
    echo $e->getMessage() . "\n";
}

// sign_recoverable with invalid msg length
try {
    secp256k1_ecdsa_sign_recoverable("short", $sk);
} catch (ValueError $e) {
    echo $e->getMessage() . "\n";
}

// recover with invalid msg length
try {
    secp256k1_ecdsa_recover($rsig, "short");
} catch (ValueError $e) {
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
bool(true)
bool(true)
bool(true)
bool(true)
bool(false)
Cannot instantiate secp256k1_ecdsa_recoverable_signature directly, use secp256k1_ecdsa_recoverable_signature_parse_compact() or secp256k1_ecdsa_sign_recoverable()
secp256k1_ecdsa_recoverable_signature_parse_compact(): Argument #2 ($recid) must be between 0 and 3
secp256k1_ecdsa_recoverable_signature_parse_compact(): Argument #2 ($recid) must be between 0 and 3
bool(false)
secp256k1_ecdsa_sign_recoverable(): Argument #2 ($seckey32) must be exactly 32 bytes
bool(false)
secp256k1_ecdsa_recoverable_signature_parse_compact(): Argument #1 ($sig64) must be exactly 64 bytes
secp256k1_ecdsa_sign_recoverable(): Argument #1 ($msghash32) must be exactly 32 bytes
secp256k1_ecdsa_recover(): Argument #2 ($msghash32) must be exactly 32 bytes
