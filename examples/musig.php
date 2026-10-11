<?php

/**
 * MuSig2 (BIP-327) 3-of-3 multisignature example.
 *
 * Adapted from libsecp256k1's examples/musig.c
 *
 * Scenario:
 *   - Three signers each generate a keypair
 *   - Public keys are sorted and aggregated into a single aggregate key
 *   - The aggregate key is tweaked (BIP 32 plain tweak + BIP 341 Taproot tweak)
 *   - Each signer generates a nonce and shares the public part
 *   - Nonces are aggregated, a signing session is created
 *   - Each signer produces a partial signature
 *   - Partial signatures are verified individually, then aggregated
 *   - The final 64-byte Schnorr signature is verified against the tweaked aggregate key
 */

$n_signers = 3;

// ============================================================
// Step 1: Create keypairs
// ============================================================

echo "Creating key pairs......";

$seckeys = [];
$keypairs = [];
$pubkeys = [];
for ($i = 0; $i < $n_signers; $i++) {
    $seckeys[$i] = random_bytes(32);
    $keypairs[$i] = secp256k1_keypair_create($seckeys[$i]);
    if ($keypairs[$i] === false) {
        throw new \Exception('Failed to create keypair');
    }
    $pubkeys[$i] = secp256k1_keypair_pub($keypairs[$i]);
}

echo "ok" . PHP_EOL;

// ============================================================
// Step 2: Sort and aggregate public keys
// ============================================================

echo "Combining public keys...";

// Sort signers for a canonical aggregate key independent of signer order.
// secp256k1_ec_pubkey_sort is not available in our binding, so we sort by
// pubkey comparison and reorder all arrays together.
$indices = range(0, $n_signers - 1);
usort($indices, fn($a, $b) => secp256k1_ec_pubkey_cmp($pubkeys[$a], $pubkeys[$b]));
$pubkeys = array_map(fn($i) => $pubkeys[$i], $indices);
$keypairs = array_map(fn($i) => $keypairs[$i], $indices);
$seckeys = array_map(fn($i) => $seckeys[$i], $indices);

$cache = secp256k1_musig_pubkey_agg($pubkeys);
if ($cache === false) {
    throw new \Exception('Failed to aggregate public keys');
}

echo "ok" . PHP_EOL;

// ============================================================
// Step 3: Tweak the aggregate key
// ============================================================

echo "Tweaking................";

// Plain tweaking (BIP 32 child key derivation)
$plain_tweak = str_pad("this could be a BIP32 tweak", 32, ".");
$tweaked_pk = secp256k1_musig_pubkey_ec_tweak_add($cache, $plain_tweak);
if ($tweaked_pk === false) {
    throw new \Exception('Failed to apply plain tweak');
}

// X-only tweaking (BIP 341 Taproot commitment)
$xonly_tweak = str_pad("this could be a Taproot tweak", 32, ".");
$tweaked_pk = secp256k1_musig_pubkey_xonly_tweak_add($cache, $xonly_tweak);
if ($tweaked_pk === false) {
    throw new \Exception('Failed to apply xonly tweak');
}

// Convert to x-only for final Schnorr verification
$agg_pk = secp256k1_xonly_pubkey_from_pubkey($tweaked_pk, $parity);

echo "ok" . PHP_EOL;

// ============================================================
// Step 4: Generate nonces (round 1)
// ============================================================

echo "Signing message.........";

$msg32 = str_pad("this_could_be_the_hash_of_a_msg", 32, "\0");

$secnonces = [];
$pubnonces = [];
for ($i = 0; $i < $n_signers; $i++) {
    $nonce = secp256k1_musig_nonce_gen(
        $pubkeys[$i],
        $seckeys[$i],
        $msg32,
        $cache
    );
    if ($nonce === false) {
        throw new \Exception("Failed to generate nonce for signer $i");
    }
    $secnonces[$i] = $nonce['secnonce'];
    $pubnonces[$i] = $nonce['pubnonce'];
}

// ============================================================
// Step 5: Aggregate nonces and create signing session
// ============================================================

$aggnonce = secp256k1_musig_nonce_agg($pubnonces);
if ($aggnonce === false) {
    throw new \Exception('Failed to aggregate nonces');
}

$session = secp256k1_musig_nonce_process($aggnonce, $msg32, $cache);
if ($session === false) {
    throw new \Exception('Failed to process nonce');
}

// ============================================================
// Step 6: Each signer produces a partial signature
// ============================================================

$partial_sigs = [];
for ($i = 0; $i < $n_signers; $i++) {
    $psig = secp256k1_musig_partial_sign($secnonces[$i], $keypairs[$i], $cache, $session);
    if ($psig === false) {
        throw new \Exception("Failed to create partial signature for signer $i");
    }
    $partial_sigs[$i] = $psig;
}

// ============================================================
// Step 7: Verify partial signatures and aggregate
// ============================================================

for ($i = 0; $i < $n_signers; $i++) {
    if (!secp256k1_musig_partial_sig_verify($partial_sigs[$i], $pubnonces[$i], $pubkeys[$i], $cache, $session)) {
        throw new \Exception("Partial signature verification failed for signer $i");
    }
}

$sig64 = secp256k1_musig_partial_sig_agg($session, $partial_sigs);
if ($sig64 === false) {
    throw new \Exception('Failed to aggregate partial signatures');
}

echo "ok" . PHP_EOL;

// ============================================================
// Step 8: Verify the final Schnorr signature
// ============================================================

echo "Verifying signature.....";

if (!secp256k1_schnorrsig_verify($sig64, $msg32, $agg_pk)) {
    throw new \Exception('Signature verification failed');
}

echo "ok" . PHP_EOL;

// Clear secret key material
for ($i = 0; $i < $n_signers; $i++) {
    sodium_memzero($seckeys[$i]);
}
