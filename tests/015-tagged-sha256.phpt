--TEST--
secp256k1_tagged_sha256
--EXTENSIONS--
secp256k1
--FILE--
<?php

// tagged_sha256 computes SHA256(SHA256(tag) || SHA256(tag) || msg)
// verify against a known vector computed with PHP's hash functions
$tag = "BIP0340/challenge";
$msg = str_repeat("\xab", 32);

$result = secp256k1_tagged_sha256($tag, $msg);
var_dump(strlen($result) === 32);

// compute expected: SHA256(SHA256(tag) || SHA256(tag) || msg)
$tag_hash = hash("sha256", $tag, true);
$expected = hash("sha256", $tag_hash . $tag_hash . $msg, true);
var_dump($result === $expected);

// different tag produces different hash
$result2 = secp256k1_tagged_sha256("other/tag", $msg);
var_dump($result !== $result2);

// different msg produces different hash
$result3 = secp256k1_tagged_sha256($tag, str_repeat("\xcd", 32));
var_dump($result !== $result3);

// empty tag and empty msg work
$result4 = secp256k1_tagged_sha256("", "");
var_dump(strlen($result4) === 32);
$tag_hash_empty = hash("sha256", "", true);
$expected_empty = hash("sha256", $tag_hash_empty . $tag_hash_empty, true);
var_dump($result4 === $expected_empty);

?>
--EXPECT--
bool(true)
bool(true)
bool(true)
bool(true)
bool(true)
bool(true)
