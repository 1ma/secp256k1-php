--TEST--
secp256k1_musig pubnonce, aggnonce and partial_sig serialization roundtrips
--EXTENSIONS--
secp256k1
--SKIPIF--
<?php if (!function_exists('secp256k1_musig_pubkey_agg')) die('skip musig not available'); ?>
--FILE--
<?php

$sk1 = str_repeat("\x01", 32);
$sk2 = str_repeat("\x02", 32);
$pk1 = secp256k1_ec_pubkey_create($sk1);
$pk2 = secp256k1_ec_pubkey_create($sk2);

// Generate nonces to get pubnonce objects
['pubnonce' => $pn1] = secp256k1_musig_nonce_gen($pk1);
['pubnonce' => $pn2] = secp256k1_musig_nonce_gen($pk2);

// Pubnonce roundtrip
$pn1_ser = secp256k1_musig_pubnonce_serialize($pn1);
var_dump(strlen($pn1_ser) === 66);
$pn1_parsed = secp256k1_musig_pubnonce_parse($pn1_ser);
var_dump($pn1_parsed instanceof secp256k1_musig_pubnonce);
var_dump(secp256k1_musig_pubnonce_serialize($pn1_parsed) === $pn1_ser);

// Aggnonce roundtrip
$aggnonce = secp256k1_musig_nonce_agg([$pn1, $pn2]);
$agg_ser = secp256k1_musig_aggnonce_serialize($aggnonce);
var_dump(strlen($agg_ser) === 66);
$agg_parsed = secp256k1_musig_aggnonce_parse($agg_ser);
var_dump($agg_parsed instanceof secp256k1_musig_aggnonce);
var_dump(secp256k1_musig_aggnonce_serialize($agg_parsed) === $agg_ser);

// Partial sig roundtrip (need a full signing flow to get one)
$cache = secp256k1_musig_pubkey_agg([$pk1, $pk2]);
$msg32 = str_repeat("\xAA", 32);
['secnonce' => $sn1, 'pubnonce' => $spn1] = secp256k1_musig_nonce_gen($pk1, $sk1, $msg32, $cache);
['secnonce' => $sn2, 'pubnonce' => $spn2] = secp256k1_musig_nonce_gen($pk2, $sk2, $msg32, $cache);
$aggnonce = secp256k1_musig_nonce_agg([$spn1, $spn2]);
$session = secp256k1_musig_nonce_process($aggnonce, $msg32, $cache);
$kp1 = secp256k1_keypair_create($sk1);
$psig = secp256k1_musig_partial_sign($sn1, $kp1, $cache, $session);

$psig_ser = secp256k1_musig_partial_sig_serialize($psig);
var_dump(strlen($psig_ser) === 32);
$psig_parsed = secp256k1_musig_partial_sig_parse($psig_ser);
var_dump($psig_parsed instanceof secp256k1_musig_partial_sig);
var_dump(secp256k1_musig_partial_sig_serialize($psig_parsed) === $psig_ser);

// Parse errors: wrong input length
try {
    secp256k1_musig_pubnonce_parse("short");
} catch (ValueError $e) {
    echo $e->getMessage() . "\n";
}

try {
    secp256k1_musig_aggnonce_parse("short");
} catch (ValueError $e) {
    echo $e->getMessage() . "\n";
}

try {
    secp256k1_musig_partial_sig_parse("short");
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
secp256k1_musig_pubnonce_parse(): Argument #1 ($in66) must be exactly 66 bytes
secp256k1_musig_aggnonce_parse(): Argument #1 ($in66) must be exactly 66 bytes
secp256k1_musig_partial_sig_parse(): Argument #1 ($in32) must be exactly 32 bytes
