--TEST--
secp256k1_ec_seckey_verify() rejects wrong-length input
--EXTENSIONS--
secp256k1
--FILE--
<?php

try {
    secp256k1_ec_seckey_verify("too short");
} catch (ValueError $e) {
    echo $e->getMessage() . "\n";
}

try {
    secp256k1_ec_seckey_verify(str_repeat("\x01", 33));
} catch (ValueError $e) {
    echo $e->getMessage() . "\n";
}

try {
    secp256k1_ec_seckey_verify("");
} catch (ValueError $e) {
    echo $e->getMessage() . "\n";
}

?>
--EXPECT--
secp256k1_ec_seckey_verify(): Argument #1 ($seckey32) must be exactly 32 bytes
secp256k1_ec_seckey_verify(): Argument #1 ($seckey32) must be exactly 32 bytes
secp256k1_ec_seckey_verify(): Argument #1 ($seckey32) must be exactly 32 bytes
