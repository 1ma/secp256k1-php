# secp256k1 binding for PHP

This is a brand-new PHP extension where we'll try to build a thin binding for secp256k1,
because [the existing one](https://github.com/Bit-Wasp/secp256k1-php) hasn't seen any activity for 7 years.

The supported PHP versions will be 8.2 to 8.6 (still in development at the time of writing).

The minimum required version of secp256k1 will be 0.2.0 (released 2022-12-12), but newer releases include
new functionality. The config.m4 file should have build flags for each of these new modules, but ideally it
should be smart enough to set appropriate default values for each build flag depending on the version of
secp256k1 available.

| version | new module               |
|---------|--------------------------|
| v0.4.0  | secp256k1_ellswift       |
| v0.6.0  | secp256k1_musig          |
| v0.8.0  | secp256k1_silentpayments |

When the version of secp256k1 that we have available doesn't have one of these modules, the corresponding
PHP functions should simply not be registered (same approach as ext/gd, ext/openssl, etc.). Users check
availability with `function_exists()`. This avoids maintaining stubs for unavailable features. Each optional
module should have its own `.c` and `.stub.php` file, compiled conditionally via `config.m4` using
`PHP_CHECK_LIBRARY` to detect the presence of a specific symbol (e.g. `secp256k1_musig_nonce_gen`).

## Context management

A single global `secp256k1_context` is created and randomized at `PHP_MINIT` and destroyed at `PHP_MSHUTDOWN`.
The 32 bytes of entropy for `secp256k1_context_randomize` come from PHP's internal
`php_random_bytes()` (`ext/random/php_random.h`), which sources randomness from the OS kernel
(`getrandom()` on Linux). This is the same source behind userland `random_bytes()`.
This is safe under ZTS because all API functions that users call take `const secp256k1_context *`, which the
library guarantees is safe for concurrent use from multiple threads. Only `context_destroy`, `context_randomize`
and the `set_*_callback` functions take a non-const pointer and require exclusive access, and none of these
are called during normal operation after MINIT.

Alternatives considered and rejected:
- **Expose context to userland**: adds ceremony without real benefit for most users. The only use case would
  be different error callbacks per context, which is very marginal.
- **Create context per function call**: explicitly discouraged by libsecp256k1 docs ("Do not create a new
  context object for each operation, as construction and randomization can take non-negligible time").

## Opaque data structures

The internal opaque structures (`secp256k1_pubkey`, `secp256k1_ecdsa_signature`, etc.) are represented as
opaque PHP strings of their fixed size (e.g. 64 bytes for pubkey, 64 bytes for signature). Users don't
manipulate them directly — parse/serialize functions convert between external formats (33/65 bytes for
pubkeys, DER/compact for signatures) and the internal representation.

## Integration plan

### Phase 0+1 — Infrastructure + first function
- Set up global context (MINIT/MSHUTDOWN)
- Clean up skeleton (remove test1, test2)
- Include `<secp256k1.h>`
- Implement `secp256k1_ec_seckey_verify(string $seckey32): bool`

### Phase 2 — Public key generation and serialization
- `secp256k1_ec_pubkey_create`
- `secp256k1_ec_pubkey_parse`
- `secp256k1_ec_pubkey_serialize`

### Phase 3a — ECDSA signature object + parse/serialize
- Opaque class: `secp256k1_ecdsa_signature`
- `secp256k1_ecdsa_signature_parse_compact`
- `secp256k1_ecdsa_signature_parse_der`
- `secp256k1_ecdsa_signature_serialize_compact`
- `secp256k1_ecdsa_signature_serialize_der`

### Phase 3b — ECDSA sign + verify
- `secp256k1_ecdsa_sign`
- `secp256k1_ecdsa_verify`

### Phase 3c — ECDSA signature normalization
- `secp256k1_ecdsa_signature_normalize`

### Phase 4 — ECDSA Recovery
- `secp256k1_ecdsa_sign_recoverable`
- `secp256k1_ecdsa_recoverable_signature_parse_compact` / `_serialize_compact`
- `secp256k1_ecdsa_recoverable_signature_convert`
- `secp256k1_ecdsa_recover`

### Phase 5 — ECDH
- `secp256k1_ecdh`

### Phase 6 — Extrakeys + Schnorr (BIP-340)
- xonly pubkey: parse, serialize, from_pubkey, tweak_add, tweak_add_check, cmp
- keypair: create, pub, xonly_pub, sec, xonly_tweak_add
- schnorrsig: sign32, verify
- `secp256k1_tagged_sha256`

### Phase 7 — Auxiliary key operations
- `secp256k1_ec_seckey_negate`, `_tweak_add`, `_tweak_mul`
- `secp256k1_ec_pubkey_negate`, `_tweak_add`, `_tweak_mul`
- `secp256k1_ec_pubkey_combine`, `_sort`, `_cmp`
- `secp256k1_context_randomize`

### Phase 8+ — Optional modules (conditional compilation)
- **ellswift** (>= 0.4.0): encode, decode, create, xdh
- **musig** (>= 0.6.0): full MuSig2 API
- **silentpayments** (>= 0.8.0): full BIP-352 API

## Testing

Based on GitHub Actions with `shivammathur/setup-php`.
It should test NTS and ZTS PHP builds.
It should use Valgrind to detect memory leaks.
It should gather coverage stats of the C code with lcov and upload it to coveralls.io
