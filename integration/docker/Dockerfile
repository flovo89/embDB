FROM ubuntu:18.04

LABEL version="1.0"
LABEL description="Container used to compile and run linux application unit-tests"
LABEL maintainer="flovo89"

# Avoid that install package stalls on 
# asking for timezone in configure tzdata
ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update && apt-get install -y \
    cmake \
    build-essential \
    curl \
    git \
    pkg-config \
    libevent-dev \
    liblog4cplus-dev 

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

# Install googletest version 1.10.0
RUN curl -OL https://github.com/google/googletest/archive/release-1.10.0.tar.gz && \
    tar xzf release-1.10.0.tar.gz && \
    cd googletest-release-1.10.0 && \
    mkdir build && cd build && \
    cmake .. && \
    make install

# Nlohmann json
RUN git clone https://github.com/nlohmann/json.git && \
    cd json && \
    git checkout v3.9.0 && \
    mkdir build && \
    cd build && \ 
    cmake .. && \
    make && \
    make install

# Compile and install embDB
RUN git clone https://github.com/flovo89/embDB.git && \
    cd /embDB && \
    git checkout linear_db && \
    mkdir build && \
    cd build && \
    cmake .. && \
    make && \
    make install

# Start embDB
EXPOSE 5678
CMD embdb --tcpport 5678 --file /embdb.database