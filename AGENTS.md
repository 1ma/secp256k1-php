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

When the version of secp256k1 that we have available doesn't have one of these modules I'm not sure if
we don't want to even expose a userland function, or the userland functions must always exist but throw
an exception if they are missing the underlying implementation. We should investigate what other bindings do.

## Testing

Based on GitHub Actions with `shivammathur/setup-php`.
It should test NTS and ZTS PHP builds.
It should use Valgrind to detect memory leaks.
It should gather coverage stats of the C code with lcov and upload it to coveralls.io
