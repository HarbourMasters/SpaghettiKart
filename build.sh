#!/bin/sh

# Init git submodules
git config --global --add safe.directory /project
git submodule update --init

# Build spaghetti.o2r
cmake -H. -Bbuild-cmake -GNinja -DCMAKE_BUILD_TYPE=Release
cmake --build build-cmake --target GenerateO2R -j$(nproc)

# Build spaghetti executable
cmake --build build-cmake --config Release -j$(nproc)

# Pack AppImage
cd build-cmake
cpack -G External
cd ..
