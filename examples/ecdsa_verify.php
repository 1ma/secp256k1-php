<?php

$msg32 = hash('sha256', 'this is a message!', true);

$signatureRaw = hex2bin('3044022055ef6953afd139d917d947ba7823ab5dfb9239ba8a26295a218cad88fb7299ef022057147cf4233ff3b87fa64d82a0b9a327e9b6d5d0070ab3f671b795934c4f2074');
$publicKeyRaw = hex2bin('04fae8f5e64c9997749ef65c5db9f0ec3e121dc6901096c30da0f105a13212b6db4315e65a2d63cc667c034fac05cdb3c7bc1abfc2ad90f7f97321613f901758c9');

$publicKey = secp256k1_ec_pubkey_parse($publicKeyRaw);
if ($publicKey === false) {
    throw new \Exception('Failed to parse public key');
}

$signature = secp256k1_ecdsa_signature_parse_der($signatureRaw);
if ($signature === false) {
    throw new \Exception('Failed to parse DER signature');
}

if (secp256k1_ecdsa_verify($signature, $msg32, $publicKey)) {
    echo 'Signature was verified' . PHP_EOL;
} else {
    echo 'Signature was NOT VERIFIED' . PHP_EOL;
}
