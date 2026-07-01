**English** | [Español](README_ES.md) | [简体中文](README_CN.md) | [繁體中文](README_TW.md) | [日本語](README_JP.md) | [한국어](README_KR.md) | [Polski](README_PL.md) | [Português (Brasil)](README_PT-BR.md) | [Türkçe](README_TR.md) | [Русский](README_RU.md) | [Tiếng Việt](README_VI.md) | [Indonesia](README_ID.md) | [עברית](README_IW.md) | [हिंदी](README_IN.md) | [Italiano](README_IT.md)

# SxKernelSU

<img src="https://kernelsu.org/logo.png" style="width: 96px;" alt="logo">

A kernel-based root solution for Android devices — forked from KernelSU, supercharged.

[![Latest release](https://img.shields.io/github/v/release/linchuanlu56-dot/SxKernelSU?label=Release&logo=github)](https://github.com/linchuanlu56-dot/SxKernelSU/releases/latest)
[![License: GPL v2](https://img.shields.io/badge/License-GPL%20v2-orange.svg?logo=gnu)](https://www.gnu.org/licenses/old-licenses/gpl-2.0.en.html)
[![Changelog](https://img.shields.io/badge/Changelog-v2.0.0-FF6B9D?logo=github)](CHANGELOG.md)

> 🌸 **SxKernelSU** — Everything KernelSU is, plus Ghost-tier root hiding, atomic module management, and a kawaii anime theme.

---

## ✨ What's New

| Version | What |
|---------|------|
| **v2.0.0** 🧠 | **Ghost Architecture**: 6-layer root hiding, Shadow Syscall Table, Per-Task VFS, Biological Immune System |
| **v1.5.0** 🎀 | **Anime Theme**: Sakura pink, sparkle background, card animations, bouncy transitions |
| **v1.4.0** 🔒 | Atomic module install/rollback, debugfs interface, Docker dev environment |
| **v1.3.0** 🛡️ | BUG_ON→WARN_ON, init timeout, symbol resolver fallback, workqueue migration |

📖 **[Full Changelog →](CHANGELOG.md)**

---

## Features

1. ✅ **All KernelSU features** — Kernel-based `su`, module system, App Profile, SELinux hide
2. 🧠 **Ghost Hiding** — Module invisible to `lsmod`, `/proc/modules`, `/proc/kallsyms`. Per-task VFS filtering. Shadow syscall table.
3. 🎀 **Anime Theme** — Default Sakura pink, animated sparkles, kawaii card design
4. 🔒 **Atomic Modules** — Auto-backup & rollback on install failure
5. 🛡️ **Stability** — No kernel panics on SELinux anomalies, 60s script timeout, ordered module exit

## Compatibility

Android GKI 2.0 (kernel 5.10+) and older kernels (4.14+). `arm64-v8a` and `x86_64`.

> [!CAUTION]
> This is a fork with significant kernel-level modifications. Test thoroughly before production use.

## 📜 Changelog

All version history with detailed descriptions is available in the [CHANGELOG.md](CHANGELOG.md) file.

Quick links:
- [v2.0.0 — Ghost Release](CHANGELOG.md#v200--2026-07-01--ghost-release)
- [v1.5.0 — Anime Release](CHANGELOG.md#v150--2026-07-01--anime-release)
- [v1.4.0 — Lock & Load](CHANGELOG.md#v140--2026-07-01--lock--load)
- [v1.3.0 — Stability Release](CHANGELOG.md#v130--2026-06-30--stability-release)
- [v1.2.0 — Initial Fork](CHANGELOG.md#v120--2026-06-30--initial-fork)

---

## Build & Install

```bash
# Build kernel module
cd kernel && CONFIG_KSU=m CC=clang make

# Build userspace
cargo build --target aarch64-linux-android --release

# Build Manager APK
cd manager && ./gradlew assembleRelease
```

Or use Docker:

```bash
docker compose up -d
docker compose exec dev bash
```

## License

GPL-2.0 (kernel) / GPL-3.0-or-later (userspace)
