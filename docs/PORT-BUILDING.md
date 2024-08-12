If you have some problem to get it setup you can refer to [https://github.com/HarbourMasters/Shipwright/blob/develop/docs/BUILDING.md](https://github.com/HarbourMasters/Shipwright/blob/develop/docs/BUILDING.md)

# Contribute

## Github
1) Fork
2) Create a new branch
3) `git clone --branch <branch_name> <fork_url>`

4) Place baserom.us.z64 in root.
5) `git submodule update --init`
6) copy from mk64 assets/course in assets 

# Windows

## Setup
```bat
& 'C:\Program Files\CMake\bin\cmake' -S . -B "build/x64" -G "Visual Studio 17 2022" -T v143 -A x64
```

## Extract Assets
```bat
& 'C:\Program Files\CMake\bin\cmake' --build "build/x64" --target ExtractAssets
```

## Compile
```bat
& 'C:\Program Files\CMake\bin\cmake.exe' --build .\build\x64
```

## Clean
```bat
& 'C:\Program Files\CMake\bin\cmake.exe' --build .\build\x64 --target clean
```

# Linux
## Install dependencies
### Debian/Ubuntu
(not tried)
```sh
# using gcc
apt-get install gcc g++ git cmake ninja-build lsb-release libsdl2-dev libpng-dev libsdl2-net-dev libzip-dev zipcmp zipmerge ziptool nlohmann-json3-dev libtinyxml2-dev libspdlog-dev libboost-dev libopengl-dev

# or using clang
apt-get install clang git cmake ninja-build lsb-release libsdl2-dev libpng-dev libsdl2-net-dev libzip-dev zipcmp zipmerge ziptool nlohmann-json3-dev libtinyxml2-dev libspdlog-dev libboost-dev libopengl-dev
```
### Arch
(not tried)
```sh
# using gcc
pacman -S gcc git cmake ninja lsb-release sdl2 libpng libzip nlohmann-json tinyxml2 spdlog sdl2_net boost

# or using clang
pacman -S clang git cmake ninja lsb-release sdl2 libpng libzip nlohmann-json tinyxml2 spdlog sdl2_net boost
```
### Fedora
```sh
# using gcc
dnf install gcc gcc-c++ git cmake ninja-build lsb_release SDL2-devel libpng-devel libzip-devel libzip-tools json-devel tinyxml2-devel spdlog-devel boost-devel SDL2_net-devel

# or using clang
dnf install clang git cmake ninja-build lsb_release SDL2-devel libpng-devel libzip-devel libzip-tools json-devel tinyxml2-devel spdlog-devel boost-devel SDL2_net-devel
```
### openSUSE
(not tried)
```sh
# using gcc
zypper in gcc gcc-c++ git cmake ninja SDL2-devel libpng16-devel libzip-devel libzip-tools nlohmann_json-devel tinyxml2-devel spdlog-devel

# or using clang
zypper in clang libstdc++-devel git cmake ninja SDL2-devel libpng16-devel libzip-devel libzip-tools nlohmann_json-devel tinyxml2-devel spdlog-devel
```

## Setup
```bash
cmake -H. -Bbuild-cmake -GNinja
```

## Extract Assets
```bash
cmake --build build-cmake --target ExtractAssets
```

## Compile
```bash
cmake --build build-cmake
```

## Clean
```bash
cmake --build build-cmake --target clean
```