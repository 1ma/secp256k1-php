--TEST--
secp256k1_pubkey cannot be instantiated, serialized or cloned from userland
--EXTENSIONS--
secp256k1
--FILE--
<?php

// Cannot instantiate directly
try {
    new secp256k1_pubkey();
} catch (Error $e) {
    echo $e->getMessage() . "\n";
}

// Cannot serialize
$pubkey = secp256k1_ec_pubkey_create(str_repeat("\x01", 32));
try {
    serialize($pubkey);
} catch (Exception $e) {
    echo $e->getMessage() . "\n";
}

// Cannot clone
try {
    clone $pubkey;
} catch (Error $e) {
    echo $e->getMessage() . "\n";
}

?>
--EXPECT--
Cannot instantiate secp256k1_pubkey directly, use secp256k1_ec_pubkey_create() or secp256k1_ec_pubkey_parse()
Serialization of 'secp256k1_pubkey' is not allowed
Trying to clone an uncloneable object of class secp256k1_pubkey
