<?php

$publicKeyBin = hex2bin('04ae1a62fe09c5f51b13905f07f06b99a2f7159b2225f374cd378d71302fa28414e7aab37397f554a7df5f142c21c1b7303b8a0626f1baded5c72a704f7e6cd84c');

$publicKey = secp256k1_ec_pubkey_parse($publicKeyBin);
if ($publicKey === false) {
    throw new \RuntimeException('Failed to parse public key');
}

echo bin2hex(secp256k1_ec_pubkey_serialize($publicKey)) . PHP_EOL;
