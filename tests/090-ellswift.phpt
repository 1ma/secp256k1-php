--TEST--
secp256k1 ElligatorSwift
--EXTENSIONS--
secp256k1
--SKIPIF--
<?php
if (!function_exists('secp256k1_ellswift_encode')) die('skip ellswift module not available');
?>
--FILE--
<?php

$sk = str_repeat("\x01", 32);
$pk = secp256k1_ec_pubkey_create($sk);

// encode + decode roundtrip
$rnd = random_bytes(32);
$ell64 = secp256k1_ellswift_encode($pk, $rnd);
var_dump(strlen($ell64) === 64);

$decoded = secp256k1_ellswift_decode($ell64);
var_dump($decoded instanceof secp256k1_pubkey);
var_dump(secp256k1_ec_pubkey_serialize($decoded) === secp256k1_ec_pubkey_serialize($pk));

// different randomness produces different encodings
$ell64_2 = secp256k1_ellswift_encode($pk, random_bytes(32));
var_dump($ell64 !== $ell64_2);

// create from seckey
$ell_created = secp256k1_ellswift_create($sk);
var_dump(strlen($ell_created) === 64);

// decode(create(sk)) matches the pubkey
$decoded2 = secp256k1_ellswift_decode($ell_created);
var_dump(secp256k1_ec_pubkey_serialize($decoded2) === secp256k1_ec_pubkey_serialize($pk));

// create with auxrnd
$ell_aux = secp256k1_ellswift_create($sk, random_bytes(32));
var_dump(strlen($ell_aux) === 64);

// create with invalid seckey returns false
var_dump(secp256k1_ellswift_create(str_repeat("\x00", 32)));

// xdh BIP-324 roundtrip
$alice_sk = str_repeat("\x01", 32);
$bob_sk = str_repeat("\x02", 32);
$alice_ell = secp256k1_ellswift_create($alice_sk);
$bob_ell = secp256k1_ellswift_create($bob_sk);

$secret_a = secp256k1_ellswift_xdh($alice_ell, $bob_ell, $alice_sk, false);
$secret_b = secp256k1_ellswift_xdh($alice_ell, $bob_ell, $bob_sk, true);
var_dump(strlen($secret_a) === 32);
var_dump($secret_a === $secret_b);

// swapped party produces different secret
$secret_wrong = secp256k1_ellswift_xdh($alice_ell, $bob_ell, $alice_sk, true);
var_dump($secret_wrong !== $secret_a);

// xdh with prefix hash function
$prefix = str_repeat("\xab", 64);
$secret_p_a = secp256k1_ellswift_xdh($alice_ell, $bob_ell, $alice_sk, false, SECP256K1_ELLSWIFT_XDH_HASH_PREFIX, $prefix);
$secret_p_b = secp256k1_ellswift_xdh($alice_ell, $bob_ell, $bob_sk, true, SECP256K1_ELLSWIFT_XDH_HASH_PREFIX, $prefix);
var_dump(strlen($secret_p_a) === 32);
var_dump($secret_p_a === $secret_p_b);

// prefix result differs from bip324 result
var_dump($secret_p_a !== $secret_a);

// any 64 bytes decode successfully
$random_ell = random_bytes(64);
$random_pk = secp256k1_ellswift_decode($random_ell);
var_dump($random_pk instanceof secp256k1_pubkey);

// length validation errors
try {
    secp256k1_ellswift_encode($pk, "short");
} catch (ValueError $e) {
    echo $e->getMessage() . "\n";
}

try {
    secp256k1_ellswift_decode("short");
} catch (ValueError $e) {
    echo $e->getMessage() . "\n";
}

try {
    secp256k1_ellswift_create("short");
} catch (ValueError $e) {
    echo $e->getMessage() . "\n";
}

try {
    secp256k1_ellswift_create($sk, "short");
} catch (ValueError $e) {
    echo $e->getMessage() . "\n";
}

try {
    secp256k1_ellswift_xdh("short", $bob_ell, $alice_sk, false);
} catch (ValueError $e) {
    echo $e->getMessage() . "\n";
}

try {
    secp256k1_ellswift_xdh($alice_ell, "short", $alice_sk, false);
} catch (ValueError $e) {
    echo $e->getMessage() . "\n";
}

try {
    secp256k1_ellswift_xdh($alice_ell, $bob_ell, "short", false);
} catch (ValueError $e) {
    echo $e->getMessage() . "\n";
}

// invalid hashfn
try {
    secp256k1_ellswift_xdh($alice_ell, $bob_ell, $alice_sk, false, 99);
} catch (ValueError $e) {
    echo $e->getMessage() . "\n";
}

// prefix hashfn without prefix
try {
    secp256k1_ellswift_xdh($alice_ell, $bob_ell, $alice_sk, false, SECP256K1_ELLSWIFT_XDH_HASH_PREFIX);
} catch (ValueError $e) {
    echo $e->getMessage() . "\n";
}

// prefix hashfn with wrong prefix length
try {
    secp256k1_ellswift_xdh($alice_ell, $bob_ell, $alice_sk, false, SECP256K1_ELLSWIFT_XDH_HASH_PREFIX, "short");
} catch (ValueError $e) {
    echo $e->getMessage() . "\n";
}

// xdh with invalid seckey
var_dump(secp256k1_ellswift_xdh($alice_ell, $bob_ell, str_repeat("\x00", 32), false));

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
bool(true)
bool(true)
bool(true)
bool(true)
bool(true)
bool(true)
bool(true)
secp256k1_ellswift_encode(): Argument #2 ($rnd32) must be exactly 32 bytes
secp256k1_ellswift_decode(): Argument #1 ($ell64) must be exactly 64 bytes
secp256k1_ellswift_create(): Argument #1 ($seckey32) must be exactly 32 bytes
secp256k1_ellswift_create(): Argument #2 ($auxrnd32) must be exactly 32 bytes
secp256k1_ellswift_xdh(): Argument #1 ($ell_a64) must be exactly 64 bytes
secp256k1_ellswift_xdh(): Argument #2 ($ell_b64) must be exactly 64 bytes
secp256k1_ellswift_xdh(): Argument #3 ($seckey32) must be exactly 32 bytes
secp256k1_ellswift_xdh(): Argument #5 ($hashfn) must be SECP256K1_ELLSWIFT_XDH_HASH_BIP324 or SECP256K1_ELLSWIFT_XDH_HASH_PREFIX
secp256k1_ellswift_xdh(): Argument #6 ($prefix64) is required when using SECP256K1_ELLSWIFT_XDH_HASH_PREFIX
secp256k1_ellswift_xdh(): Argument #6 ($prefix64) must be exactly 64 bytes
bool(false)
