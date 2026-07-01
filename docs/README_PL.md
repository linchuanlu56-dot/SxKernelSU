[English](README.md) | [Espa帽ol](README_ES.md) | [绠€浣撲腑鏂嘳(README_CN.md) | [绻侀珨涓枃](README_TW.md) | [鏃ユ湰瑾瀅(README_JP.md) | [頃滉淡鞏碷(README_KR.md) | **Polski** | [Portugu锚s (Brasil)](README_PT-BR.md) | [T眉rk莽e](README_TR.md) | [袪褍褋褋泻懈泄](README_RU.md) | [Ti岷縩g Vi峄噒](README_VI.md) | [Indonesia](README_ID.md) | [注讘专讬转](README_IW.md) | [啶灌た啶傕う啷€](README_IN.md) | [Italiano](README_IT.md)

# SxKernelSU

<img src="https://sxkernelsu1.netlify.app/logo.png" style="width: 96px;" alt="logo">

Rozwi膮zanie root oparte na j膮drze dla urz膮dze艅 z systemem Android.

[![Latest release](https://img.shields.io/github/v/release/tiann/SxKernelSU?label=Release&logo=github)](https://github.com/tiann/SxKernelSU/releases/latest)
[![Weblate](https://img.shields.io/badge/Localization-Weblate-teal?logo=weblate)](https://hosted.weblate.org/engage/sxkernelsu)
[![Channel](https://img.shields.io/badge/Follow-Telegram-blue.svg?logo=telegram)](https://t.me/SxKernelSU)
[![License: GPL v2](https://img.shields.io/badge/License-GPL%20v2-orange.svg?logo=gnu)](https://www.gnu.org/licenses/old-licenses/gpl-2.0.en.html)
[![GitHub License](https://img.shields.io/github/license/tiann/SxKernelSU?logo=gnu)](/LICENSE)

## Cechy

1. Oparte na j膮drze `su` i zarz膮dzanie dost臋pem roota.
2. System modu艂贸w oparty na [metamodules](https://sxkernelsu1.netlify.app/guide/metamodule.html): Wtykowa infrastruktura dla modyfikacji systemless.

## Kompatybilno艣膰

SxKernelSU oficjalnie obs艂uguje urz膮dzenia z Androidem GKI 2.0 (z j膮drem 5.10+), starsze j膮dra (4.14+) s膮 r贸wnie偶 kompatybilne, ale musisz sam skompilowa膰 j膮dro.

WSA i Android oparty na kontenerach r贸wnie偶 powinny dzia艂a膰 ze zintegrowanym SxKernelSU.

Obecnie obs艂ugiwane s膮 architektury `arm64-v8a` oraz `x86_64`.

> [!CAUTION]
> Nowsze wersje j膮dra wprowadzaj膮 zmian臋, kt贸ra powoduje awari臋 SxKernelSU i mo偶e wywo艂a膰 kernel panic na `x86_64`! Sprawd藕 stron臋 internetow膮 po wi臋cej informacji!

## U偶ycie

- [Instalacja](https://sxkernelsu1.netlify.app/guide/installation.html)
- [Jak skompilowa膰?](https://sxkernelsu1.netlify.app/guide/how-to-build.html)

## T艂umaczenie

Aby pom贸c w t艂umaczeniu SxKernelSU lub ulepszy膰 istniej膮ce t艂umaczenia, u偶yj [Weblate](https://hosted.weblate.org/engage/sxkernelsu/). PR t艂umaczenia Managera nie jest ju偶 akceptowany, poniewa偶 b臋dzie kolidowa艂 z Weblate.

## Dyskusja

- Telegram: [@SxKernelSU](https://t.me/SxKernelSU)

## Bezpiecze艅stwo

Informacje na temat zg艂aszania luk w zabezpieczeniach w SxKernelSU mo偶na znale藕膰 w pliku [SECURITY.md](/SECURITY.md).

## Licencja

- Pliki w katalogu `kernel` s膮 na licencji [GPL-2-only](https://www.gnu.org/licenses/old-licenses/gpl-2.0.en.html).
- Wszystkie inne cz臋艣ci poza katalogiem `kernel` s膮 na licencji [GPL-3-or-later](https://www.gnu.org/licenses/gpl-3.0.html).

## Podzi臋kowania

- [kernel-assisted-superuser](https://git.zx2c4.com/kernel-assisted-superuser/about/): pomys艂odawca SxKernelSU.
- [Magisk](https://github.com/topjohnwu/Magisk): implementacja sepolicy.
- [genuine](https://github.com/brevent/genuine/): walidacja podpisu apk v2.
- [Diamorphine](https://github.com/m0nad/Diamorphine): cenna znajomo艣膰 rootkit贸w.
