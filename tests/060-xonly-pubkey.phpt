--TEST--
secp256k1_xonly_pubkey parse, serialize, cmp, from_pubkey
--EXTENSIONS--
secp256k1
--SKIPIF--
<?php
if (!function_exists('secp256k1_xonly_pubkey_parse')) die('skip extrakeys module not available');
?>
--FILE--
<?php

$sk = str_repeat("\x01", 32);
$pk = secp256k1_ec_pubkey_create($sk);

// from_pubkey returns xonly_pubkey and sets parity
$xonly = secp256k1_xonly_pubkey_from_pubkey($pk, $parity);
var_dump($xonly instanceof secp256k1_xonly_pubkey);
var_dump($parity === 0 || $parity === 1);

// serialize is always 32 bytes
$ser = secp256k1_xonly_pubkey_serialize($xonly);
var_dump(strlen($ser));

// parse roundtrip
$parsed = secp256k1_xonly_pubkey_parse($ser);
var_dump($parsed instanceof secp256k1_xonly_pubkey);
var_dump(secp256k1_xonly_pubkey_serialize($parsed) === $ser);

// cmp: equal keys
var_dump(secp256k1_xonly_pubkey_cmp($xonly, $parsed));

// cmp: different keys
$sk2 = str_repeat("\x02", 32);
$pk2 = secp256k1_ec_pubkey_create($sk2);
$xonly2 = secp256k1_xonly_pubkey_from_pubkey($pk2, $parity2);
$cmp = secp256k1_xonly_pubkey_cmp($xonly, $xonly2);
var_dump($cmp === 1 || $cmp === -1);

// parse invalid input returns false
var_dump(secp256k1_xonly_pubkey_parse(str_repeat("\x00", 32)));

// parse wrong length throws
try {
    secp256k1_xonly_pubkey_parse("short");
} catch (ValueError $e) {
    echo $e->getMessage() . "\n";
}

// cannot instantiate directly
try {
    new secp256k1_xonly_pubkey();
} catch (Error $e) {
    echo $e->getMessage() . "\n";
}

?>
--EXPECT--
bool(true)
bool(true)
int(32)
bool(true)
bool(true)
int(0)
bool(true)
bool(false)
secp256k1_xonly_pubkey_parse(): Argument #1 ($input32) must be exactly 32 bytes
Cannot instantiate secp256k1_xonly_pubkey directly, use secp256k1_xonly_pubkey_parse() or secp256k1_xonly_pubkey_from_pubkey()
