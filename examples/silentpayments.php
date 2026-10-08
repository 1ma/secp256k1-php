<?php

/**
 * Silent Payments (BIP-352) end-to-end example.
 *
 * Adapted from libsecp256k1's examples/silentpayments.c
 *
 * Scenario:
 *   - Bob publishes a labeled Silent Payments address (label m=1)
 *   - Carol publishes a plain (unlabeled) Silent Payments address
 *   - Alice sends one output to Bob's labeled address and two outputs to Carol
 *   - Bob scans the transaction with his label cache and finds his output
 *   - Carol scans the transaction without labels and finds her two outputs
 *   - Bob verifies he can spend his output by reconstructing the full secret key
 */

$smallest_outpoint = hex2bin(
    '169e1e83e930853391bc6f35f605c6754cfead57cf8387639d3b4096c54f18f4' .
    '00000000'
);

// --- Bob's keys ---

$bob_scan_key = hex2bin('a89054c95be3c301566574f2aa93ade051850903a69cbdd1d47eae263d7bc031');
$bob_spend_key = hex2bin('9d6ad855ce3417ef84e836892e5a56392bfba05fa5d97ccea30e266f540e08b3');
$bob_scan_pubkey = secp256k1_ec_pubkey_parse(hex2bin(
    '021540aea897547ad439b4e0f609e5f0fa63de89ab11ede31e8cde4be219425f23'
));
$bob_spend_pubkey = secp256k1_ec_pubkey_parse(hex2bin(
    '025cc9856d6f8375350e123978daac200c260cb5b5ae83106cab90484dcd8fcf36'
));
// --- Carol's keys ---

$carol_scan_key = hex2bin('04b2a411635c097759aacd0f005a4c82c8c92862c6fc284b80b8efebc20c3d17');
$carol_scan_pubkey = secp256k1_ec_pubkey_parse(hex2bin(
    '03bbc63f12745d3b9e9d24c6cd7a1efebad0a7f469232fbecf31fba7b4f7ddeda8'
));
$carol_spend_pubkey = secp256k1_ec_pubkey_parse(hex2bin(
    '0381eb9a9a9ec739d527c1631b31b421566f5c2a47b4ab5b1f6a686dfb68eab716'
));

// ============================================================
// Bob creates a labeled Silent Payments address (label m=1)
// ============================================================

$label = secp256k1_silentpayments_recipient_label_create($bob_scan_key, 1, $label_tweak);
if ($label === false) {
    throw new \Exception('Failed to create label');
}

$label_serialized = secp256k1_silentpayments_recipient_label_serialize($label);

// Bob's label cache: maps serialized label (33 bytes) to tweak (32 bytes)
$bob_labels = [$label_serialized => $label_tweak];

// Bob's labeled spend pubkey (this goes into his published address)
$bob_labeled_spend_pubkey = secp256k1_silentpayments_recipient_create_labeled_spend_pubkey(
    $bob_spend_pubkey, $label
);
if ($bob_labeled_spend_pubkey === false) {
    throw new \Exception('Failed to create labeled spend pubkey');
}

echo "Bob's labeled spend pubkey: " . bin2hex(secp256k1_ec_pubkey_serialize($bob_labeled_spend_pubkey)) . PHP_EOL;

// ============================================================
// Alice sends to Bob (labeled) and Carol (2 outputs)
// ============================================================

// Generate 2 random taproot input keypairs for Alice
$sender_keypairs = [];
$tx_input_xonly = [];
for ($i = 0; $i < 2; $i++) {
    $seckey = random_bytes(32);
    $kp = secp256k1_keypair_create($seckey);
    if ($kp === false) {
        throw new \Exception('Failed to create keypair');
    }
    $sender_keypairs[] = $kp;
    $tx_input_xonly[] = secp256k1_keypair_xonly_pub($kp, $parity);
}

// Recipients:
//   [0] Carol (plain address)
//   [1] Bob  (labeled address)
//   [2] Carol (plain address, second output)
$scan_pubkeys  = [$carol_scan_pubkey,  $bob_scan_pubkey,    $carol_scan_pubkey];
$spend_pubkeys = [$carol_spend_pubkey, $bob_labeled_spend_pubkey, $carol_spend_pubkey];

$tx_outputs = secp256k1_silentpayments_sender_create_outputs(
    $smallest_outpoint,
    $scan_pubkeys,
    $spend_pubkeys,
    $sender_keypairs,
    []
);
if ($tx_outputs === false) {
    throw new \Exception('Failed to create outputs');
}

echo PHP_EOL . "Alice created the following outputs:" . PHP_EOL;
foreach ($tx_outputs as $i => $output) {
    echo "    [$i] " . bin2hex(secp256k1_xonly_pubkey_serialize($output)) . PHP_EOL;
}

// ============================================================
// Bob scans the transaction (full node, with labels)
// ============================================================

$prevouts_summary = secp256k1_silentpayments_recipient_prevouts_summary_create(
    $smallest_outpoint,
    $tx_input_xonly,
    []
);
if ($prevouts_summary === false) {
    throw new \Exception('Failed to create prevouts summary');
}

$bob_found = secp256k1_silentpayments_recipient_scan_outputs(
    $bob_scan_key,
    $bob_spend_pubkey,
    $prevouts_summary,
    $tx_outputs,
    $bob_labels
);

echo PHP_EOL;
if ($bob_found && count($bob_found) > 0) {
    echo "Bob found " . count($bob_found) . " output(s):" . PHP_EOL;
    foreach ($bob_found as $found) {
        $output_hex = bin2hex(secp256k1_xonly_pubkey_serialize($found['output']));
        $with_label = $found['found_with_label'] ? ' (labeled)' : '';
        echo "    $output_hex$with_label" . PHP_EOL;

        // Verify Bob can spend: add the tweak to his spend key
        $full_seckey = secp256k1_ec_seckey_tweak_add($bob_spend_key, $found['tweak']);
        if ($full_seckey === false) {
            throw new \Exception('Failed to tweak spend key');
        }
        $kp = secp256k1_keypair_create($full_seckey);
        $reconstructed = secp256k1_keypair_xonly_pub($kp, $parity);

        if (secp256k1_xonly_pubkey_cmp($reconstructed, $found['output']) === 0) {
            echo "    -> Verified: Bob can spend this output" . PHP_EOL;
        } else {
            throw new \Exception('Output mismatch — this should never happen');
        }
    }
} else {
    echo "Bob did not find any outputs." . PHP_EOL;
}

// ============================================================
// Carol scans the transaction (full node, no labels)
// ============================================================

$carol_found = secp256k1_silentpayments_recipient_scan_outputs(
    $carol_scan_key,
    $carol_spend_pubkey,
    $prevouts_summary,
    $tx_outputs
);

echo PHP_EOL;
if ($carol_found && count($carol_found) > 0) {
    echo "Carol found " . count($carol_found) . " output(s):" . PHP_EOL;
    foreach ($carol_found as $found) {
        echo "    " . bin2hex(secp256k1_xonly_pubkey_serialize($found['output'])) . PHP_EOL;
    }
} else {
    echo "Carol did not find any outputs." . PHP_EOL;
}
