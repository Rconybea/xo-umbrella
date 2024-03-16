# xo projects

A set of libraries for complex event processing

## Features

- native c++
- deterministic simulation
- reflection
- python bindings

## Getting Started

```
$ git clone https://github.com/rconybea/xo-umbrella.git xo
```

### build + install

with clone in `.xo/`:
```
$ mkdir xo/.build
$ PREFIX=/usr/local # for example
$ cmake -B xo/.build -DCMAKE_INSTALL_PREFIX=$PREFIX -DCMAKE_BUILD_TYPE=debug
$ cmake --build xo/.build -j
$ cmake --install xo/.build
```
