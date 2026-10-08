<?php

$msg32 = hash('sha256', 'this is a message!', true);
$privateKey = hex2bin('88b59280e39997e49ebd47ecc9e3850faff5d7df1e2a22248c136cbdd0d60aae');

$signature = secp256k1_ecdsa_sign($msg32, $privateKey);
if ($signature === false) {
    throw new \Exception('Failed to create signature');
}

echo sprintf('Produced signature: %s', bin2hex(secp256k1_ecdsa_signature_serialize_der($signature))) . PHP_EOL;
