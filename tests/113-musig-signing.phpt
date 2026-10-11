--TEST--
secp256k1_musig full 2-of-2 signing roundtrip
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
$msg32 = hash('sha256', 'MuSig2 test message', true);

// Keypairs and aggregation
$kp1 = secp256k1_keypair_create($sk1);
$kp2 = secp256k1_keypair_create($sk2);
$cache = secp256k1_musig_pubkey_agg([$pk1, $pk2]);
$agg_pk = secp256k1_musig_pubkey_get($cache);
$agg_xonly = secp256k1_xonly_pubkey_from_pubkey($agg_pk, $parity);

// Each signer generates nonces
['secnonce' => $sn1, 'pubnonce' => $pn1] = secp256k1_musig_nonce_gen($pk1, $sk1, $msg32, $cache);
['secnonce' => $sn2, 'pubnonce' => $pn2] = secp256k1_musig_nonce_gen($pk2, $sk2, $msg32, $cache);

// Aggregate nonces
$aggnonce = secp256k1_musig_nonce_agg([$pn1, $pn2]);

// Create session
$session = secp256k1_musig_nonce_process($aggnonce, $msg32, $cache);
var_dump($session instanceof secp256k1_musig_session);

// Each signer produces a partial signature
$psig1 = secp256k1_musig_partial_sign($sn1, $kp1, $cache, $session);
var_dump($psig1 instanceof secp256k1_musig_partial_sig);
$psig2 = secp256k1_musig_partial_sign($sn2, $kp2, $cache, $session);
var_dump($psig2 instanceof secp256k1_musig_partial_sig);

// Verify each partial signature
var_dump(secp256k1_musig_partial_sig_verify($psig1, $pn1, $pk1, $cache, $session));
var_dump(secp256k1_musig_partial_sig_verify($psig2, $pn2, $pk2, $cache, $session));

// Aggregate partial signatures
$sig64 = secp256k1_musig_partial_sig_agg($session, [$psig1, $psig2]);
var_dump(strlen($sig64) === 64);

// Verify with schnorrsig_verify
var_dump(secp256k1_schnorrsig_verify($sig64, $msg32, $agg_xonly));

// Counter-based nonce generation
$nonce_cnt = secp256k1_musig_nonce_gen_counter(0, $kp1);
var_dump($nonce_cnt['secnonce'] instanceof secp256k1_musig_secnonce);
var_dump($nonce_cnt['pubnonce'] instanceof secp256k1_musig_pubnonce);

// Counter-based nonce with all optional params
$cache_cnt = secp256k1_musig_pubkey_agg([$pk1, $pk2]);
$nonce_cnt_full = secp256k1_musig_nonce_gen_counter(1, $kp1, str_repeat("\xCC", 32), $cache_cnt, str_repeat("\xDD", 32));
var_dump($nonce_cnt_full['secnonce'] instanceof secp256k1_musig_secnonce);

// Nonce gen with all optional params
$cache_full = secp256k1_musig_pubkey_agg([$pk1, $pk2]);
$nonce_full = secp256k1_musig_nonce_gen($pk1, $sk1, str_repeat("\xCC", 32), $cache_full, str_repeat("\xDD", 32));
var_dump($nonce_full['secnonce'] instanceof secp256k1_musig_secnonce);

// Error: wrong msg length for nonce_process
try {
    secp256k1_musig_nonce_process($aggnonce, "short", $cache);
} catch (ValueError $e) {
    echo $e->getMessage() . "\n";
}

// Error: empty partial_sigs array
try {
    secp256k1_musig_partial_sig_agg($session, []);
} catch (ValueError $e) {
    echo $e->getMessage() . "\n";
}

// Error: wrong type in partial_sigs array
try {
    secp256k1_musig_partial_sig_agg($session, [$psig1, "not a sig"]);
} catch (TypeError $e) {
    echo $e->getMessage() . "\n";
}

// Error: wrong seckey length
try {
    secp256k1_musig_nonce_gen($pk1, "short");
} catch (ValueError $e) {
    echo $e->getMessage() . "\n";
}

// Error: wrong msg length
try {
    secp256k1_musig_nonce_gen($pk1, null, "short");
} catch (ValueError $e) {
    echo $e->getMessage() . "\n";
}

// Error: wrong extra_input length
try {
    secp256k1_musig_nonce_gen($pk1, null, null, null, "short");
} catch (ValueError $e) {
    echo $e->getMessage() . "\n";
}

// Error: wrong msg length for nonce_gen_counter
try {
    secp256k1_musig_nonce_gen_counter(0, $kp1, "short");
} catch (ValueError $e) {
    echo $e->getMessage() . "\n";
}

// Error: wrong extra_input length for nonce_gen_counter
try {
    secp256k1_musig_nonce_gen_counter(0, $kp1, null, null, "short");
} catch (ValueError $e) {
    echo $e->getMessage() . "\n";
}

// Error: empty pubnonces array
try {
    secp256k1_musig_nonce_agg([]);
} catch (ValueError $e) {
    echo $e->getMessage() . "\n";
}

// Error: wrong type in pubnonces array
try {
    secp256k1_musig_nonce_agg([$pn1, "not a nonce"]);
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
secp256k1_musig_nonce_process(): Argument #2 ($msg32) must be exactly 32 bytes
secp256k1_musig_partial_sig_agg(): Argument #2 ($partial_sigs) must not be empty
secp256k1_musig_partial_sig_agg(): Argument #2 ($partial_sigs) must contain only secp256k1_musig_partial_sig objects
secp256k1_musig_nonce_gen(): Argument #2 ($seckey32) must be exactly 32 bytes
secp256k1_musig_nonce_gen(): Argument #3 ($msg32) must be exactly 32 bytes
secp256k1_musig_nonce_gen(): Argument #5 ($extra_input32) must be exactly 32 bytes
secp256k1_musig_nonce_gen_counter(): Argument #3 ($msg32) must be exactly 32 bytes
secp256k1_musig_nonce_gen_counter(): Argument #5 ($extra_input32) must be exactly 32 bytes
secp256k1_musig_nonce_agg(): Argument #1 ($pubnonces) must not be empty
secp256k1_musig_nonce_agg(): Argument #1 ($pubnonces) must contain only secp256k1_musig_pubnonce objects
