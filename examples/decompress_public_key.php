<?php

$publicKeyBin = hex2bin('02ae1a62fe09c5f51b13905f07f06b99a2f7159b2225f374cd378d71302fa28414');

$publicKey = secp256k1_ec_pubkey_parse($publicKeyBin);
if ($publicKey === false) {
    throw new \RuntimeException('Failed to parse public key');
}

echo bin2hex(secp256k1_ec_pubkey_serialize($publicKey, SECP256K1_EC_UNCOMPRESSED)) . PHP_EOL;
