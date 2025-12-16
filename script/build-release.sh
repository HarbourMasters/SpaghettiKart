#!/bin/bash
set -e

# Script to build SpaghettiKart using Docker
# Usage: ./docker/build.sh [Release|Debug]

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_DIR="$(dirname "$SCRIPT_DIR")"

BUILD_TYPE="${1:-Release}"
BUILD_DIR="build-docker"
IMAGE_NAME="spaghettikart-builder"
USER_ID=$(id -u)
GROUP_ID=$(id -g)

cd "$PROJECT_DIR"

# Build Docker image
echo "Building Docker image..."
docker build -t "${IMAGE_NAME}" -f script/Dockerfile .

# Clean build directory if it contains incompatible cache
if [[ -f "${BUILD_DIR}/CMakeCache.txt" ]] && grep -q "/home/coco" "${BUILD_DIR}/CMakeCache.txt" 2>/dev/null; then
    echo "Cleaning incompatible build cache..."
    rm -rf "${BUILD_DIR}"
fi

# Run build in Docker
echo "Building in Docker container..."
docker run --rm \
    -v "$(pwd):/project" \
    -e BUILD_TYPE="${BUILD_TYPE}" \
    -e USER_ID="${USER_ID}" \
    -e GROUP_ID="${GROUP_ID}" \
    "${IMAGE_NAME}" \
    bash -c "
        # Copy vcpkg.json and install dependencies
        cp vcpkg.json /tmp/vcpkg.json && \
        cd /tmp && \
        \${VCPKG_ROOT}/vcpkg install --triplet x64-linux && \
        cd /project && \
        cmake -B ${BUILD_DIR} -G Ninja \
            -DCMAKE_BUILD_TYPE=${BUILD_TYPE} \
            -DCMAKE_TOOLCHAIN_FILE=/opt/vcpkg/scripts/buildsystems/vcpkg.cmake \
            -DVCPKG_TARGET_TRIPLET=x64-linux && \
        cmake --build ${BUILD_DIR} --parallel && \
        cp -f spaghetti.o2r ${BUILD_DIR}/ 2>/dev/null || true && \
        cp -f mk64.o2r ${BUILD_DIR}/ 2>/dev/null || true && \
        cd ${BUILD_DIR} && cpack -G External && \
        chown -R ${USER_ID}:${GROUP_ID} /project/${BUILD_DIR} /project/logs 2>/dev/null || true
    "

echo ""
echo "Build complete!"
echo "Executable: ${BUILD_DIR}/Spaghettify"
ls -1 "${BUILD_DIR}"/*.appimage 2>/dev/null && echo "AppImage generated successfully!" || echo "Note: AppImage generation may have failed"
