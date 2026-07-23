#!/bin/bash
set -euo pipefail

# Unified build script for SpaghettiKart
#
# Usage:
#   ./script/build.sh                    # Build x64 Release (default)
#   ./script/build.sh x64                # Build x64 Release
#   ./script/build.sh x86                # Build x86 32-bit Release
#   ./script/build.sh x64 Debug          # Build x64 Debug
#   ./script/build.sh all                # Build both architectures
#   ./script/build.sh x64 Release appimage  # Build x64 with AppImage

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_DIR="$(dirname "$SCRIPT_DIR")"

ARCH="${1:-x64}"
BUILD_TYPE="${2:-Release}"
PACKAGE="${3:-}"  # Optional: "appimage" to generate AppImage
USER_ID=$(id -u)
GROUP_ID=$(id -g)

cd "$PROJECT_DIR"

case "$BUILD_TYPE" in
    Debug|Release|RelWithDebInfo|MinSizeRel) ;;
    *)
        echo "Unsupported build type: ${BUILD_TYPE}" >&2
        exit 1
        ;;
esac

if [[ -n "$PACKAGE" && "$PACKAGE" != "appimage" ]]; then
    echo "Unsupported package type: ${PACKAGE}" >&2
    exit 1
fi

build_arch() {
    local arch=$1
    local build_type=$2
    local package=$3
    local build_dir="build-docker-${arch}"
    local image_name="spaghettikart-${arch}"
    local dockerfile=""
    local platform=""
    
    echo "========================================"
    echo "Building ${arch} ${build_type}"
    echo "========================================"
    
    # Set architecture-specific options
    if [ "$arch" = "x86" ]; then
        dockerfile="script/Dockerfile.x86"
        platform="--platform linux/386"
        # Ensure QEMU is set up for i386 emulation
        docker run --rm --privileged multiarch/qemu-user-static --reset -p yes 2>/dev/null || true
    else
        dockerfile="script/Dockerfile"
    fi
    
    # Build Docker image
    echo "Building Docker image for ${arch}..."
    docker build \
        ${platform} \
        -t "${image_name}" \
        -f "${dockerfile}" .
    
    # Clean build directory if it contains incompatible cache
    if [[ -f "${build_dir}/CMakeCache.txt" ]]; then
        echo "Cleaning existing build cache..."
        rm -rf "${build_dir}"
    fi
    
    # Build package command if requested
    local package_cmd=":"
    if [ "$package" = "appimage" ]; then
        package_cmd="cd ${build_dir} && cpack -G External"
    fi
    
    # Run build in Docker
    echo "Building in Docker container..."
    
    # Different build commands for x64 (with vcpkg) and x86 (without vcpkg)
    if [ "$arch" = "x86" ]; then
        # x86: No vcpkg available, use system packages
        docker run --rm \
            ${platform} \
            -v "$(pwd):/project" \
            -e BUILD_TYPE="${build_type}" \
            -e USER_ID="${USER_ID}" \
            -e GROUP_ID="${GROUP_ID}" \
            "${image_name}" \
            bash -c "
                set -e
                cmake -B ${build_dir} -G Ninja \
                    -DCMAKE_BUILD_TYPE=${build_type} && \
                cmake --build ${build_dir} --parallel && \
                (cp -f spaghetti.o2r ${build_dir}/ 2>/dev/null || true) && \
                (cp -f mk64.o2r ${build_dir}/ 2>/dev/null || true) && \
                ${package_cmd} && \
                (chown -R ${USER_ID}:${GROUP_ID} /project/${build_dir} 2>/dev/null || true)
            "
    else
        # x64: Let the CMake integration bootstrap and configure vcpkg.
        docker run --rm \
            ${platform} \
            -v "$(pwd):/project" \
            -e BUILD_TYPE="${build_type}" \
            -e USER_ID="${USER_ID}" \
            -e GROUP_ID="${GROUP_ID}" \
            "${image_name}" \
            bash -c "
                set -e
                cmake -B ${build_dir} -G Ninja \
                    -DCMAKE_BUILD_TYPE=${build_type} \
                    -DENABLE_VCPKG=ON && \
                cmake --build ${build_dir} --parallel && \
                (cp -f spaghetti.o2r ${build_dir}/ 2>/dev/null || true) && \
                (cp -f mk64.o2r ${build_dir}/ 2>/dev/null || true) && \
                ${package_cmd} && \
                (chown -R ${USER_ID}:${GROUP_ID} /project/${build_dir} 2>/dev/null || true)
            "
    fi
    
    echo ""
    echo "${arch} build complete!"
    echo "Executable: ${build_dir}/Spaghettify"
    if [ -f "${build_dir}/Spaghettify" ]; then
        file "${build_dir}/Spaghettify"
    fi
    if [ "$package" = "appimage" ]; then
        ls -1 "${build_dir}"/*.appimage 2>/dev/null && echo "AppImage generated!" || echo "Note: AppImage may have failed"
    fi
    echo ""
}

case "$ARCH" in
    x64|x86)
        build_arch "$ARCH" "$BUILD_TYPE" "$PACKAGE"
        ;;
    all)
        build_arch x64 "$BUILD_TYPE" "$PACKAGE"
        build_arch x86 "$BUILD_TYPE" "$PACKAGE"
        ;;
    *)
        echo "Usage: $0 [x64|x86|all] [Release|Debug] [appimage]"
        echo ""
        echo "Architectures:"
        echo "  x64      - x86_64 64-bit (default)"
        echo "  x86      - x86 32-bit"
        echo "  all      - Build both architectures"
        echo ""
        echo "Options:"
        echo "  appimage - Generate AppImage package"
        echo ""
        echo "Examples:"
        echo "  $0                      # Build x64 Release"
        echo "  $0 x86                  # Build x86 Release"
        echo "  $0 x64 Release appimage # Build x64 with AppImage"
        exit 1
        ;;
esac

echo "========================================"
echo "All builds completed successfully!"
echo "========================================"
