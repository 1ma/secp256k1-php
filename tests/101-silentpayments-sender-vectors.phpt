--TEST--
secp256k1_silentpayments_sender_create_outputs with BIP-352 known-answer vectors
--EXTENSIONS--
secp256k1
--SKIPIF--
<?php
if (!function_exists('secp256k1_silentpayments_sender_create_outputs')) die('skip silentpayments not available');
?>
--FILE--
<?php
// BIP-352 vector 0: Simple send: two non-taproot inputs
$outpoint = hex2bin("169e1e83e930853391bc6f35f605c6754cfead57cf8387639d3b4096c54f18f400000000");
$scan = secp256k1_ec_pubkey_parse(hex2bin("0220bcfac5b99e04ad1a06ddfb016ee13582609d60b6291e98d01a9bc9a16c96d4"));
$spend = secp256k1_ec_pubkey_parse(hex2bin("025cc9856d6f8375350e123978daac200c260cb5b5ae83106cab90484dcd8fcf36"));
$sk0 = hex2bin("eadc78165ff1f8ea94ad7cfdc54990738a4c53f6e0507b42154201b8e5dff3b1");
$sk1 = hex2bin("93f5ed907ad5b2bdbbdcb5d9116ebc0a4e1f92f910d5260237fa45a9408aad16");
$out = secp256k1_silentpayments_sender_create_outputs($outpoint, [$scan], [$spend], [], [$sk0, $sk1]);
var_dump(bin2hex(secp256k1_xonly_pubkey_serialize($out[0])) === "3e9fce73d4e77a4809908e3c3a2e54ee147b9312dc5044a193d1fc85de46e3c1");

// BIP-352 vector 6: taproot-only inputs (even y-values)
$kp0 = secp256k1_keypair_create(hex2bin("eadc78165ff1f8ea94ad7cfdc54990738a4c53f6e0507b42154201b8e5dff3b1"));
$kp1 = secp256k1_keypair_create(hex2bin("fc8716a97a48ba9a05a98ae47b5cd201a25a7fd5d8b73c203c5f7b6b6b3b6ad7"));
$out = secp256k1_silentpayments_sender_create_outputs($outpoint, [$scan], [$spend], [$kp0, $kp1], []);
var_dump(bin2hex(secp256k1_xonly_pubkey_serialize($out[0])) === "de88bea8e7ffc9ce1af30d1132f910323c505185aec8eae361670421e749a1fb");

// BIP-352 vector 8: mixed inputs (taproot + non-taproot)
$kp = secp256k1_keypair_create(hex2bin("eadc78165ff1f8ea94ad7cfdc54990738a4c53f6e0507b42154201b8e5dff3b1"));
$sk = hex2bin("8d4751f6e8a3586880fb66c19ae277969bd5aa06f61c4ee2f1e2486efdf666d3");
$out = secp256k1_silentpayments_sender_create_outputs($outpoint, [$scan], [$spend], [$kp], [$sk]);
var_dump(bin2hex(secp256k1_xonly_pubkey_serialize($out[0])) === "30523cca96b2a9ae3c98beb5e60f7d190ec5bc79b2d11a0b2d4d09a608c448f0");
?>
--EXPECT--
bool(true)
bool(true)
bool(true)
