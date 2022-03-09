# Dual-IR

[![CI](https://github.com/GuitarML/Chameleon/actions/workflows/cmake.yml/badge.svg)](https://github.com/GuitarML/Chameleon/actions/workflows/cmake.yml)

### Build with Cmake

```bash
# Clone the repository
$ git clone https://github.com/GuitarML/Dual-IR.git
$ cd Dual-IR

# initialize and set up submodules
$ git submodule update --init --recursive

# build with CMake
$ cmake -Bbuild
$ cmake --build build --config Release
```
The binaries will be located in `Dual-IR/build/Dual-IR_artefacts/`
