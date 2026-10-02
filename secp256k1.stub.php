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

/**
 * @strict-properties
 * @not-serializable
 */
final class secp256k1_ecdsa_signature
{
}

function secp256k1_ecdsa_signature_parse_compact(string $sig64): secp256k1_ecdsa_signature|false {}

function secp256k1_ecdsa_signature_parse_der(string $der): secp256k1_ecdsa_signature|false {}

function secp256k1_ecdsa_signature_serialize_compact(secp256k1_ecdsa_signature $sig): string {}

function secp256k1_ecdsa_signature_serialize_der(secp256k1_ecdsa_signature $sig): string {}

function secp256k1_ecdsa_signature_normalize(secp256k1_ecdsa_signature &$sig): bool {}

function secp256k1_ecdsa_sign(string $msghash32, string $seckey32): secp256k1_ecdsa_signature|false {}

function secp256k1_ecdsa_verify(secp256k1_ecdsa_signature $sig, string $msghash32, secp256k1_pubkey $pubkey): bool {}

function secp256k1_ec_seckey_negate(string $seckey32): string|false {}

function secp256k1_ec_seckey_tweak_add(string $seckey32, string $tweak32): string|false {}

function secp256k1_ec_seckey_tweak_mul(string $seckey32, string $tweak32): string|false {}

function secp256k1_ec_pubkey_negate(secp256k1_pubkey &$pubkey): void {}

function secp256k1_ec_pubkey_tweak_add(secp256k1_pubkey &$pubkey, string $tweak32): bool {}

function secp256k1_ec_pubkey_tweak_mul(secp256k1_pubkey &$pubkey, string $tweak32): bool {}

function secp256k1_ec_pubkey_combine(array $pubkeys): secp256k1_pubkey|false {}

function secp256k1_ec_pubkey_cmp(secp256k1_pubkey $pubkey1, secp256k1_pubkey $pubkey2): int {}

#if defined(HAVE_SECP256K1_ECDH)
function secp256k1_ecdh(secp256k1_pubkey $pubkey, string $seckey32): string|false {}
#endif

#if defined(HAVE_SECP256K1_EXTRAKEYS)
/**
 * @strict-properties
 * @not-serializable
 */
final class secp256k1_xonly_pubkey
{
}

function secp256k1_xonly_pubkey_parse(string $input32): secp256k1_xonly_pubkey|false {}

function secp256k1_xonly_pubkey_serialize(secp256k1_xonly_pubkey $pubkey): string {}

function secp256k1_xonly_pubkey_cmp(secp256k1_xonly_pubkey $pk1, secp256k1_xonly_pubkey $pk2): int {}

function secp256k1_xonly_pubkey_from_pubkey(secp256k1_pubkey $pubkey, int &$parity): secp256k1_xonly_pubkey {}

function secp256k1_xonly_pubkey_tweak_add(secp256k1_xonly_pubkey $pubkey, string $tweak32): secp256k1_pubkey|false {}

function secp256k1_xonly_pubkey_tweak_add_check(string $tweaked_pubkey32, int $tweaked_pk_parity, secp256k1_xonly_pubkey $internal_pubkey, string $tweak32): bool {}

/**
 * @strict-properties
 * @not-serializable
 */
final class secp256k1_keypair
{
}

function secp256k1_keypair_create(string $seckey32): secp256k1_keypair|false {}

function secp256k1_keypair_pub(secp256k1_keypair $keypair): secp256k1_pubkey {}

function secp256k1_keypair_xonly_pub(secp256k1_keypair $keypair, int &$parity): secp256k1_xonly_pubkey {}

function secp256k1_keypair_sec(secp256k1_keypair $keypair): string {}

function secp256k1_keypair_xonly_tweak_add(secp256k1_keypair &$keypair, string $tweak32): bool {}
#endif
