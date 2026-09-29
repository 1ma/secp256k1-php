<?php

/**
 * @generate-class-entries
 * @undocumentable
 * @generate-legacy-arginfo 80200
 */

/**
 * @var int
 * @cvalue SECP256K1_EC_COMPRESSED
 */
const SECP256K1_EC_COMPRESSED = UNKNOWN;

/**
 * @var int
 * @cvalue SECP256K1_EC_UNCOMPRESSED
 */
const SECP256K1_EC_UNCOMPRESSED = UNKNOWN;

/**
 * @strict-properties
 * @not-serializable
 */
final class secp256k1_pubkey
{
}

function secp256k1_ec_seckey_verify(string $seckey32): bool {}

function secp256k1_ec_pubkey_create(string $seckey32): secp256k1_pubkey|false {}

function secp256k1_ec_pubkey_parse(string $pubkey): secp256k1_pubkey|false {}

function secp256k1_ec_pubkey_serialize(secp256k1_pubkey $pubkey, int $flags = SECP256K1_EC_COMPRESSED): string {}
