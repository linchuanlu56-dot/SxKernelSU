[English](README.md) | **Espa帽ol** | [绠€浣撲腑鏂嘳(README_CN.md) | [绻侀珨涓枃](README_TW.md) | [鏃ユ湰瑾瀅(README_JP.md) | [頃滉淡鞏碷(README_KR.md) | [Polski](README_PL.md) | [Portugu锚s (Brasil)](README_PT-BR.md) | [T眉rk莽e](README_TR.md) | [袪褍褋褋泻懈泄](README_RU.md) | [Ti岷縩g Vi峄噒](README_VI.md) | [Indonesia](README_ID.md) | [注讘专讬转](README_IW.md) | [啶灌た啶傕う啷€](README_IN.md) | [Italiano](README_IT.md)

# SxKernelSU

<img src="https://sxkernelsu1.netlify.app/logo.png" style="width: 96px;" alt="logo">

Una soluci贸n root basada en el kernel para dispositivos Android.

[![Latest release](https://img.shields.io/github/v/release/tiann/SxKernelSU?label=Release&logo=github)](https://github.com/tiann/SxKernelSU/releases/latest)
[![Weblate](https://img.shields.io/badge/Localizaci贸n-Weblate-teal?logo=weblate)](https://hosted.weblate.org/engage/sxkernelsu)
[![Channel](https://img.shields.io/badge/Seguir-Telegram-blue.svg?logo=telegram)](https://t.me/SxKernelSU)
[![License: GPL v2](https://img.shields.io/badge/Licencia-GPL%20v2-orange.svg?logo=gnu)](https://www.gnu.org/licenses/old-licenses/gpl-2.0.en.html)
[![GitHub License](https://img.shields.io/github/license/tiann/SxKernelSU?logo=gnu)](/LICENSE)

## Caracter铆sticas

1. Binario `su` basado en el kernel y gesti贸n de acceso root.
2. Sistema de m贸dulos basado en [metamodules](https://sxkernelsu1.netlify.app/guide/metamodule.html): Infraestructura conectable para modificaciones sin sistema.

## Estado de compatibilidad

**SxKernelSU** soporta de forma oficial dispositivos Android con **GKI 2.0** (a partir de la versi贸n **5.10** del kernel). Los kernels antiguos (a partir de la versi贸n **4.14**) tambi茅n son compatibles, pero necesitas compilarlos por tu cuenta.

Con esto, WSA, ChromeOS y Android basado en contenedores est谩n todos compatibles.

Actualmente, se soportan las arquitecturas `arm64-v8a` y `x86_64`.

> [!CAUTION]
> 隆Las versiones recientes del kernel han implementado un cambio que rompe la compatibilidad, causando que SxKernelSU falle y potencialmente provoque un kernel panic en `x86_64`! 隆Consulta el sitio web para m谩s informaci贸n!

## Uso

- [驴C贸mo instalarlo?](https://sxkernelsu1.netlify.app/guide/installation.html)
- [驴C贸mo compilarlo?](https://sxkernelsu1.netlify.app/guide/how-to-build.html)
- [Site oficial](https://sxkernelsu1.netlify.app/)

## Traducci贸n

Para ayudar a traducir SxKernelSU o mejorar las traducciones existentes, utilice [Weblate](https://hosted.weblate.org/engage/sxkernelsu/). Ya no se aceptan PR de la traducci贸n de Manager porque entrar谩 en conflicto con Weblate.

## Discusi贸n

- Telegram: [@SxKernelSU](https://t.me/SxKernelSU)

## Seguridad

Para obtener informaci贸n sobre c贸mo informar vulnerabilidades de seguridad en SxKernelSU, consulte [SECURITY.md](/SECURITY.md).

##  Licencia

- Los archivos bajo el directorio `kernel` est谩n licenciados bajo [GPL-2-only](https://www.gnu.org/licenses/old-licenses/gpl-2.0.en.html).
- Todas las dem谩s partes, a excepci贸n del directorio `kernel`, est谩n licenciados bajo [GPL-3-or-later](https://www.gnu.org/licenses/gpl-3.0.html).

## Cr茅ditos

- [kernel-assisted-superuser](https://git.zx2c4.com/kernel-assisted-superuser/about/): la idea de SxKernelSU.
- [Magisk](https://github.com/topjohnwu/Magisk): la poderosa herramienta root.
- [genuine](https://github.com/brevent/genuine/): validaci贸n de firma apk v2.
- [Diamorphine](https://github.com/m0nad/Diamorphine): algunas habilidades de rootkit.
