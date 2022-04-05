# Dual-IR

[![CI](https://github.com/GuitarML/Dual-IR/actions/workflows/cmake.yml/badge.svg)](https://github.com/GuitarML/Dual-IR/actions/workflows/cmake.yml)[![License: GPL v3](https://img.shields.io/badge/License-GPLv3-brightgreen.svg)](https://www.gnu.org/licenses/gpl-3.0) [![Downloads](https://img.shields.io/github/downloads/GuitarML/Dual-IR/total)](https://somsubhra.github.io/github-release-stats/?username=GuitarML&repository=Dual-IR&page=1&per_page=30)

Dual-IR is a stereo impulse response loader with two output modes. Stereo enabled will output IR-A and IR-B on separate channels with pan controls. Stereo disabled will allow you to blend IR-A and IR-B into a mono output. Click the Load IR button to select a folder containing your impulse response files. The IR's will be available in both the IR-A and IR-B dropdown menus. IR's not included. 


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
