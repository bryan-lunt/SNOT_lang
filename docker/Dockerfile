FROM ubuntu:24.04 AS snot_build
LABEL maintainer="bryan.j.lunt+snot_lang@gmail.com"

RUN apt-get update -y -q && \
    apt-get install -y curl && \
    apt-get install -y autoconf automake libtool pkg-config && \
    apt-get install -y libtool libtool-bin libltdl7 && \
    apt-get install -y gcc-13 g++-13 && \
    apt-get install -y cmake && \
    apt-get install -y build-essential && \
    rm -rf /var/lib/apt/lists/*

RUN apt-get update -y -q && \
    apt-get install -y git vim && \
    apt-get install -y flex bison && \
    rm -rf /var/lib/apt/lists/*

    RUN apt-get update -y -q && \
        apt-get install -y gdb && \
        rm -rf /var/lib/apt/lists/*

WORKDIR /workspace
