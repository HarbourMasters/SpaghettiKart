FROM ubuntu:22.04

# Make the automatic TARGETARCH variable available in this stage
ARG TARGETARCH

ENV DEBIAN_FRONTEND=noninteractive

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
RUN if [ "$TARGETARCH" = "amd64" ]; then \
    arch=x86_64; \
    elif [ "$TARGETARCH" = "arm64" ]; then \
    arch=aarch64; \
    fi && \
    wget https://github.com/Kitware/CMake/releases/download/v${DOCKER_CMAKE_VERSION}/cmake-${DOCKER_CMAKE_VERSION}-linux-$arch.sh && \
    chmod +x cmake-${DOCKER_CMAKE_VERSION}-linux-$arch.sh && \
    ./cmake-${DOCKER_CMAKE_VERSION}-linux-$arch.sh --prefix=/usr/local --skip-license && \
    rm cmake-${DOCKER_CMAKE_VERSION}-linux-$arch.sh

WORKDIR /project
