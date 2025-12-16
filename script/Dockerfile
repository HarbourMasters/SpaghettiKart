FROM ubuntu:22.04

ENV DEBIAN_FRONTEND=noninteractive
ENV VCPKG_ROOT=/opt/vcpkg
ENV PATH="${VCPKG_ROOT}:${PATH}"

# Install base dependencies
RUN apt-get update -y && apt-get -y upgrade && \
    apt-get -y install \
    build-essential \
    gcc \
    g++ \
    git \
    ninja-build \
    curl \
    zip \
    unzip \
    tar \
    pkg-config \
    wget \
    file \
    autoconf \
    automake \
    libtool \
    python3 \
    libx11-dev \
    libxrandr-dev \
    libxi-dev \
    libxinerama-dev \
    libxcursor-dev \
    libgl1-mesa-dev \
    libopengl-dev \
    libwayland-dev \
    libxkbcommon-dev \
    libasound2-dev \
    libpulse-dev \
    libudev-dev \
    libdbus-1-dev

# Install CMake 4.0.3
ENV DOCKER_CMAKE_VERSION="4.0.3"
RUN wget https://github.com/Kitware/CMake/releases/download/v${DOCKER_CMAKE_VERSION}/cmake-${DOCKER_CMAKE_VERSION}-linux-x86_64.sh && \
    chmod +x cmake-${DOCKER_CMAKE_VERSION}-linux-x86_64.sh && \
    ./cmake-${DOCKER_CMAKE_VERSION}-linux-x86_64.sh --prefix=/usr/local --skip-license && \
    rm cmake-${DOCKER_CMAKE_VERSION}-linux-x86_64.sh

# Install vcpkg
RUN git clone https://github.com/microsoft/vcpkg.git ${VCPKG_ROOT} && \
    cd ${VCPKG_ROOT} && \
    ./bootstrap-vcpkg.sh -disableMetrics

WORKDIR /project
