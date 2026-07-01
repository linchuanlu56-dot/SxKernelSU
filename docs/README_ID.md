[English](README.md) | [Espa帽ol](README_ES.md) | [绠€浣撲腑鏂嘳(README_CN.md) | [绻侀珨涓枃](README_TW.md) | [鏃ユ湰瑾瀅(README_JP.md) | [頃滉淡鞏碷(README_KR.md) | [Polski](README_PL.md) | [Portugu锚s (Brasil)](README_PT-BR.md) | [T眉rk莽e](README_TR.md) | [袪褍褋褋泻懈泄](README_RU.md) | [Ti岷縩g Vi峄噒](README_VI.md) | **Indonesia** | [注讘专讬转](README_IW.md) | [啶灌た啶傕う啷€](README_IN.md) | [Italiano](README_IT.md)

# SxKernelSU

<img src="https://sxkernelsu1.netlify.app/logo.png" style="width: 96px;" alt="logo">

Solusi root berbasis Kernel untuk perangkat Android.

[![Latest release](https://img.shields.io/github/v/release/tiann/SxKernelSU?label=Release&logo=github)](https://github.com/tiann/SxKernelSU/releases/latest)
[![Weblate](https://img.shields.io/badge/Localization-Weblate-teal?logo=weblate)](https://hosted.weblate.org/engage/sxkernelsu)
[![Channel](https://img.shields.io/badge/Follow-Telegram-blue.svg?logo=telegram)](https://t.me/SxKernelSU)
[![License: GPL v2](https://img.shields.io/badge/License-GPL%20v2-orange.svg?logo=gnu)](https://www.gnu.org/licenses/old-licenses/gpl-2.0.en.html)
[![GitHub License](https://img.shields.io/github/license/tiann/SxKernelSU?logo=gnu)](/LICENSE)

## Fitur

1. Manajemen akses root dan `su` berbasis kernel.
2. Sistem modul berdasarkan [metamodules](https://sxkernelsu1.netlify.app/id_ID/guide/metamodule.html): Infrastruktur pluggable untuk modifikasi systemless.
3. [Profil Aplikasi](https://sxkernelsu1.netlify.app/guide/app-profile.html): Kunci daya root di dalam sangkar.

## Status Kompatibilitas

SxKernelSU secara resmi mendukung perangkat Android GKI 2.0 (dengan kernel 5.10+), kernel lama (4.14+) juga kompatibel, tetapi Anda perlu membuat kernel sendiri.

WSA, ChromeOS, dan Android berbasis wadah juga dapat bekerja dengan SxKernelSU terintegrasi.

Saat ini, arsitektur `arm64-v8a` dan `x86_64` didukung.

> [!CAUTION]
> Versi kernel terbaru telah menerapkan perubahan yang merusak yang menyebabkan SxKernelSU gagal dan berpotensi memicu kernel panic pada `x86_64`! Cek situs web untuk info lebih lanjut!

## Penggunaan

- [Petunjuk Instalasi](https://sxkernelsu1.netlify.app/id_ID/guide/installation.html)
- [Bagaimana cara membuat?](https://sxkernelsu1.netlify.app/id_ID/guide/how-to-build.html)
- [Situs Web Resmi](https://sxkernelsu1.netlify.app/id_ID/)

## Terjemahan

Untuk menerjemahkan SxKernelSU ke dalam bahasa Anda atau menyempurnakan terjemahan yang sudah ada, harap gunakan [Weblat](https://hosted.weblate.org/engage/sxkernelsu/).

## Diskusi

- Telegram: [@SxKernelSU](https://t.me/SxKernelSU)

## Lisensi

- File di bawah direktori `kernel` adalah [GPL-2-only](https://www.gnu.org/licenses/old-licenses/gpl-2.0.en.html).
- Semua bagian lain kecuali direktori `kernel` adalah [GPL-3.0-or-later](https://www.gnu.org/licenses/gpl-3.0.html).

## Kredit

- [kernel-assisted-superuser](https://git.zx2c4.com/kernel-assisted-superuser/about/): ide SxKernelSU.
- [Magisk](https://github.com/topjohnwu/Magisk): alat root yang ampuh.
- [genuine](https://github.com/brevent/genuine/): validasi tanda tangan apk v2.
- [Diamorphine](https://github.com/m0nad/Diamorphine): beberapa keterampilan rootkit.
