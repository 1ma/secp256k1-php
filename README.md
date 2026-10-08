# secp256k1-php

[![Continuous Integration](https://github.com/1ma/secp256k1-php/actions/workflows/ci.yml/badge.svg?branch=master)](https://github.com/1ma/secp256k1-php/actions/workflows/ci.yml)
[![Coverage Status](https://coveralls.io/repos/github/1ma/secp256k1-php/badge.svg?branch=master)](https://coveralls.io/github/1ma/secp256k1-php?branch=master)

PHP bindings for https://github.com/bitcoin-core/secp256k1

This project seeks to reboot [Bit-Wasp's secp256k1-php](https://github.com/Bit-Wasp/secp256k1-php) to make this binding
available again for PHP 8.2 and above, as well as exposing the newer secp256k1 APIs of more recent versions.

It is not a fork of the old binding, rather a new implementation built from scratch
starting from the official extension skeleton given by PHP 8.5.

## Installation

The easiest way to install the extension is with [PIE](https://github.com/php/pie/):

```shell
$ pie check-build-tools
$ sudo apt install libsecp256k1-dev
$ sudo pie install uma/secp256k1-php
```

This should take care of building and enabling the extension in PHP on your behalf.
Verify with the `--ri` flag:

```shell
$ php --ri secp256k1

secp256k1

secp256k1 support => enabled
binding version => 0.1.2
libsecp256k1 version => 0.8.0
ecdh module => On
recovery module => On
extrakeys module => On
schnorrsig module => On
ellswift module => On
musig module => Not supported
silentpayments module => On
```

## Manual Build

If your system packages a version of libsecp256k1 older than `v0.2.0` or you want to use one of
the newer APIs you will need to build libsecp256k1 manually before you install the PHP extension.

```shell
$ sudo apt install build-essential cmake git
$ git clone --branch v0.8.0 https://github.com/bitcoin-core/secp256k1
$ cmake -S secp256k1 -B secp256k1/build -DSECP256K1_ENABLE_MODULE_RECOVERY=ON -DSECP256K1_BUILD_BENCHMARK=OFF -DSECP256K1_BUILD_CTIME_TESTS=OFF -DSECP256K1_BUILD_TESTS=OFF -DSECP256K1_BUILD_EXHAUSTIVE_TESTS=OFF
$ cmake --build secp256k1/build
$ sudo cmake --install secp256k1/build
$ rm -rf secp256k1
```

This will install the development headers and dynamic library of libsecp256k1 at `/usr/local`, and also produce an
`/usr/local/lib/pkgconfig/libsecp256k1.pc` pkg-config configuration file.

Now when you install the PHP extension as usual with PIE, under the hood pkg-config should find the libsecp256k1
installation at `/usr/local` and link the PHP extension against it instead of using the system's default package.

## Full API

See [secp256k1.stub.php](secp256k1.stub.php)

## F.A.Q.

### Are there stubs for the `secp256k1` functions?

Yes. Simply use Composer to install `uma/secp256k1-php` as a development dependency of your project:

```shell
$ composer require --dev uma/secp256k1-php
```

**NOTICE:** This isn't a substitute for the real installation described above.
This will just make the [secp256k1.stub.php](secp256k1.stub.php) file visible to your IDE so that it's aware of the extension functions.
But this *does not* install the extension, which is really the `secp256k1.so` binary shared library.

### What was the motivation for developing this extension?

Three years ago I wanted to use Bit-Wasp's [secp256k1-php](https://github.com/Bit-Wasp/secp256k1-php) binding, but
it was already abandoned at the time and I wrote [my own small extension](https://github.com/1ma/secp256k1-nostr-php/)
that also uses libsecp256k1 under the hood, but is focused on solving a very particular use case centered on Nostr.

Now with the help of LLMs and the experience gained with that previous work I decided to resurrect the full
secp256k1-php binding, which is still unmaintained as I write this in late 2026.

My motivation was not only to make the core ECDSA and Schnorr functionality available to modern PHP versions (>= 8.2),
but also to expose the new APIs that libsecp256k1 has gained over the years (`ellswift`, `musig`, `silentpayments`).

### Why is the `musig` module not supported?

The API of this module is remarkably complex and I don't fully understand it at the same level as the others.

I decided to put off the implementation for later releases, since the base module plus extrakeys+schnorrsig
is probably what 99% of users will care about.

If you have a use case for the musig API let me know in a GitHub issue.

All the other secp256k1 modules are supported, but only if they were enabled in libsecp256k1
when it was built or packaged for your distro.

### Does `secp256k1` follow [semantic versioning](https://semver.org/)?

Only once the binding reaches v1.0.0, if it ever does.

Pre v1.0.0 versions must be considered alpha releases and could break
backwards compatibility.

### Is Windows supported?

No, sorry Ballmer.

### Can `secp256k1` be installed with [PECL](https://pecl.php.net/)?

No, PECL is deprecated and no effort will be made to add this extension there.

### Has this codebase been vibecoded?

Claude Opus 4.6 has written 95% to 99% of the codebase, but my way of using AI involves planning the
work in human-digestible chunks and not let it go on until I fully approve and understand each chunk.
Each chunk of work usually involves asking questions about the secp256k1 APIs, about the Zend Engine,
about cryptography, finding bugs in the logic, finding gaps in the test suite, discussing potential
refactors, etc.

This forces the agent to work much slower than it could, but on the other hand I end up building exactly
what I want while I catch a lot of errors and learn new things along the way.

I feel confident enough about my understanding of this binding to claim full ownership of the code as if
I had written it myself.
