# optionutil library

low-level data structures useful when working with equity options

## Getting Started

### build + install dependencies

build + install these first

- xo-printjson [github.com/rconybea/xo-printjson](https://github.com/rconybea/xo-printjson)

### build + install xo-optionutil
```
$ cd xo-optionutil
$ mkdir .build
$ PREFIX=/usr/local  # or wherever you prefer
$ cmake -DCMAKE_INSTALL_PREFIX=$PREFIX -B .build
$ cmake --build .build
$ cmake --install .build
```
(also see .github/workflows/main.yml)

## Development

### build for unit test coverage
```
$ cd xo-optionutil
$ mkdir .build-ccov
$ cmake -DCMAKE_INSTALL_PREFIX=$PREFIX -DCMAKE_PREFIX_PATH=$PREFIX -DCODE_COVERAGE=ON -DCMAKE_BUILD_TYPE=Debug -B .build-ccov
```

### LSP support

LSP looks for compile commands in the root of the source tree;
cmake creates them in the root of its build directory.

```
$ cd xo-optionutil
$ ln -s .build/compile_commands.json
```

### display cmake variables

- `-L` list variables
- `-A` include 'advanced' variables
- `-H` include help text

```
$ cd xo-optionutil/.build
$ cmake -LAH
```
