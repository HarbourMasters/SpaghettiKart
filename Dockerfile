FROM ubuntu:24.04 as build

RUN apt-get update && \
    apt-get upgrade -y && \
    apt-get install -y \
        binutils-mips-linux-gnu \
        build-essential \
        cmake \
        git \
        libaudiofile-dev \
        libcapstone-dev \
        pkg-config \
        python3 \
        zlib1g-dev

RUN mkdir /mk64
WORKDIR /mk64

CMD echo 'Usage: docker run --rm -v ${PWD}:/mk64 mk64 make\n'\
         'See https://github.com/n64decomp/mk64/blob/master/README.md for more information'
