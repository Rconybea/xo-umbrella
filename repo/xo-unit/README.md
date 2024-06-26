# unit library

Provides compile-time dimension checking and scaling.

Similar to `boost::units`,  but:
1. streamlined: assumes modern (c++20) support
2. supports fractional dimensions (rational powers)

## Getting Started

### build + install dependencies

- [github/Rconybea/reflect](https://github.com/Rconybea/reflect)

### build + install
```
$ cd xo-unit
$ mkdir .build
$ cd .build
$ PREFIX=/usr/local  # or wherever you prefer
$ cmake -DCMAKE_MODULE_PATH=${PREFIX}/share/cmake -DCMAKE_PREFIX_PATH=${PREFIX} -DCMAKE_INSTALL_PREFIX=${PREFIX} ..
$ make
$ make install
```

### build for unit test coverage
```
$ cd xo-unit
$ mkdir .build-ccov
$ cd .build-ccov
$ cmake -DCMAKE_MODULE_PATH=${INSTALL_PREFIX}/share/cmake  -DCMAKE_PREFIX_PATH=${INSTALL_PREFIX} -DCODE_COVERAGE=ON -DCMAKE_BUILD_TYPE=Debug ..
```

### LSP support
```
$ cd xo-unit
$ ln -s .build/compile_commands.json  # lsp will look for compile_commands.json in the root of the source tree
```
