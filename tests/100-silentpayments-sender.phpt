--TEST--
secp256k1_silentpayments_sender_create_outputs() creates outputs for recipients
--EXTENSIONS--
secp256k1
--SKIPIF--
<?php
if (!function_exists('secp256k1_silentpayments_sender_create_outputs')) die('skip silentpayments not available');
?>
--FILE--
<?php
$seckey = str_repeat("\x01", 32);
$keypair = secp256k1_keypair_create($seckey);
$outpoint = str_repeat("\x00", 36);

// recipient keys
$recipient_scan_seckey = str_repeat("\x02", 32);
$recipient_spend_seckey = str_repeat("\x03", 32);
$scan_pubkey = secp256k1_ec_pubkey_create($recipient_scan_seckey);
$spend_pubkey = secp256k1_ec_pubkey_create($recipient_spend_seckey);

// single recipient with taproot input
$outputs = secp256k1_silentpayments_sender_create_outputs(
    $outpoint,
    [$scan_pubkey],
    [$spend_pubkey],
    [$keypair],
    []
);
var_dump(is_array($outputs));
var_dump(count($outputs));
var_dump($outputs[0] instanceof secp256k1_xonly_pubkey);

// single recipient with non-taproot input
$outputs2 = secp256k1_silentpayments_sender_create_outputs(
    $outpoint,
    [$scan_pubkey],
    [$spend_pubkey],
    [],
    [$seckey]
);
var_dump(is_array($outputs2));
var_dump($outputs2[0] instanceof secp256k1_xonly_pubkey);

// mixed inputs: both keypairs and seckeys
$seckey2 = str_repeat("\x05", 32);
$outputs_mixed = secp256k1_silentpayments_sender_create_outputs(
    $outpoint,
    [$scan_pubkey],
    [$spend_pubkey],
    [$keypair],
    [$seckey2]
);
var_dump($outputs_mixed[0] instanceof secp256k1_xonly_pubkey);

// two recipients with same scan key, different spend keys
$recipient_spend_seckey2 = str_repeat("\x04", 32);
$spend_pubkey2 = secp256k1_ec_pubkey_create($recipient_spend_seckey2);
$outputs3 = secp256k1_silentpayments_sender_create_outputs(
    $outpoint,
    [$scan_pubkey, $scan_pubkey],
    [$spend_pubkey, $spend_pubkey2],
    [$keypair],
    []
);
var_dump(count($outputs3));
var_dump($outputs3[0] instanceof secp256k1_xonly_pubkey);
var_dump($outputs3[1] instanceof secp256k1_xonly_pubkey);
var_dump(secp256k1_xonly_pubkey_serialize($outputs3[0]) !== secp256k1_xonly_pubkey_serialize($outputs3[1]));

// two recipients with different scan keys
$scan_seckey2 = str_repeat("\x06", 32);
$scan_pubkey2 = secp256k1_ec_pubkey_create($scan_seckey2);
$outputs4 = secp256k1_silentpayments_sender_create_outputs(
    $outpoint,
    [$scan_pubkey, $scan_pubkey2],
    [$spend_pubkey, $spend_pubkey2],
    [$keypair],
    []
);
var_dump(count($outputs4));
var_dump($outputs4[0] instanceof secp256k1_xonly_pubkey);
var_dump($outputs4[1] instanceof secp256k1_xonly_pubkey);
var_dump(secp256k1_xonly_pubkey_serialize($outputs4[0]) !== secp256k1_xonly_pubkey_serialize($outputs4[1]));

// error: wrong outpoint length
try {
    secp256k1_silentpayments_sender_create_outputs("short", [$scan_pubkey], [$spend_pubkey], [$keypair], []);
} catch (ValueError $e) {
    echo $e->getMessage() . "\n";
}

// error: empty recipients
try {
    secp256k1_silentpayments_sender_create_outputs($outpoint, [], [], [$keypair], []);
} catch (ValueError $e) {
    echo $e->getMessage() . "\n";
}

// error: mismatched recipient array lengths
try {
    secp256k1_silentpayments_sender_create_outputs($outpoint, [$scan_pubkey, $scan_pubkey], [$spend_pubkey], [$keypair], []);
} catch (ValueError $e) {
    echo $e->getMessage() . "\n";
}

// error: no keypairs and no seckeys
try {
    secp256k1_silentpayments_sender_create_outputs($outpoint, [$scan_pubkey], [$spend_pubkey], [], []);
} catch (ValueError $e) {
    echo $e->getMessage() . "\n";
}

// error: wrong type in scan pubkeys array
try {
    secp256k1_silentpayments_sender_create_outputs($outpoint, ["not a pubkey"], [$spend_pubkey], [$keypair], []);
} catch (TypeError $e) {
    echo $e->getMessage() . "\n";
}

// error: wrong type in spend pubkeys array
try {
    secp256k1_silentpayments_sender_create_outputs($outpoint, [$scan_pubkey], [42], [$keypair], []);
} catch (TypeError $e) {
    echo $e->getMessage() . "\n";
}

// error: wrong type in keypairs array
try {
    secp256k1_silentpayments_sender_create_outputs($outpoint, [$scan_pubkey], [$spend_pubkey], [$scan_pubkey], []);
} catch (TypeError $e) {
    echo $e->getMessage() . "\n";
}

// error: wrong value in seckeys array
try {
    secp256k1_silentpayments_sender_create_outputs($outpoint, [$scan_pubkey], [$spend_pubkey], [], ["short"]);
} catch (ValueError $e) {
    echo $e->getMessage() . "\n";
}

// error: non-string in seckeys array
try {
    secp256k1_silentpayments_sender_create_outputs($outpoint, [$scan_pubkey], [$spend_pubkey], [], [42]);
} catch (ValueError $e) {
    echo $e->getMessage() . "\n";
}

// error: wrong argument count
try {
    secp256k1_silentpayments_sender_create_outputs($outpoint, [$scan_pubkey], [$spend_pubkey], [$keypair]);
} catch (ArgumentCountError $e) {
    echo $e->getMessage() . "\n";
}
?>
--EXPECT--
bool(true)
int(1)
bool(true)
bool(true)
bool(true)
bool(true)
int(2)
bool(true)
bool(true)
bool(true)
int(2)
bool(true)
bool(true)
bool(true)
secp256k1_silentpayments_sender_create_outputs(): Argument #1 ($outpoint_smallest36) must be exactly 36 bytes
secp256k1_silentpayments_sender_create_outputs(): Argument #2 ($recipient_scan_pubkeys) must not be empty
secp256k1_silentpayments_sender_create_outputs(): Argument #3 ($recipient_spend_pubkeys) must have the same length as argument #2
secp256k1_silentpayments_sender_create_outputs(): Argument #4 ($keypairs) at least one of keypairs or seckeys must be non-empty
secp256k1_silentpayments_sender_create_outputs(): Argument #2 ($recipient_scan_pubkeys) must contain only secp256k1_pubkey objects
secp256k1_silentpayments_sender_create_outputs(): Argument #3 ($recipient_spend_pubkeys) must contain only secp256k1_pubkey objects
secp256k1_silentpayments_sender_create_outputs(): Argument #4 ($keypairs) must contain only secp256k1_keypair objects
secp256k1_silentpayments_sender_create_outputs(): Argument #5 ($seckeys) must contain only 32-byte strings
secp256k1_silentpayments_sender_create_outputs(): Argument #5 ($seckeys) must contain only 32-byte strings
secp256k1_silentpayments_sender_create_outputs() expects exactly 5 arguments, 4 given
