--TEST--
secp256k1_context_randomize() re-randomizes the global context
--EXTENSIONS--
secp256k1
--FILE--
<?php

var_dump(secp256k1_context_randomize());

// signing still works after re-randomization
$seckey = str_repeat("\x01", 32);
$msg = hash('sha256', 'test', true);
$sig = secp256k1_ecdsa_sign($msg, $seckey);
var_dump($sig instanceof secp256k1_ecdsa_signature);

// re-randomize again, verify still works
var_dump(secp256k1_context_randomize());
$pubkey = secp256k1_ec_pubkey_create($seckey);
var_dump(secp256k1_ecdsa_verify($sig, $msg, $pubkey));

// no arguments accepted
try {
    secp256k1_context_randomize("extra");
} catch (ArgumentCountError $e) {
    echo $e->getMessage() . "\n";
}
?>
--EXPECT--
bool(true)
bool(true)
bool(true)
bool(true)
secp256k1_context_randomize() expects exactly 0 arguments, 1 given
