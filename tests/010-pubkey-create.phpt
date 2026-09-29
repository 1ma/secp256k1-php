--TEST--
secp256k1_ec_pubkey_create() generates a public key
--EXTENSIONS--
secp256k1
--FILE--
<?php

$pubkey = secp256k1_ec_pubkey_create(str_repeat("\x01", 32));
var_dump($pubkey instanceof secp256k1_pubkey);

// Invalid secret key (all zeros)
var_dump(secp256k1_ec_pubkey_create(str_repeat("\x00", 32)));

// Wrong length
try {
    secp256k1_ec_pubkey_create("short");
} catch (ValueError $e) {
    echo $e->getMessage() . "\n";
}

?>
--EXPECT--
bool(true)
bool(false)
secp256k1_ec_pubkey_create(): Argument #1 ($seckey32) must be exactly 32 bytes
