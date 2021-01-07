FROM ubuntu:18.04

RUN apt-get update && apt-get install -y \
    cmake \
    libgtest-dev \
    git \
    pkg-config \
    googletest \
    google-mock \
    libevent-dev \
    liblog4cplus-dev \
    nlohmann-json-dev

# Protobuf
RUN apt-get install autoconf automake libtool curl make g++ unzip -y && \
    git clone https://github.com/google/protobuf.git && \
    cd protobuf && \
    git checkout v3.14.0 && \
    git submodule update --init --recursive && \
    ./autogen.sh && \
    ./configure && \
    make && \
    make install && \
    ldconfig

# Compile and install embDB
RUN git clone https://github.com/flovo89/embDB.git && \
    cd /embDB && \
    mkdir build && \
    cd build && \
    cmake .. && \
    make && \
    make install