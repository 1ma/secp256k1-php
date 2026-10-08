<?php

$publicKeyBin = hex2bin('03fae8f5e64c9997749ef65c5db9f0ec3e121dc6901096c30da0f105a13212b6db');
$tweak = hex2bin('0000000000000000000000000000000000000000000000000000000000000002');

$publicKey = secp256k1_ec_pubkey_parse($publicKeyBin);
if ($publicKey === false) {
    throw new \Exception('Failed to parse public key');
}

if (!secp256k1_ec_pubkey_tweak_mul($publicKey, $tweak)) {
    throw new \Exception('Invalid public key or tweak value');
}

echo sprintf('Tweaked public key: %s', bin2hex(secp256k1_ec_pubkey_serialize($publicKey))) . PHP_EOL;
