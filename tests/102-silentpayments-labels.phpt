--TEST--
secp256k1_silentpayments_recipient label create, serialize, parse, and labeled spend pubkey
--EXTENSIONS--
secp256k1
--SKIPIF--
<?php
if (!function_exists('secp256k1_silentpayments_recipient_label_create')) die('skip silentpayments not available');
?>
--FILE--
<?php
$scan_key = str_repeat("\x02", 32);
$spend_seckey = str_repeat("\x03", 32);
$spend_pubkey = secp256k1_ec_pubkey_create($spend_seckey);

// create label m=0 (change label)
$label0 = secp256k1_silentpayments_recipient_label_create($scan_key, 0, $tweak0);
var_dump($label0 instanceof secp256k1_silentpayments_label);
var_dump(strlen($tweak0) === 32);

// create label m=1
$label1 = secp256k1_silentpayments_recipient_label_create($scan_key, 1, $tweak1);
var_dump($label1 instanceof secp256k1_silentpayments_label);
var_dump($tweak0 !== $tweak1);

// serialize and parse roundtrip
$serialized = secp256k1_silentpayments_recipient_label_serialize($label0);
var_dump(strlen($serialized) === 33);
$parsed = secp256k1_silentpayments_recipient_label_parse($serialized);
var_dump($parsed instanceof secp256k1_silentpayments_label);
$reserialized = secp256k1_silentpayments_recipient_label_serialize($parsed);
var_dump($serialized === $reserialized);

// labeled spend pubkey differs from unlabeled
$labeled_spend = secp256k1_silentpayments_recipient_create_labeled_spend_pubkey($spend_pubkey, $label0);
var_dump($labeled_spend instanceof secp256k1_pubkey);
$labeled_ser = secp256k1_ec_pubkey_serialize($labeled_spend);
$unlabeled_ser = secp256k1_ec_pubkey_serialize($spend_pubkey);
var_dump($labeled_ser !== $unlabeled_ser);

// different labels produce different labeled spend pubkeys
$labeled_spend1 = secp256k1_silentpayments_recipient_create_labeled_spend_pubkey($spend_pubkey, $label1);
$labeled_ser1 = secp256k1_ec_pubkey_serialize($labeled_spend1);
var_dump($labeled_ser !== $labeled_ser1);

// cannot instantiate label directly
try {
    new secp256k1_silentpayments_label();
} catch (Error $e) {
    echo $e->getMessage() . "\n";
}

// error: wrong scan key length
try {
    secp256k1_silentpayments_recipient_label_create("short", 0, $t);
} catch (ValueError $e) {
    echo $e->getMessage() . "\n";
}

// error: negative m
try {
    secp256k1_silentpayments_recipient_label_create($scan_key, -1, $t);
} catch (ValueError $e) {
    echo $e->getMessage() . "\n";
}

// parse fails with invalid point (33 bytes of zeros is not on the curve)
var_dump(secp256k1_silentpayments_recipient_label_parse(str_repeat("\x00", 33)));

// error: wrong parse input length
try {
    secp256k1_silentpayments_recipient_label_parse("short");
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
bool(true)
bool(true)
bool(true)
bool(true)
bool(true)
Cannot instantiate secp256k1_silentpayments_label directly, use secp256k1_silentpayments_recipient_label_create() or secp256k1_silentpayments_recipient_label_parse()
secp256k1_silentpayments_recipient_label_create(): Argument #1 ($scan_key32) must be exactly 32 bytes
secp256k1_silentpayments_recipient_label_create(): Argument #2 ($m) must be between 0 and (2^32)-1
bool(false)
secp256k1_silentpayments_recipient_label_parse(): Argument #1 ($in33) must be exactly 33 bytes
