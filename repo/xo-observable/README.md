# observable library

describe observations with outcomes,  and corresponding outcome spaces.

## Getting Started

### build + install dependencies

See `.github/workflows/main.yml` in this repo for example build+install on ubuntu

### build + install xo-observable
```
$ cd xo-observable
$ mkdir .build
$ INSTALL_PREFIX=/usr/local  # or wherever you prefer
$ cmake -B .build -DCMAKE_PREFIX_PATH=${INSTALL_PREFIX} -DCMAKE_INSTALL_PREFIX=${INSTALL_PREFIX}
$ cmake --build .build -j
$ cmake --install .build
```
(also see .github/workflows/main.yml)

### build for unit test coverage
```
$ cd xo-observable
$ mkdir ccov
$ cd ccov
$ cmake -DCMAKE_MODULE_PATH=${INSTALL_PREFIX}/share/cmake  -DCMAKE_PREFIX_PATH=${INSTALL_PREFIX} -DCODE_COVERAGE=ON -DCMAKE_BUILD_TYPE=Debug ..
```

## Development

### LSP support

LSP looks for compile commands in the root of the source tree;
cmake creates them in the root of its build directory.

```
$ cd xo-observable
$ ln -s .build/compile_commands.json
```

### display cmake variables

- `-L` list variables
- `-A` include 'advanced' variables
- `-H` include help text

```
$ cd xo-observable/.build
$ cmake -LAH
```
