[English](README.md) | [Espa帽ol](README_ES.md) | [绠€浣撲腑鏂嘳(README_CN.md) | [绻侀珨涓枃](README_TW.md) | [鏃ユ湰瑾瀅(README_JP.md) | [頃滉淡鞏碷(README_KR.md) | [Polski](README_PL.md) | **Portugu锚s (Brasil)** | [T眉rk莽e](README_TR.md) | [袪褍褋褋泻懈泄](README_RU.md) | [Ti岷縩g Vi峄噒](README_VI.md) | [Indonesia](README_ID.md) | [注讘专讬转](README_IW.md) | [啶灌た啶傕う啷€](README_IN.md) | [Italiano](README_IT.md)

# SxKernelSU

<img src="https://sxkernelsu1.netlify.app/logo.png" style="width: 96px;" alt="logo">

Uma solu莽茫o root baseada em kernel para dispositivos Android.

[![Latest release](https://img.shields.io/github/v/release/tiann/SxKernelSU?label=Release&logo=github)](https://github.com/tiann/SxKernelSU/releases/latest)
[![Weblate](https://img.shields.io/badge/Localiza莽茫o-Weblate-teal?logo=weblate)](https://hosted.weblate.org/engage/sxkernelsu)
[![Channel](https://img.shields.io/badge/Seguir-Telegram-blue.svg?logo=telegram)](https://t.me/SxKernelSU)
[![License: GPL v2](https://img.shields.io/badge/Licen莽a-GPL%20v2-orange.svg?logo=gnu)](https://www.gnu.org/licenses/old-licenses/gpl-2.0.en.html)
[![GitHub License](https://img.shields.io/github/license/tiann/SxKernelSU?logo=gnu)](/LICENSE)

## Caracter铆sticas

1. `su` e gerenciamento de acesso root baseado em kernel.
2. Sistema de m贸dulos baseado em [metamodules](https://sxkernelsu1.netlify.app/pt_BR/guide/metamodule.html): Infraestrutura plug谩vel para modifica莽玫es systemless.
3. [Perfil do Aplicativo](https://sxkernelsu1.netlify.app/pt_BR/guide/app-profile.html): Tranque o poder root em uma gaiola.

## Estado de compatibilidade

O SxKernelSU oferece suporte oficial a dispositivos Android GKI 2.0 (kernel 5.10+). Kernels mais antigos (4.14+) tamb茅m s茫o compat铆veis, mas ser谩 necess谩rio construir o kernel manualmente.

Com isso, WSA, ChromeOS e Android baseado em cont锚iner s茫o todos suportados.

Atualmente, as arquiteturas `arm64-v8a` e `x86_64` s茫o suportadas.

> [!CAUTION]
> Vers玫es recentes do kernel implementaram uma mudan莽a que quebra a compatibilidade, fazendo com que o SxKernelSU falhe e potencialmente cause um kernel panic no `x86_64`! Verifique o site para mais informa莽玫es!

## Uso

 - [Instala莽茫o](https://sxkernelsu1.netlify.app/pt_BR/guide/installation.html)
 - [Como compilar](https://sxkernelsu1.netlify.app/pt_BR/guide/how-to-build.html)
 - [Site oficial](https://sxkernelsu1.netlify.app/pt_BR/)

## Tradu莽茫o

Para contribuir com a tradu莽茫o do SxKernelSU ou aprimorar tradu莽玫es existentes, por favor, use o [Weblate](https://hosted.weblate.org/engage/sxkernelsu/). PR para a tradu莽茫o do Manager n茫o s茫o mais aceitas, pois podem entrar em conflito com o Weblate.

## Discuss茫o

- Telegram: [@SxKernelSU](https://t.me/SxKernelSU)

## Seguran莽a

Para obter informa莽玫es sobre como relatar vulnerabilidades de seguran莽a do SxKernelSU, consulte [SECURITY.md](/SECURITY.md).

## Licen莽a

- Os arquivos no diret贸rio `kernel` s茫o [GPL-2.0-only](https://www.gnu.org/licenses/old-licenses/gpl-2.0.en.html).
- Todas as outras partes, exceto o diret贸rio `kernel` s茫o [GPL-3.0-or-later](https://www.gnu.org/licenses/gpl-3.0.html).

## Cr茅ditos

- [Kernel-Assisted Superuser](https://git.zx2c4.com/kernel-assisted-superuser/about/): A ideia do SxKernelSU.
- [Magisk](https://github.com/topjohnwu/Magisk): A poderosa ferramenta root.
- [genuine](https://github.com/brevent/genuine/): Valida莽茫o de assinatura APK v2.
- [Diamorphine](https://github.com/m0nad/Diamorphine): Algumas habilidades de rootkit.
