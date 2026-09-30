--TEST--
secp256k1_ec_pubkey_negate(), _tweak_add(), _tweak_mul()
--EXTENSIONS--
secp256k1
--FILE--
<?php

$seckey = str_repeat("\x01", 32);
$pubkey = secp256k1_ec_pubkey_create($seckey);
$original = secp256k1_ec_pubkey_serialize($pubkey);

// Negate: negate(negate(pk)) == pk
secp256k1_ec_pubkey_negate($pubkey);
$negated = secp256k1_ec_pubkey_serialize($pubkey);
var_dump($negated !== $original);

secp256k1_ec_pubkey_negate($pubkey);
var_dump(secp256k1_ec_pubkey_serialize($pubkey) === $original);

// Tweak add: pubkey + tweak*G produces a different valid pubkey
$pubkey2 = secp256k1_ec_pubkey_create($seckey);
$tweak = str_repeat("\x02", 32);
$result = secp256k1_ec_pubkey_tweak_add($pubkey2, $tweak);
var_dump($result);
var_dump(secp256k1_ec_pubkey_serialize($pubkey2) !== $original);

// Tweak add failure preserves the original pubkey
$pubkey3 = secp256k1_ec_pubkey_create($seckey);
$before = secp256k1_ec_pubkey_serialize($pubkey3);
$neg_seckey = secp256k1_ec_seckey_negate($seckey);
var_dump(secp256k1_ec_pubkey_tweak_add($pubkey3, $neg_seckey));
var_dump(secp256k1_ec_pubkey_serialize($pubkey3) === $before);

// Tweak mul failure preserves the original pubkey
$pubkey3b = secp256k1_ec_pubkey_create($seckey);
$before3b = secp256k1_ec_pubkey_serialize($pubkey3b);
var_dump(secp256k1_ec_pubkey_tweak_mul($pubkey3b, str_repeat("\x00", 32)));
var_dump(secp256k1_ec_pubkey_serialize($pubkey3b) === $before3b);

// Tweak mul: pubkey * tweak produces a different valid pubkey
$pubkey4 = secp256k1_ec_pubkey_create($seckey);
$result = secp256k1_ec_pubkey_tweak_mul($pubkey4, $tweak);
var_dump($result);
var_dump(secp256k1_ec_pubkey_serialize($pubkey4) !== $original);

// Invalid tweak length
$pubkey6 = secp256k1_ec_pubkey_create($seckey);
try {
    secp256k1_ec_pubkey_tweak_add($pubkey6, "short");
} catch (ValueError $e) {
    echo $e->getMessage() . "\n";
}

try {
    secp256k1_ec_pubkey_tweak_mul($pubkey6, "short");
} catch (ValueError $e) {
    echo $e->getMessage() . "\n";
}

?>
--EXPECT--
bool(true)
bool(true)
bool(true)
bool(true)
bool(false)
bool(true)
bool(false)
bool(true)
bool(true)
bool(true)
secp256k1_ec_pubkey_tweak_add(): Argument #2 ($tweak32) must be exactly 32 bytes
secp256k1_ec_pubkey_tweak_mul(): Argument #2 ($tweak32) must be exactly 32 bytes
