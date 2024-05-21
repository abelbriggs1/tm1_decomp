# Original Dockerfile taken from `decomp.me`'s `gcc2.7.0-mipsel` template.
# https://github.com/decompme/compilers/blob/main/platforms/ps1/gcc2.7.0-mipsel/Dockerfile

FROM debian:bookworm

# Add some extra dependencies for use in a devcontainer context.
RUN apt-get update
RUN apt-get install -y \
    gcc-mipsel-linux-gnu                   \
    binutils-mipsel-linux-gnu              \
    clang-format                           \
    python3 python3-pip python3-virtualenv \
    nano                                   \
    curl wget                              \
    unzip                                  \
    git make patch                         \
    cmake ninja-build                      \
    && rm -rf /var/lib/apt/lists/*

# Get the GCC 2.7.2 toolchain and MASPSX for building.
RUN mkdir -p /compilers/ps1/gcc2.7.2-mipsel

RUN wget -O gcc-2.7.2.tar.gz "https://github.com/decompals/old-gcc/releases/download/0.7/gcc-2.7.2.tar.gz"
RUN tar xvzf gcc-2.7.2.tar.gz -C /compilers/ps1/gcc2.7.2-mipsel

ARG MASPSX_HASH=d2d75a1686f397b549216f1f09d7b16265394281

RUN wget -O maspsx.zip https://github.com/mkst/maspsx/archive/${MASPSX_HASH}.zip
RUN unzip maspsx.zip
RUN cp -r maspsx-${MASPSX_HASH} /compilers/ps1/gcc2.7.2-mipsel/maspsx

RUN echo '#!/bin/bash' >> as
RUN echo 'python3 $(dirname -- $0)/maspsx/maspsx.py --run-assembler --aspsx-version=2.34 --gnu-as-path=mipsel-linux-gnu-as -I${COMPILER_DIR} -- $@' >> as
RUN cp as /compilers/ps1/gcc2.7.2-mipsel/

RUN chown -R root:root /compilers/ps1/gcc2.7.2-mipsel/
RUN chmod +x /compilers/ps1/gcc2.7.2-mipsel/*

# Add a default non-root user.
ARG USERNAME=vscode
ARG USER_UID=1000
ARG USER_GID=$USER_UID

RUN groupadd --gid $USER_GID $USERNAME \
    && useradd --uid $USER_UID --gid $USER_GID -m $USERNAME

USER $USERNAME
