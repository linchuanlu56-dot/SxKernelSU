alias bk := build-sksud
alias bm := build-manager
alias bkm := build-kmi
alias clean := clean-all
alias check := clippy

# ── One-time development environment setup ──────────────────────────────────
dev-setup:
    #!/usr/bin/env bash
    set -euo pipefail
    echo "==> SxKernelSU dev-setup"

    # Rust / cross toolchain
    if ! command -v rustup &>/dev/null; then
        echo "Installing rustup..."
        curl --proto '=https' --tlsv1.2 -sSf https://sh.rustup.rs | sh -s -- -y
    fi
    rustup update stable

    # Android NDK
    if [ -z "${ANDROID_NDK_HOME:-}" ]; then
        echo "WARNING: ANDROID_NDK_HOME is not set."
        echo "  Install NDK via Android Studio SDK Manager, then:"
        echo "  export ANDROID_NDK_HOME=\$HOME/Android/Sdk/ndk/<version>"
    fi

    # cargo-cross → needed for cross-compiling sksud
    if ! command -v cross &>/dev/null; then
        echo "Installing cross-rs..."
        cargo install cross --git https://github.com/cross-rs/cross
    fi

    # Generate default .cargo/config.toml
    python3 scripts/setup_cargo_config.py 2>/dev/null || true

    echo "==> Setup complete."
    echo "  just build-sksud    → cross-compile sksud for aarch64"
    echo "  just build-manager → build Android Manager APK"
    echo "  just build-kmi     → build LKM for all KMI targets"
    echo "  just clippy        → run Rust lints"

# ── Build sksud (Rust userspace daemon) ──────────────────────────────────────
build-sksud *TARGET="aarch64-linux-android":
    cross build --target {{ TARGET }} --release

# ── Build Android Manager APK ───────────────────────────────────────────────
build-manager: build-sksud
    cp target/aarch64-linux-android/release/sksud manager/app/src/main/jniLibs/arm64-v8a/libsksud.so
    cd manager && ./gradlew aDebug

# ── Build LKM for a specific KMI (requires DDK toolchain) ───────────────────
build-kmi KMI="android15-6.6":
    cd kernel && CONFIG_SKS=m CC=clang make

# ── Lint ────────────────────────────────────────────────────────────────────
clippy:
    cargo fmt --check
    cross clippy --target aarch64-linux-android --release --manifest-path userspace/sksud/Cargo.toml

clippy-fix:
    cargo fmt
    cross clippy --target aarch64-linux-android --release --manifest-path userspace/sksud/Cargo.toml --fix

# ── Clean ───────────────────────────────────────────────────────────────────
clean-all:
    cargo clean
    cd kernel && make clean 2>/dev/null || true
    cd manager && ./gradlew clean 2>/dev/null || true
