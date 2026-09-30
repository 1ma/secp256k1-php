--TEST--
secp256k1_ec_pubkey_combine() and secp256k1_ec_pubkey_cmp()
--EXTENSIONS--
secp256k1
--FILE--
<?php

$pk1 = secp256k1_ec_pubkey_create(str_repeat("\x01", 32));
$pk2 = secp256k1_ec_pubkey_create(str_repeat("\x02", 32));

// cmp: same key
var_dump(secp256k1_ec_pubkey_cmp($pk1, $pk1));

// cmp: different keys, consistent ordering
$cmp12 = secp256k1_ec_pubkey_cmp($pk1, $pk2);
$cmp21 = secp256k1_ec_pubkey_cmp($pk2, $pk1);
var_dump($cmp12 !== 0);
var_dump($cmp12 === -$cmp21);

// combine: two keys
$combined = secp256k1_ec_pubkey_combine([$pk1, $pk2]);
var_dump($combined instanceof secp256k1_pubkey);
var_dump(secp256k1_ec_pubkey_cmp($combined, $pk1) !== 0);

// combine: single key returns a copy
$single = secp256k1_ec_pubkey_combine([$pk1]);
var_dump(secp256k1_ec_pubkey_cmp($single, $pk1));

// combine: pk + (-pk) = point at infinity, returns false
$pk_neg = secp256k1_ec_pubkey_create(str_repeat("\x01", 32));
secp256k1_ec_pubkey_negate($pk_neg);
var_dump(secp256k1_ec_pubkey_combine([$pk1, $pk_neg]));

// combine: empty array throws
try {
    secp256k1_ec_pubkey_combine([]);
} catch (ValueError $e) {
    echo $e->getMessage() . "\n";
}

// combine: wrong type in array throws
try {
    secp256k1_ec_pubkey_combine([$pk1, "not a pubkey"]);
} catch (TypeError $e) {
    echo $e->getMessage() . "\n";
}

?>
--EXPECT--
int(0)
bool(true)
bool(true)
bool(true)
bool(true)
int(0)
bool(false)
secp256k1_ec_pubkey_combine(): Argument #1 ($pubkeys) must not be empty
secp256k1_ec_pubkey_combine(): Argument #1 ($pubkeys) must contain only secp256k1_pubkey objects
