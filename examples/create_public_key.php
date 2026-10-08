<?php

$privateKey = hex2bin('abcdef0123456789abcdef0123456789abcdef0123456789abcdef0123456789');

$publicKey = secp256k1_ec_pubkey_create($privateKey);
if ($publicKey === false) {
    throw new \Exception('secp256k1_ec_pubkey_create: secret key was invalid');
}

echo bin2hex(secp256k1_ec_pubkey_serialize($publicKey)) . PHP_EOL;
