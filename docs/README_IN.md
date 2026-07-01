[English](README.md) | [Espa帽ol](README_ES.md) | [绠€浣撲腑鏂嘳(README_CN.md) | [绻侀珨涓枃](README_TW.md) | [鏃ユ湰瑾瀅(README_JP.md) | [頃滉淡鞏碷(README_KR.md) | [Polski](README_PL.md) | [Portugu锚s (Brasil)](README_PT-BR.md) | [T眉rk莽e](README_TR.md) | [袪褍褋褋泻懈泄](README_RU.md) | [Ti岷縩g Vi峄噒](README_VI.md) | [Indonesia](README_ID.md) | [注讘专讬转](README_IW.md) |  **啶灌た啶傕う啷€** | [Italiano](README_IT.md)

# SxKernelSU

<img src="https://sxkernelsu1.netlify.app/logo.png" style="width: 96px;" alt="logo">

Android 啶夃お啶曕ぐ啶｀啶?啶曕 啶侧た啶?啶曕ぐ啷嵿え啷囙げ-啶嗋ぇ啶距ぐ啶苦い 啶班啶?啶膏ぎ啶距ぇ啶距え啷?
[![Latest release](https://img.shields.io/github/v/release/tiann/SxKernelSU?label=Release&logo=github)](https://github.com/tiann/SxKernelSU/releases/latest)
[![Weblate](https://img.shields.io/badge/Localization-Weblate-teal?logo=weblate)](https://hosted.weblate.org/engage/sxkernelsu)
[![Channel](https://img.shields.io/badge/Follow-Telegram-blue.svg?logo=telegram)](https://t.me/SxKernelSU)
[![License: GPL v2](https://img.shields.io/badge/License-GPL%20v2-orange.svg?logo=gnu)](https://www.gnu.org/licenses/old-licenses/gpl-2.0.en.html)
[![GitHub License](https://img.shields.io/github/license/tiann/SxKernelSU?logo=gnu)](/LICENSE)

## 啶掂た啶多啶粪い啶距啶?
1. 啶曕ぐ啷嵿え啷囙げ-啶嗋ぇ啶距ぐ啶苦い `su` 啶斷ぐ 啶班啶?啶忇啷嵿じ啷囙じ 啶啶班が啶傕ぇ啶ㄠイ
2. [metamodules](https://sxkernelsu1.netlify.app/guide/metamodule.html) 啶ぐ 啶嗋ぇ啶距ぐ啶苦い 啶啶∴啶啶?啶啶班ぃ啶距げ啷€: Systemless 啶膏啶多啶оえ啷嬥 啶曕 啶侧た啶?啶啶侧啷囙が啶?啶囙啶啶班ぞ啶膏啶熰啶班啷嵿啶班イ
3. [App Profile](https://sxkernelsu1.netlify.app/guide/app-profile.html): Root 啶多啷嵿い啶?啶曕 啶た啶傕啶班 啶啶?啶啶?啶曕ぐ 啶︵啷?
## 啶呧え啷佮啷傕げ啶むぞ 啶呧さ啶膏啶ムぞ

SxKernelSU 啶嗋ぇ啶苦啶距ぐ啶苦 啶む啶?啶ぐ Android GKI 2.0 啶∴た啶掂ぞ啶囙じ (啶曕ぐ啷嵿え啷囙げ 5.10+) 啶曕ぞ 啶膏ぎ啶班啶ムえ 啶曕ぐ啶むぞ 啶灌啷?啶啶班ぞ啶ㄠ 啶曕ぐ啷嵿え啷囙げ (4.14+) 啶 啶膏啶椸い 啶灌啶? 啶侧啶曕た啶?啶曕ぐ啷嵿え啷囙げ 啶曕 啶啶ㄠ啶啶呧げ 啶班啶?啶膏 啶え啶距え啶?啶灌啶椸ぞ啷?
啶囙じ啶曕 啶膏ぞ啶? WSA, ChromeOS 啶斷ぐ 啶曕啶熰啶ㄠぐ-啶嗋ぇ啶距ぐ啶苦い Android 啶膏き啷€ 啶膏ぎ啶班啶ムた啶?啶灌啶傕イ

啶掂ぐ啷嵿い啶ぞ啶?啶啶? `arm64-v8a` 啶斷ぐ `x86_64` 啶嗋ぐ啷嵿啶苦啷囙啷嵿啶?啶膏ぎ啶班啶ムた啶?啶灌啶傕イ

> [!CAUTION]
> 啶灌ぞ啶?啶灌 啶曕 啶曕ぐ啷嵿え啷囙げ 啶膏啶膏啶曕ぐ啶｀啶?啶啶?啶忇 啶愢じ啶?啶う啶侧ぞ啶?啶侧ぞ啶椸 啶曕た啶ぞ 啶椸く啶?啶灌 啶溹た啶膏じ啷?SxKernelSU 啶掂た啶げ 啶灌 啶膏啶むぞ 啶灌 啶斷ぐ `x86_64` 啶ぐ 啶曕ぐ啷嵿え啷囙げ 啶啶ㄠた啶?(kernel panic) 啶熰啶班た啶椸ぐ 啶灌 啶膏啶むぞ 啶灌! 啶呧ぇ啶苦 啶溹ぞ啶ㄠ啶距ぐ啷€ 啶曕 啶侧た啶?啶掂啶じ啶距啶?啶︵啶栢啶?

## 啶啶班く啷嬥

- [啶膏啶ムぞ啶え啶?啶ㄠた啶班啶︵啶禲(https://sxkernelsu1.netlify.app/guide/installation.html)
- [啶曕啶膏 啶え啶距え啶?啶灌 ?](https://sxkernelsu1.netlify.app/guide/how-to-build.html)
- [啶嗋ぇ啶苦啶距ぐ啶苦 啶掂啶じ啶距啶焆(https://sxkernelsu1.netlify.app/)

## 啶呧え啷佮さ啶距う 啶曕ぐ啶ㄠぞ

SxKernelSU 啶曕ぞ 啶呧え啷佮さ啶距う 啶曕ぐ啶ㄠ 啶ぞ 啶啶溹啶︵ぞ 啶呧え啷佮さ啶距う啷嬥 啶曕 啶啶灌い啶?啶え啶距え啷?啶啶?啶膏す啶距く啶むぞ 啶曕 啶侧た啶? 啶曕啶く啶?啶囙じ啶曕ぞ 啶夃お啶啶?啶曕ぐ啷囙 [Weblate](https://hosted.weblate.org/engage/sxkernelsu/).

## 啶す啶?
- Telegram: [@SxKernelSU](https://t.me/SxKernelSU)

## 啶侧ぞ啶囙じ啷囙啶?
- `Kernel` 啶ㄠた啶班啶︵啶多た啶曕ぞ 啶曕 啶呧啶むぐ啷嵿啶?啶ぜ啶距啶侧啶?啶灌啶?[GPL-2-only](https://www.gnu.org/licenses/old-licenses/gpl-2.0.en.html)
- `Kernel` 啶ㄠた啶班啶︵啶多た啶曕ぞ 啶曕 啶涏啶∴ぜ啶曕ぐ 啶呧え啷嵿く 啶膏き啷€ 啶ぞ啶?啶灌啶?[GPL-3.0-or-later](https://www.gnu.org/licenses/gpl-3.0.html)

## 啶嗋き啶距ぐ 啶膏啶氞

- [kernel-assisted-superuser](https://git.zx2c4.com/kernel-assisted-superuser/about/): SxKernelSU 啶掂た啶氞ぞ啶班イ
- [Magisk](https://github.com/topjohnwu/Magisk): 啶多啷嵿い啶苦ざ啶距げ啷€ root 啶夃お啶曕ぐ啶｀イ
- [genuine](https://github.com/brevent/genuine/): apk v2 啶灌じ啷嵿い啶距啷嵿し啶?啶膏い啷嵿く啶距お啶ㄠイ
- [Diamorphine](https://github.com/m0nad/Diamorphine): 啶曕啶?啶班啶熰啶苦 啶曕啶多げ啷?