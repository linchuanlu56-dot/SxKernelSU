[English](REAME.md) | [Espa帽ol](README_ES.md) | [绠€浣撲腑鏂嘳(README_CN.md) | [绻侀珨涓枃](README_TW.md) | [鏃ユ湰瑾瀅(README_JP.md) | [頃滉淡鞏碷(README_KR.md) | [Polski](README_PL.md) | [Portugu锚s (Brasil)](README_PT-BR.md) | [T眉rk莽e](README_TR.md) | [袪褍褋褋泻懈泄](README_RU.md) | [Ti岷縩g Vi峄噒](README_VI.md) | [Indonesia](README_ID.md) | [注讘专讬转](README_IW.md) | [啶灌た啶傕う啷€](README_IN.md) | **Italiano**

# SxKernelSU

<img src="https://sxkernelsu1.netlify.app/logo.png" style="width: 96px;" alt="logo">

Una soluzione per il root basata sul kernel per i dispositivi Android. 

[![Latest release](https://img.shields.io/github/v/release/tiann/SxKernelSU?label=Release&logo=github)](https://github.com/tiann/SxKernelSU/releases/latest)
[![Weblate](https://img.shields.io/badge/Localization-Weblate-teal?logo=weblate)](https://hosted.weblate.org/engage/sxkernelsu)
[![Canale Telegraml](https://img.shields.io/badge/Follow-Telegram-blue.svg?logo=telegram)](https://t.me/SxKernelSU)
[![Licenza componenti kernel: GPL v2](https://img.shields.io/badge/License-GPL%20v2-orange.svg?logo=gnu)](https://www.gnu.org/licenses/old-licenses/gpl-2.0.en.html)
[![Licenza elementi non kern](https://img.shields.io/github/license/tiann/SxKernelSU?logo=gnu)](/LICENSE)

## Funzionalit脿

1. `su` e accesso root basato sul kernel.
2. Sistema di moduli basato su [metamodules](https://sxkernelsu1.netlify.app/guide/metamodule.html): Infrastruttura modulare per modifiche systemless.
3. [App profile](https://sxkernelsu1.netlify.app/guide/app-profile.html): Limita i poteri dell'accesso root a permessi specifici.

## Compatibilit脿

SxKernelSU supporta ufficialmente i dispositivi Android GKI 2.0 (kernel 5.10 o superiore). I kernel precedenti (kernel 4.14+) sono anche compatibili, ma il kernel deve essere compilato manualmente.

Questo implica che WSA, ChromeOS e tutti le varianti di Android basate su container e virtualizzazione sono supportate.

Attualmente sono supportate le architetture `arm64-v8a` e `x86_64`.

> [!CAUTION]
> Le versioni recenti del kernel hanno implementato una modifica che causa il fallimento di SxKernelSU e potenzialmente innesca un kernel panic su `x86_64`! Controlla il sito web per maggiori informazioni!

## Utilizzo

- [Istruzioni per l'installazione](https://sxkernelsu1.netlify.app/guide/installation.html)
- [Come compilare manualmente?](https://sxkernelsu1.netlify.app/guide/how-to-build.html)
- [Sito web ufficiale](https://sxkernelsu1.netlify.app/)

## Traduzioni

Per aiutare a tradurre SxKernelSU o migliorare le traduzioni esistenti, si 猫 pregati di utilizzare 
To help translate SxKernelSU or improve existing translations, please use [Weblate](https://hosted.weblate.org/engage/sxkernelsu/). Le richieste di pull delle traduzioni del manager non saranno pi霉 accettate perch茅 sarebbero in conflitto con Weblate.

## Discussione

- Telegram: [@SxKernelSU](https://t.me/SxKernelSU)

## Securezza

Per informazioni riguardo la segnalazione di vulnerabilit脿 di sicurezza per SxKernelSU, leggi [SECURITY.md](/SECURITY.md).

## Licenza

- I file nella cartella `kernel` sono forniti secondo la licenza [GPL-2.0-only](https://www.gnu.org/licenses/old-licenses/gpl-2.0.en.html).
- Tutte le altre parti, ad eccezione della certella `kernel`, seguono la licenza [GPL-3.0-or-later](https://www.gnu.org/licenses/gpl-3.0.html).

## Riconoscimenti e attribuzioni

- [kernel-assisted-superuser](https://git.zx2c4.com/kernel-assisted-superuser/about/): l'idea alla base di SxKernelSU.
- [Magisk](https://github.com/topjohnwu/Magisk): la potente utilit脿 per il root.
- [genuine](https://github.com/brevent/genuine/): verifica della firma apk v2.
- [Diamorphine](https://github.com/m0nad/Diamorphine): alcune capacit脿 di rootkit.
