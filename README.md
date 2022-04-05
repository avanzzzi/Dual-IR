# Dual-IR

Dual-IR is a stereo impulse response loader with two output modes. Stereo enabled will output IR-A and IR-B on separate channels with pan controls. Stereo disabled will allow you to blend IR-A and IR-B into a mono output. Click the Load IR button to select a folder containing your impulse response files. The IR's will be available in both the IR-A and IR-B dropdown menus. IR's not included. 

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
