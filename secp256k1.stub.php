<?php

/**
 * @generate-class-entries
 * @undocumentable
 * @generate-legacy-arginfo 80200
 */

function secp256k1_context_randomize(): bool {}

function secp256k1_tagged_sha256(string $tag, string $msg): string {}

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

function secp256k1_ec_seckey_negate(string $seckey32): string|false {}

function secp256k1_ec_seckey_tweak_add(string $seckey32, string $tweak32): string|false {}

function secp256k1_ec_seckey_tweak_mul(string $seckey32, string $tweak32): string|false {}

function secp256k1_ec_pubkey_create(string $seckey32): secp256k1_pubkey|false {}

function secp256k1_ec_pubkey_parse(string $pubkey): secp256k1_pubkey|false {}

function secp256k1_ec_pubkey_serialize(secp256k1_pubkey $pubkey, int $flags = SECP256K1_EC_COMPRESSED): string {}

function secp256k1_ec_pubkey_negate(secp256k1_pubkey &$pubkey): void {}

function secp256k1_ec_pubkey_tweak_add(secp256k1_pubkey &$pubkey, string $tweak32): bool {}

function secp256k1_ec_pubkey_tweak_mul(secp256k1_pubkey &$pubkey, string $tweak32): bool {}

function secp256k1_ec_pubkey_combine(array $pubkeys): secp256k1_pubkey|false {}

function secp256k1_ec_pubkey_cmp(secp256k1_pubkey $pubkey1, secp256k1_pubkey $pubkey2): int {}

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

function secp256k1_xonly_pubkey_from_pubkey(secp256k1_pubkey $pubkey, int &$parity): secp256k1_xonly_pubkey {}

function secp256k1_xonly_pubkey_parse(string $input32): secp256k1_xonly_pubkey|false {}

function secp256k1_xonly_pubkey_serialize(secp256k1_xonly_pubkey $pubkey): string {}

function secp256k1_xonly_pubkey_tweak_add(secp256k1_xonly_pubkey $pubkey, string $tweak32): secp256k1_pubkey|false {}

function secp256k1_xonly_pubkey_tweak_add_check(string $tweaked_pubkey32, int $tweaked_pk_parity, secp256k1_xonly_pubkey $internal_pubkey, string $tweak32): bool {}

function secp256k1_xonly_pubkey_cmp(secp256k1_xonly_pubkey $pk1, secp256k1_xonly_pubkey $pk2): int {}

/**
 * @strict-properties
 * @not-serializable
 */
final class secp256k1_keypair
{
}

function secp256k1_keypair_create(string $seckey32): secp256k1_keypair|false {}

function secp256k1_keypair_sec(secp256k1_keypair $keypair): string {}

function secp256k1_keypair_pub(secp256k1_keypair $keypair): secp256k1_pubkey {}

function secp256k1_keypair_xonly_pub(secp256k1_keypair $keypair, int &$parity): secp256k1_xonly_pubkey {}

function secp256k1_keypair_xonly_tweak_add(secp256k1_keypair &$keypair, string $tweak32): bool {}
#endif

#if defined(HAVE_SECP256K1_RECOVERY)
/**
 * @strict-properties
 * @not-serializable
 */
final class secp256k1_ecdsa_recoverable_signature
{
}

function secp256k1_ecdsa_recoverable_signature_parse_compact(string $sig64, int $recid): secp256k1_ecdsa_recoverable_signature|false {}

function secp256k1_ecdsa_recoverable_signature_serialize_compact(secp256k1_ecdsa_recoverable_signature $sig, int &$recid): string {}

function secp256k1_ecdsa_recoverable_signature_convert(secp256k1_ecdsa_recoverable_signature $sig): secp256k1_ecdsa_signature {}

function secp256k1_ecdsa_sign_recoverable(string $msghash32, string $seckey32): secp256k1_ecdsa_recoverable_signature|false {}

function secp256k1_ecdsa_recover(secp256k1_ecdsa_recoverable_signature $sig, string $msghash32): secp256k1_pubkey|false {}
#endif

#if defined(HAVE_SECP256K1_SCHNORRSIG)
function secp256k1_schnorrsig_sign32(string $msghash32, secp256k1_keypair $keypair, ?string $aux_rand32 = null): string|false {}

function secp256k1_schnorrsig_sign_custom(string $msg, secp256k1_keypair $keypair): string|false {}

function secp256k1_schnorrsig_verify(string $sig64, string $msg, secp256k1_xonly_pubkey $pubkey): bool {}
#endif

#if defined(HAVE_SECP256K1_ELLSWIFT)
/**
 * @var int
 * @cvalue SECP256K1_ELLSWIFT_XDH_HASH_BIP324
 */
const SECP256K1_ELLSWIFT_XDH_HASH_BIP324 = UNKNOWN;

/**
 * @var int
 * @cvalue SECP256K1_ELLSWIFT_XDH_HASH_PREFIX
 */
const SECP256K1_ELLSWIFT_XDH_HASH_PREFIX = UNKNOWN;

function secp256k1_ellswift_create(string $seckey32, ?string $auxrnd32 = null): string|false {}

function secp256k1_ellswift_decode(string $ell64): secp256k1_pubkey {}

function secp256k1_ellswift_encode(secp256k1_pubkey $pubkey, string $rnd32): string {}

function secp256k1_ellswift_xdh(string $ell_a64, string $ell_b64, string $seckey32, bool $party, int $hashfn = SECP256K1_ELLSWIFT_XDH_HASH_BIP324, ?string $prefix64 = null): string|false {}
#endif
