@page compiling Installing mk64 Decomp
# Build Instructions
The build system has the following package requirements:

    binutils-mips >= 2.27
    python3 >= 3.6
    libaudiofile

[TOC]

To add submodules run `git submodule update --init --recursive` after cloning.

Run `make assets` to extract assets.

# Linux

### Ubuntu

```bash
sudo apt install cmake build-essential pkg-config git binutils-mips-linux-gnu python3 zlib1g-dev libaudiofile-dev libcapstone-dev
```

### Fedora

```bash
sudo dnf install make automake gcc gcc-c++ kernel-devel cmake pkg-config git python3 zlib-devel audiofile-devel capstone
```
and install [gcc toolchain of libdragon](https://github.com/DragonMinded/libdragon/releases/tag/toolchain-continuous-prerelease) and add `export PATH="/opt/libdragon/bin:$PATH"` to ~/.bashrc

### Arch

```bash
sudo pacman -S base-devel capstone python
```
Install the following AUR package:
* [mips64-elf-binutils](https://aur.archlinux.org/packages/mips64-elf-binutils) (AUR)

Review the [n64decomp/sm64](https://github.com/n64decomp/sm64) readme for instructions to compile in other distributions.

# Windows

- Clone the repo or download the zip.
- Download and extract the toolchain from [here](https://github.com/coco875/mk64-tools/releases/download/v0.0.8/mips-tools-chain-windows.zip)

### Setup
- Place the `mingw64` folder from the toolchain into the tools folder like so: `mk64/tools/mingw64`.
- Open a terminal (cmd or powershell) in the repo folder and run `"tools\mingw64\w64devkit.exe"` and after `make assets && make -j`
- Wait for the build to finish and Enjoy!

# macOS

Install [Homebrew](https://brew.sh), then install the following dependencies:
```bash
brew update
brew install python3 capstone coreutils make pkg-config tehzz/n64-dev/mips64-elf-binutils
```

Build using `gmake` ensuring homebrew `make` is used instead of the old macOS system `make`.

# Docker

Build the Docker image:
```bash
docker build -t mk64 .
```

When building and using other tools, append the following in front of every command you run:
```bash
docker run --rm -v .:/mk64 mk64
```

For example:
```bash
docker run --rm -v .:/mk64 mk64 make
```

# Building US

Place a US version of Mario Kart 64 called `baserom.us.z64` into the project folder for asset extraction.

Run the following commands after pulling:
```bash
make -j
```

# Building EU

Building EU requires US to be built first. See above.

mk64 decomp supports two EU versions
* EU 1.0 `eu-1.0`
* EU 1.1 `eu-final`

Build using
```bash
make -j VERSION=eu-1.0
```
or
```bash
make -j VERSION=eu-final
```

First-diff/diff commands for EU
```bash
python3 first-diff.py --eu
./diff <function> -eu
```