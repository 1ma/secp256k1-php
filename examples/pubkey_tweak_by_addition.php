<?php

$publicKeyBin = hex2bin('03fae8f5e64c9997749ef65c5db9f0ec3e121dc6901096c30da0f105a13212b6db');

$publicKey = secp256k1_ec_pubkey_parse($publicKeyBin);
if ($publicKey === false) {
    throw new \Exception('Invalid public key');
}

$tweak = hex2bin('0000000000000000000000000000000000000000000000000000000000000001');
if (!secp256k1_ec_pubkey_tweak_add($publicKey, $tweak)) {
    throw new \Exception('Invalid public key or tweak value');
}

echo sprintf('Tweaked public key: %s', bin2hex(secp256k1_ec_pubkey_serialize($publicKey))) . PHP_EOL;
