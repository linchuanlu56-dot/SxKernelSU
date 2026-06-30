FROM ubuntu:24.04 AS dev

LABEL description="SxKernelSU Development Environment"

ENV DEBIAN_FRONTEND=noninteractive
ENV RUSTUP_HOME=/usr/local/rustup
ENV CARGO_HOME=/usr/local/cargo
ENV PATH=$CARGO_HOME/bin:$PATH

# Install system dependencies
RUN apt-get update && apt-get install -y --no-install-recommends \
    build-essential \
    clang \
    cmake \
    curl \
    file \
    flex \
    g++ \
    gcc \
    git \
    libssl-dev \
    llvm \
    make \
    ninja-build \
    openssl \
    pkg-config \
    python3 \
    zlib1g-dev \
    && rm -rf /var/lib/apt/lists/*

# Install Rust
RUN curl --proto '=https' --tlsv1.2 -sSf https://sh.rustup.rs | sh -s -- -y \
    && rustup update stable \
    && rustup target add aarch64-linux-android \
    && rustup target add x86_64-linux-android

# Install cross-rs for Android cross-compilation
RUN cargo install cross --git https://github.com/cross-rs/cross

WORKDIR /workspace
COPY . .

CMD ["/bin/bash"]
