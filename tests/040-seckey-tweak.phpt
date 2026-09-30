--TEST--
secp256k1_ec_seckey_negate(), _tweak_add(), _tweak_mul()
--EXTENSIONS--
secp256k1
--FILE--
<?php

$seckey = str_repeat("\x01", 32);

// Negate: negate(negate(x)) == x
$negated = secp256k1_ec_seckey_negate($seckey);
var_dump($negated !== $seckey);
$double_negated = secp256k1_ec_seckey_negate($negated);
var_dump($double_negated === $seckey);

// Tweak add: seckey + tweak produces a valid new key
$tweak = str_repeat("\x02", 32);
$tweaked = secp256k1_ec_seckey_tweak_add($seckey, $tweak);
var_dump($tweaked !== false);
var_dump($tweaked !== $seckey);
var_dump(secp256k1_ec_seckey_verify($tweaked));

// Negate with invalid seckey (all zeros) returns false
var_dump(secp256k1_ec_seckey_negate(str_repeat("\x00", 32)));

// Tweak add: adding the negation of the key to itself should fail (result = 0)
$neg = secp256k1_ec_seckey_negate($seckey);
var_dump(secp256k1_ec_seckey_tweak_add($seckey, $neg));

// Tweak mul: seckey * tweak produces a valid new key
$mul_tweaked = secp256k1_ec_seckey_tweak_mul($seckey, $tweak);
var_dump($mul_tweaked !== false);
var_dump(secp256k1_ec_seckey_verify($mul_tweaked));

// Tweak mul with zero tweak fails
var_dump(secp256k1_ec_seckey_tweak_mul($seckey, str_repeat("\x00", 32)));

// Invalid seckey length
try {
    secp256k1_ec_seckey_negate("short");
} catch (ValueError $e) {
    echo $e->getMessage() . "\n";
}

// Invalid seckey length
try {
    secp256k1_ec_seckey_tweak_add("short", $tweak);
} catch (ValueError $e) {
    echo $e->getMessage() . "\n";
}

try {
    secp256k1_ec_seckey_tweak_mul("short", $tweak);
} catch (ValueError $e) {
    echo $e->getMessage() . "\n";
}

// Invalid tweak length
try {
    secp256k1_ec_seckey_tweak_add($seckey, "short");
} catch (ValueError $e) {
    echo $e->getMessage() . "\n";
}

try {
    secp256k1_ec_seckey_tweak_mul($seckey, "short");
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
bool(false)
bool(false)
bool(true)
bool(true)
bool(false)
secp256k1_ec_seckey_negate(): Argument #1 ($seckey32) must be exactly 32 bytes
secp256k1_ec_seckey_tweak_add(): Argument #1 ($seckey32) must be exactly 32 bytes
secp256k1_ec_seckey_tweak_mul(): Argument #1 ($seckey32) must be exactly 32 bytes
secp256k1_ec_seckey_tweak_add(): Argument #2 ($tweak32) must be exactly 32 bytes
secp256k1_ec_seckey_tweak_mul(): Argument #2 ($tweak32) must be exactly 32 bytes
