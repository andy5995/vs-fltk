FROM debian:trixie-slim

ARG DEBIAN_FRONTEND=noninteractive
ENV DEBIAN_FRONTEND=$DEBIAN_FRONTEND

RUN \
    apt update && apt upgrade -y && \
    apt install -y \
        build-essential \
        ca-certificates \
        clang-19 \
        cmake \
        curl \
        freeglut3-dev \
        git \
        libpng-dev \
        libsqlite3-0 \
        libsqlitecpp-dev \
        libtcc-dev \
        lsb-release \
        meson \
        pkg-config \
        sudo \
        swiftlang \
        unzip \
        wget && \
    update-ca-certificates -f

RUN useradd -m builder && passwd -d builder
RUN echo "builder ALL=(ALL) ALL" >> /etc/sudoers
WORKDIR /home/builder
USER builder

RUN curl -fsSL https://bun.sh/install | bash
RUN \
    echo "export BUN_INSTALL=\"\$HOME/.bun\"" >> $HOME/.profile && \
    echo "export PATH=\"\$BUN_INSTALL/bin:\$PATH\"" >> $HOME/.profile

USER root
WORKDIR /
ADD entrypoint.sh /entrypoint.sh
ENTRYPOINT ["/entrypoint.sh"]
