--TEST--
secp256k1_ec_pubkey_parse() and secp256k1_ec_pubkey_serialize() roundtrip
--EXTENSIONS--
secp256k1
--FILE--
<?php

$pubkey = secp256k1_ec_pubkey_create(str_repeat("\x01", 32));

$compressed = secp256k1_ec_pubkey_serialize($pubkey);
var_dump(strlen($compressed) === 33);

$uncompressed = secp256k1_ec_pubkey_serialize($pubkey, SECP256K1_EC_UNCOMPRESSED);
var_dump(strlen($uncompressed) === 65);

// Parse compressed back and re-serialize
$parsed = secp256k1_ec_pubkey_parse($compressed);
var_dump($parsed instanceof secp256k1_pubkey);
var_dump(secp256k1_ec_pubkey_serialize($parsed) === $compressed);

// Parse uncompressed back and re-serialize
$parsed2 = secp256k1_ec_pubkey_parse($uncompressed);
var_dump(secp256k1_ec_pubkey_serialize($parsed2) === $compressed);

// Invalid pubkey
var_dump(secp256k1_ec_pubkey_parse("invalid"));

// Invalid flags
try {
    secp256k1_ec_pubkey_serialize($pubkey, 999);
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
secp256k1_ec_pubkey_serialize(): Argument #2 ($flags) must be SECP256K1_EC_COMPRESSED or SECP256K1_EC_UNCOMPRESSED
