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

### NTS (Non-Thread-Safe)

A single global `secp256k1_context` is created and randomized at `PHP_MINIT` and destroyed at `PHP_MSHUTDOWN`.
The 32 bytes of entropy for `secp256k1_context_randomize` come from PHP's internal
`php_random_bytes()` (`ext/random/php_random.h`), which sources randomness from the OS kernel
(`getrandom()` on Linux). This is the same source behind userland `random_bytes()`.

Users can re-randomize the context at any time by calling `secp256k1_context_randomize()` (Phase 11).
libsecp256k1 recommends re-randomizing "before every few computations involving secret keys" as a
hardening measure against side-channel attacks. Under NTS this is safe because execution is sequential.

### ZTS (Thread-Safe)

Under ZTS, each thread gets its own `secp256k1_context` via `ZEND_BEGIN_MODULE_GLOBALS` / GINIT / GSHUTDOWN.
Each context is created and randomized at GINIT (thread startup) and destroyed at GSHUTDOWN. This avoids
any contention between threads: `secp256k1_context_randomize()` only touches the calling thread's context.

### Alternatives considered and rejected

- **Expose context to userland**: adds ceremony without real benefit for most users. The only use case would
  be different error callbacks per context, which is very marginal.
- **Create context per function call**: explicitly discouraged by libsecp256k1 docs ("Do not create a new
  context object for each operation, as construction and randomization can take non-negligible time").
- **Single global context under ZTS with locking**: `context_randomize` requires exclusive access,
  which would mean contention (pthread_rwlock or similar). php-src never uses pthread_rwlock internally,
  and per-thread contexts are the idiomatic Zend approach.

## Opaque data structures

The internal opaque structures (`secp256k1_pubkey`, `secp256k1_ecdsa_signature`, etc.) are represented as
final, non-serializable, non-cloneable PHP classes that wrap the C struct in a `zend_object`. Users cannot
instantiate them directly (`new secp256k1_pubkey()` throws) — only parse/create functions return instances.
Parse/serialize functions convert between external formats (33/65 bytes for pubkeys, DER/compact for
signatures) and the internal representation. The `free_obj` handler calls `explicit_bzero` on the C struct
to avoid leaving key material in freed memory.

## Implementation plan

### Phase 0+1 — Infrastructure + first function
- Set up global context (MINIT/MSHUTDOWN)
- Clean up skeleton (remove test1, test2)
- Include `<secp256k1.h>`
- Implement `secp256k1_ec_seckey_verify(string $seckey32): bool`

### Phase 2 — Public key generation and serialization
- `secp256k1_ec_pubkey_create`
- `secp256k1_ec_pubkey_parse`
- `secp256k1_ec_pubkey_serialize`

### Phase 3a — ECDSA signature object + parse/serialize/normalize
- Opaque class: `secp256k1_ecdsa_signature`
- `secp256k1_ecdsa_signature_parse_compact`
- `secp256k1_ecdsa_signature_parse_der`
- `secp256k1_ecdsa_signature_serialize_compact`
- `secp256k1_ecdsa_signature_serialize_der`
- `secp256k1_ecdsa_signature_normalize`

### Phase 3b — ECDSA sign + verify
- `secp256k1_ecdsa_sign`
- `secp256k1_ecdsa_verify`

### Phase 4 — Auxiliary key operations
- `secp256k1_ec_seckey_negate`, `_tweak_add`, `_tweak_mul`
- `secp256k1_ec_pubkey_negate`, `_tweak_add`, `_tweak_mul`
- `secp256k1_ec_pubkey_combine`, `_cmp`

### Optional modules (conditional compilation, each in its own .c)

All optional modules are detected at configure time with `PHP_CHECK_LIBRARY` and compiled
only when the symbol is present in the installed libsecp256k1. Users check availability
with `function_exists()`. Each optional module has its own `.c` file for the implementation,
but function declarations go in the single `secp256k1.stub.php` wrapped in
`#if defined(HAVE_SECP256K1_*)` guards. `gen_stub.php` propagates these guards to the
generated arginfo, so all functions end up in a single `ext_functions` array with
preprocessor conditionals — no need for separate `zend_register_functions` calls.

### Phase 5 — ECDH (optional, default ON)
- `secp256k1_ecdh.c`
- Detect symbol: `secp256k1_ecdh`
- `secp256k1_ecdh`

### Phase 6a — Extrakeys (optional, default ON)
- `secp256k1_extrakeys.c`
- Detect symbol: `secp256k1_xonly_pubkey_parse`
- Opaque classes: `secp256k1_xonly_pubkey`, `secp256k1_keypair`
- xonly pubkey: parse, serialize, from_pubkey, tweak_add, tweak_add_check, cmp
- keypair: create, pub, xonly_pub, sec, xonly_tweak_add

### Phase 6b — Schnorr signatures (optional, default ON, requires extrakeys)
- `secp256k1_schnorrsig.c`
- Detect symbol: `secp256k1_schnorrsig_sign32`
- schnorrsig: sign32, verify
- `secp256k1_tagged_sha256`

### Phase 7 — ECDSA Recovery (optional, default OFF in libsecp256k1)
- `secp256k1_recovery.c`
- Detect symbol: `secp256k1_ecdsa_sign_recoverable`
- Opaque class: `secp256k1_ecdsa_recoverable_signature`
- `secp256k1_ecdsa_sign_recoverable`
- `secp256k1_ecdsa_recoverable_signature_parse_compact` / `_serialize_compact`
- `secp256k1_ecdsa_recoverable_signature_convert`
- `secp256k1_ecdsa_recover`

### Phase 8 — MuSig2 (optional, >= 0.6.0, default ON)
- `secp256k1_musig.c`
- Detect symbol: `secp256k1_musig_nonce_gen`
- Opaque classes: `secp256k1_musig_keyagg_cache`, `secp256k1_musig_secnonce`, `secp256k1_musig_pubnonce`, `secp256k1_musig_aggnonce`, `secp256k1_musig_session`, `secp256k1_musig_partial_sig`
- Full MuSig2 API: nonce_gen, nonce_agg, pubkey_agg, process, partial_sign, partial_sig_verify, partial_sig_agg

### Phase 9 — Silent Payments (optional, >= 0.8.0, default ON)
- `secp256k1_silentpayments.c`
- Detect symbol: `secp256k1_silentpayments_recipient_create_label_tweak`
- Opaque classes: `secp256k1_silentpayments_recipient`, `secp256k1_silentpayments_label`, `secp256k1_silentpayments_prevouts_summary`, `secp256k1_silentpayments_found_output`
- Full BIP-352 API

### Phase 10 — Ellswift (optional, >= 0.4.0, default ON)
- `secp256k1_ellswift.c`
- Detect symbol: `secp256k1_ellswift_encode`
- encode, decode, create, xdh

### Phase 11 — Context re-randomization + ZTS module globals
- `secp256k1_context_randomize`
- Introduce `ZEND_BEGIN_MODULE_GLOBALS` / GINIT / GSHUTDOWN for per-thread contexts under ZTS
- All prior phases use `const secp256k1_context *` and work on both NTS and ZTS without this

## Testing

Based on GitHub Actions with `shivammathur/setup-php`.
It should test NTS and ZTS PHP builds.
It should use Valgrind to detect memory leaks.
It should gather coverage stats of the C code with lcov and upload it to coveralls.io
