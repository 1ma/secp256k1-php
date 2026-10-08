<?php

$privateKey = hex2bin('88b59280e39997e49ebd47ecc9e3850faff5d7df1e2a22248c136cbdd0d60aae');
$tweak = hex2bin('0000000000000000000000000000000000000000000000000000000000000001');

$tweaked = secp256k1_ec_seckey_tweak_add($privateKey, $tweak);
if ($tweaked === false) {
    throw new \Exception('Invalid private key or tweak value');
}

echo sprintf('Tweaked private key: %s', bin2hex($tweaked)) . PHP_EOL;
