**English** | [Espa帽ol](README_ES.md) | [绠€浣撲腑鏂嘳(README_CN.md) | [绻侀珨涓枃](README_TW.md) | [鏃ユ湰瑾瀅(README_JP.md) | [頃滉淡鞏碷(README_KR.md) | [Polski](README_PL.md) | [Portugu锚s (Brasil)](README_PT-BR.md) | [T眉rk莽e](README_TR.md) | [袪褍褋褋泻懈泄](README_RU.md) | [Ti岷縩g Vi峄噒](README_VI.md) | [Indonesia](README_ID.md) | [注讘专讬转](README_IW.md) | [啶灌た啶傕う啷€](README_IN.md) | [Italiano](README_IT.md)

# SxKernelSU

<img src="https://sxkernelsu1.netlify.app/logo.png" style="width: 96px;" alt="logo">

A kernel-based root solution for Android devices.

[![Latest release](https://img.shields.io/github/v/release/tiann/SxKernelSU?label=Release&logo=github)](https://github.com/tiann/SxKernelSU/releases/latest)
[![Weblate](https://img.shields.io/badge/Localization-Weblate-teal?logo=weblate)](https://hosted.weblate.org/engage/sxkernelsu)
[![Channel](https://img.shields.io/badge/Follow-Telegram-blue.svg?logo=telegram)](https://t.me/SxKernelSU)
[![License: GPL v2](https://img.shields.io/badge/License-GPL%20v2-orange.svg?logo=gnu)](https://www.gnu.org/licenses/old-licenses/gpl-2.0.en.html)
[![GitHub License](https://img.shields.io/github/license/tiann/SxKernelSU?logo=gnu)](/LICENSE)

## Features

1. Kernel-based `su` and root access management.
2. Module system based on [metamodules](https://sxkernelsu1.netlify.app/guide/metamodule.html): Pluggable infrastructure for systemless modifications.
3. [App Profile](https://sxkernelsu1.netlify.app/guide/app-profile.html): Lock up the root power in a cage.

## Compatibility state

SxKernelSU officially supports Android GKI 2.0 devices (kernel 5.10+). Older kernels (4.14+) are also supported, but the kernel will need to be built manually.

With this, WSA, ChromeOS, and container-based Android are all supported.

Currently, the `arm64-v8a` and `x86_64` architectures are supported.

> [!CAUTION]
> Recent kernel versions have implemented a breaking change causing SxKernelSU to fail and potentially trigger a kernel panic on `x86_64`! Check the website for more info!

## Usage

- [Installation](https://sxkernelsu1.netlify.app/guide/installation.html)
- [How to build](https://sxkernelsu1.netlify.app/guide/how-to-build.html)
- [Official website](https://sxkernelsu1.netlify.app/)

## Translation

To help translate SxKernelSU, we no longer accept translation contributions via Weblate. All translations are now handled using LLMs.

If you would like to add support for a new language, feel free to open a PR. Please note that modifications to existing English and Chinese translations are not accepted.

## Discussion

- Telegram: [@SxKernelSU](https://t.me/SxKernelSU)

## Security

For information on reporting security vulnerabilities in SxKernelSU, see [SECURITY.md](/SECURITY.md).

## License

- Files under the `kernel` directory are [GPL-2.0-only](https://www.gnu.org/licenses/old-licenses/gpl-2.0.en.html).
- All other parts except the `kernel` directory are [GPL-3.0-or-later](https://www.gnu.org/licenses/gpl-3.0.html).

## Credits

- [Kernel-Assisted Superuser](https://git.zx2c4.com/kernel-assisted-superuser/about/): The SxKernelSU idea.
- [Magisk](https://github.com/topjohnwu/Magisk): The powerful root tool.
- [genuine](https://github.com/brevent/genuine/): APK v2 signature validation.
- [Diamorphine](https://github.com/m0nad/Diamorphine): Some rootkit skills.
