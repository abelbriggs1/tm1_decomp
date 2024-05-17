# Original Dockerfile taken from `decomp.me`'s `gcc2.6.0-mipsel` template.
# https://github.com/decompme/compilers/blob/main/platforms/ps1/gcc2.6.0-mipsel/Dockerfile

FROM alpine:latest

# Get the GCC 2.6.0 toolchain and MASPSX for building.
RUN mkdir -p /compilers/ps1/gcc2.6.0-mipsel

RUN wget -O gcc-2.6.0.tar.gz "https://github.com/decompals/old-gcc/releases/download/0.7/gcc-2.6.0.tar.gz"
RUN tar xvzf gcc-2.6.0.tar.gz -C /compilers/ps1/gcc2.6.0-mipsel

ARG MASPSX_HASH=d2d75a1686f397b549216f1f09d7b16265394281

RUN wget -O maspsx.zip https://github.com/mkst/maspsx/archive/${MASPSX_HASH}.zip
RUN unzip maspsx.zip
RUN cp -r maspsx-${MASPSX_HASH} /compilers/ps1/gcc2.6.0-mipsel/maspsx

RUN echo '#!/bin/bash' >> as
RUN echo 'python3 $(dirname -- $0)/maspsx/maspsx.py --run-assembler -I${COMPILER_DIR} $@' >> as
RUN cp as /compilers/ps1/gcc2.6.0-mipsel/

RUN chown -R root:root /compilers/ps1/gcc2.6.0-mipsel/
RUN chmod +x /compilers/ps1/gcc2.6.0-mipsel/*

# Add some extra dependencies for use in a devcontainer context.
RUN apk add --no-cache             \
    bash git make patch            \
    cmake python3 py3-pip mpc1-dev \
    mpfr freetype libpng ninja     \
    clang-extra-tools
