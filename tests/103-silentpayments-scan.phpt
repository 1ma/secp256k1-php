--TEST--
secp256k1_silentpayments_recipient prevouts_summary_create and scan_outputs
--EXTENSIONS--
secp256k1
--SKIPIF--
<?php
if (!function_exists('secp256k1_silentpayments_recipient_scan_outputs')) die('skip silentpayments not available');
?>
--FILE--
<?php
// === Setup: sender creates outputs, receiver scans ===

$sender_seckey = str_repeat("\x02", 32);
$sender_pubkey = secp256k1_ec_pubkey_create($sender_seckey);
$sender_keypair = secp256k1_keypair_create($sender_seckey);

$scan_seckey = str_repeat("\x03", 32);
$scan_pubkey = secp256k1_ec_pubkey_create($scan_seckey);

$spend_seckey = str_repeat("\x04", 32);
$spend_pubkey = secp256k1_ec_pubkey_create($spend_seckey);

$outpoint = str_repeat("\x00", 32) . "\x00\x00\x00\x00";

// --- Roundtrip with non-taproot input ---
$outputs = secp256k1_silentpayments_sender_create_outputs(
    $outpoint, [$scan_pubkey], [$spend_pubkey], [], [$sender_seckey]
);
var_dump(is_array($outputs) && count($outputs) === 1);

// Receiver side: create prevouts summary from sender's pubkey
$summary = secp256k1_silentpayments_recipient_prevouts_summary_create(
    $outpoint, [], [$sender_pubkey]
);
var_dump($summary instanceof secp256k1_silentpayments_prevouts_summary);

// Scan the outputs
$found = secp256k1_silentpayments_recipient_scan_outputs(
    $scan_seckey, $spend_pubkey, $summary, $outputs
);
var_dump(is_array($found));
var_dump(count($found) === 1);
var_dump($found[0]['output'] instanceof secp256k1_xonly_pubkey);
var_dump(strlen($found[0]['tweak']) === 32);
var_dump($found[0]['found_with_label'] === false);
var_dump($found[0]['label'] === null);

// Verify the found output matches the sender's output
$found_ser = secp256k1_xonly_pubkey_serialize($found[0]['output']);
$expected_ser = secp256k1_xonly_pubkey_serialize($outputs[0]);
var_dump($found_ser === $expected_ser);

// --- Roundtrip with taproot input ---
$outputs_tr = secp256k1_silentpayments_sender_create_outputs(
    $outpoint, [$scan_pubkey], [$spend_pubkey], [$sender_keypair], []
);
$sender_xonly = secp256k1_keypair_xonly_pub($sender_keypair, $parity);
$summary_tr = secp256k1_silentpayments_recipient_prevouts_summary_create(
    $outpoint, [$sender_xonly], []
);
$found_tr = secp256k1_silentpayments_recipient_scan_outputs(
    $scan_seckey, $spend_pubkey, $summary_tr, $outputs_tr
);
var_dump(count($found_tr) === 1);
$found_tr_ser = secp256k1_xonly_pubkey_serialize($found_tr[0]['output']);
$expected_tr_ser = secp256k1_xonly_pubkey_serialize($outputs_tr[0]);
var_dump($found_tr_ser === $expected_tr_ser);

// --- Roundtrip with labels ---
$label = secp256k1_silentpayments_recipient_label_create($scan_seckey, 1, $tweak);
$labeled_spend = secp256k1_silentpayments_recipient_create_labeled_spend_pubkey($spend_pubkey, $label);
$label_ser = secp256k1_silentpayments_recipient_label_serialize($label);
$labels = [$label_ser => $tweak];

$outputs_lab = secp256k1_silentpayments_sender_create_outputs(
    $outpoint, [$scan_pubkey], [$labeled_spend], [], [$sender_seckey]
);

$summary_lab = secp256k1_silentpayments_recipient_prevouts_summary_create(
    $outpoint, [], [$sender_pubkey]
);
$found_lab = secp256k1_silentpayments_recipient_scan_outputs(
    $scan_seckey, $spend_pubkey, $summary_lab, $outputs_lab, $labels
);
var_dump(count($found_lab) === 1);
var_dump($found_lab[0]['found_with_label'] === true);
var_dump($found_lab[0]['label'] instanceof secp256k1_silentpayments_label);

// --- Decoy output triggers label_lookup returning NULL ---
$decoy_xonly = secp256k1_xonly_pubkey_parse(str_repeat("\x01", 32));
$found_decoy = secp256k1_silentpayments_recipient_scan_outputs(
    $scan_seckey, $spend_pubkey, $summary, [$outputs[0], $decoy_xonly], $labels
);
var_dump(count($found_decoy) === 1);
var_dump($found_decoy[0]['found_with_label'] === false);

// --- No outputs found (wrong scan key) ---
$wrong_scan = str_repeat("\x99", 32);
$found_none = secp256k1_silentpayments_recipient_scan_outputs(
    $wrong_scan, $spend_pubkey, $summary, $outputs
);
var_dump(is_array($found_none) && count($found_none) === 0);

// --- Cannot instantiate prevouts_summary directly ---
try {
    new secp256k1_silentpayments_prevouts_summary();
} catch (Error $e) {
    echo $e->getMessage() . "\n";
}

// --- Error: wrong outpoint length ---
try {
    secp256k1_silentpayments_recipient_prevouts_summary_create("short", [], []);
} catch (ValueError $e) {
    echo $e->getMessage() . "\n";
}

// --- Error: both arrays empty ---
try {
    secp256k1_silentpayments_recipient_prevouts_summary_create($outpoint, [], []);
} catch (ValueError $e) {
    echo $e->getMessage() . "\n";
}

// --- Error: wrong scan key length ---
try {
    secp256k1_silentpayments_recipient_scan_outputs("short", $spend_pubkey, $summary, $outputs);
} catch (ValueError $e) {
    echo $e->getMessage() . "\n";
}

// --- Error: empty tx_outputs ---
try {
    secp256k1_silentpayments_recipient_scan_outputs($scan_seckey, $spend_pubkey, $summary, []);
} catch (ValueError $e) {
    echo $e->getMessage() . "\n";
}

// --- Error: wrong type in tx_outputs ---
try {
    secp256k1_silentpayments_recipient_scan_outputs($scan_seckey, $spend_pubkey, $summary, [$scan_pubkey]);
} catch (TypeError $e) {
    echo $e->getMessage() . "\n";
}

// --- Error: wrong type in xonly_pubkeys ---
try {
    secp256k1_silentpayments_recipient_prevouts_summary_create($outpoint, [$sender_pubkey], []);
} catch (TypeError $e) {
    echo $e->getMessage() . "\n";
}

// --- Error: wrong type in pubkeys ---
try {
    $xonly = secp256k1_keypair_xonly_pub($sender_keypair, $p);
    secp256k1_silentpayments_recipient_prevouts_summary_create($outpoint, [], [$xonly]);
} catch (TypeError $e) {
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
bool(true)
bool(true)
bool(true)
bool(true)
bool(true)
bool(true)
bool(true)
Cannot instantiate secp256k1_silentpayments_prevouts_summary directly, use secp256k1_silentpayments_recipient_prevouts_summary_create()
secp256k1_silentpayments_recipient_prevouts_summary_create(): Argument #1 ($outpoint_smallest36) must be exactly 36 bytes
secp256k1_silentpayments_recipient_prevouts_summary_create(): Argument #2 ($xonly_pubkeys) at least one of xonly_pubkeys or pubkeys must be non-empty
secp256k1_silentpayments_recipient_scan_outputs(): Argument #1 ($scan_key32) must be exactly 32 bytes
secp256k1_silentpayments_recipient_scan_outputs(): Argument #4 ($tx_outputs) must not be empty
secp256k1_silentpayments_recipient_scan_outputs(): Argument #4 ($tx_outputs) must contain only secp256k1_xonly_pubkey objects
secp256k1_silentpayments_recipient_prevouts_summary_create(): Argument #2 ($xonly_pubkeys) must contain only secp256k1_xonly_pubkey objects
secp256k1_silentpayments_recipient_prevouts_summary_create(): Argument #3 ($pubkeys) must contain only secp256k1_pubkey objects
