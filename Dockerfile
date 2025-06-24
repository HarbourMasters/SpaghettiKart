FROM ubuntu:latest

# Install dependencies
RUN apt-get update -y && apt-get -y upgrade && apt-get -y install gcc g++ git cmake ninja-build lsb-release libsdl2-dev libsdl2-net-dev libpng-dev libzip-dev zipcmp zipmerge ziptool nlohmann-json3-dev libtinyxml2-dev libspdlog-dev libboost-dev libopengl-dev libogg-dev libvorbis-dev wget file

# Install latest SDL
RUN wget https://www.libsdl.org/release/SDL2-2.30.3.tar.gz && \
    tar -xzf SDL2-2.30.3.tar.gz && \
    cd SDL2-2.30.3 && \
    ./configure --enable-hidapi-libusb && \
    make -j$(nproc) && \
    make install && \
    cp -av /usr/local/lib/libSDL* /lib/x86_64-linux-gnu/ && \
    cd .. && \
    rm -rf SDL2-2.30.3.tar.gz SDL2-2.30.3

# Install latest SDL_net
RUN wget https://www.libsdl.org/projects/SDL_net/release/SDL2_net-2.2.0.tar.gz && \
    tar -xzf SDL2_net-2.2.0.tar.gz && \
    cd SDL2_net-2.2.0 && \
    ./configure && \
    make -j$(nproc) && \
    make install && \
    cp -av /usr/local/lib/libSDL* /lib/x86_64-linux-gnu/ && \
    cd .. && \
    rm -rf SDL2_net-2.2.0.tar.gz SDL2_net-2.2.0

# Install latest tinyxml2
RUN wget https://github.com/leethomason/tinyxml2/archive/refs/tags/10.0.0.tar.gz && \
    tar -xzf 10.0.0.tar.gz && \
    cd tinyxml2-10.0.0 && \
    mkdir -p build && \
    cd build && \
    cmake .. && \
    make -j$(nproc) && \
    make install && \
    cd ../.. && \
    rm -rf 10.0.0.tar.gz tinyxml2-10.0.0

# Install libzip without crypto
RUN wget https://github.com/nih-at/libzip/releases/download/v1.10.1/libzip-1.10.1.tar.gz && \
    tar -xzf libzip-1.10.1.tar.gz && \
    cd libzip-1.10.1 && \
    mkdir -p build && \
    cd build && \
    cmake .. -DENABLE_COMMONCRYPTO=OFF -DENABLE_GNUTLS=OFF -DENABLE_MBEDTLS=OFF -DENABLE_OPENSSL=OFF && \
    make -j$(nproc) && \
    make install & \
#    cp -av /usr/local/lib/libzip* /lib/x86_64-linux-gnu/ && \
    cd ../.. && \
    rm -rf libzip-1.10.1.tar.gz libzip-1.10.1

WORKDIR /project
