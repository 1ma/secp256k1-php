--TEST--
secp256k1_ec_seckey_verify() validates secret keys
--EXTENSIONS--
secp256k1
--FILE--
<?php

// Valid 32-byte secret key (all 0x01)
var_dump(secp256k1_ec_seckey_verify(str_repeat("\x01", 32)));

// Zero key is invalid (must be 1 <= key < order)
var_dump(secp256k1_ec_seckey_verify(str_repeat("\x00", 32)));

// Key equal to or above the curve order is invalid
// secp256k1 order n = FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEBAAEDCE6AF48A03BBFD25E8CD0364141
var_dump(secp256k1_ec_seckey_verify(hex2bin("FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEBAAEDCE6AF48A03BBFD25E8CD0364141")));

// order - 1 is valid (the largest valid key)
var_dump(secp256k1_ec_seckey_verify(hex2bin("FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEBAAEDCE6AF48A03BBFD25E8CD0364140")));

// All 0xFF is invalid (way above the order)
var_dump(secp256k1_ec_seckey_verify(str_repeat("\xFF", 32)));

?>
--EXPECT--
bool(true)
bool(false)
bool(false)
bool(true)
bool(false)
