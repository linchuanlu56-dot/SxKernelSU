# Perfil do Aplicativo

O Perfil do Aplicativo 茅 um mecanismo fornecido pelo SxKernelSU para personalizar a configura莽茫o de v谩rios apps.

Para apps com privil茅gios root (ou seja, capazes de usar `su`), o Perfil do Aplicativo tamb茅m pode ser chamado de Perfil root. Ele permite a customiza莽茫o das regras `uid`, `gid`, `grupos`, `capacidades` e `SELinux` do comando `su`, restringindo assim os privil茅gios do usu谩rio root. Por exemplo, ele pode conceder permiss玫es de rede apenas para apps de firewall enquanto nega permiss玫es de acesso a arquivos, ou pode conceder permiss玫es de shell em vez de acesso root completo para apps congelados: **mantendo o poder confinado com o princ铆pio do menor privil茅gio.**

Para apps comuns sem privil茅gios root, o Perfil do Aplicativo pode controlar o comportamento do kernel e do sistema de m贸dulos em rela莽茫o a esses apps. Por exemplo, pode determinar se as modifica莽玫es resultantes dos m贸dulos devem ser abordadas. O kernel e o sistema de m贸dulos podem tomar decis玫es com base nesta configura莽茫o, como realizar opera莽玫es semelhantes a "ocultar".

## Perfil root

### UID, GID e Grupos

Os sistemas Linux possuem dois conceitos: usu谩rios e grupos. Cada usu谩rio possui um ID de usu谩rio (UID) e pode pertencer a v谩rios grupos, cada um com seu pr贸prio ID de grupo (GID). Esses IDs s茫o usados 鈥嬧€媝ara identificar usu谩rios no sistema e determinar quais recursos do sistema eles podem acessar.

Os usu谩rios com UID 0 s茫o conhecidos como usu谩rios root, e grupos com GID 0 s茫o chamados de grupos root. O grupo de usu谩rios root geralmente tem os privil茅gios mais altos no sistema.

No caso do sistema Android, cada app funciona como um usu谩rio separado (exceto em casos de UID compartilhado) e recebe um UID exclusivo. Por exemplo, `0` representa o usu谩rio root, `1000` representa `system`, `2000` ao ADB shell e os UIDs de `10000` a `19999` s茫o atribu铆dos a apps comuns.

::: info INFORMA脟脮ES
Aqui, o UID mencionado n茫o 茅 o mesmo que o conceito de m煤ltiplos usu谩rios ou perfis de trabalho no sistema Android. Os perfis de trabalho s茫o, na verdade, implementados particionando o intervalo UID. Por exemplo, 10000-19999 representa o usu谩rio principal, enquanto 110000-119999 representa um perfil de trabalho. Cada app comum entre eles possui seu pr贸prio UID exclusivo.
:::

Cada app pode ter v谩rios grupos, com o GID representando o grupo principal, que geralmente corresponde ao UID. Outros grupos s茫o conhecidos como grupos suplementares. Certas permiss玫es s茫o controladas por meio de grupos, como permiss玫es de acesso 脿 rede ou acesso Bluetooth.

Por exemplo, se executarmos o comando `id` no ADB shell, a sa铆da pode ser semelhante a esta:

```sh
oriole:/ $ id
uid=2000(shell) gid=2000(shell) groups=2000(shell),1004(input),1007(log),1011(adb),1015(sdcard_rw),1028(sdcard_r),1078(ext_data_rw),1079(ext_obb_rw),3001(net_bt_admin),3002(net_bt),3003(inet),3006(net_bw_stats),3009(readproc),3011(uhid),3012(readtracefs) context=u:r:shell:s0
```

Aqui, o UID 茅 `2000` e o GID (ID do grupo prim谩rio) tamb茅m 茅 `2000`. Al茅m disso, pertence a v谩rios grupos suplementares, como `inet` (indicando a capacidade de criar soquetes `AF_INET` e `AF_INET6`) e `sdcard_rw` (indicando permiss玫es de leitura/grava莽茫o para o cart茫o SD).

O Perfil root do SxKernelSU permite personalizar o UID, GID e grupos para o processo root ap贸s a execu莽茫o de `su`. Por exemplo, o Perfil root de um app root pode definir seu UID como `2000`, o que significa que, ao usar `su`, as permiss玫es reais do app est茫o no n铆vel do ADB shell. Al茅m disso, o grupo `inet` pode ser removido, evitando que o comando `su` tenha acesso 脿 rede.

::: tip OBSERVA脟脙O
O Perfil do Aplicativo controla apenas as permiss玫es do processo root ap贸s usar `su` e n茫o afeta as permiss玫es do pr贸prio app. Se um app solicitou permiss茫o para acessar a rede, ele ainda poder谩 acessar a rede mesmo sem usar `su`. Remover o grupo `inet` de `su` apenas impede que `su` acesse a rede.
:::

O Perfil root 茅 aplicado no kernel e n茫o depende do comportamento volunt谩rio de apps root, ao contr谩rio da troca de usu谩rios ou grupos por meio de `su`. A concess茫o da permiss茫o `su` depende inteiramente do usu谩rio e n茫o do desenvolvedor.

### Capacidades

As capacidades s茫o um mecanismo para separa莽茫o de privil茅gios no Linux.

Para realizar verifica莽玫es de permiss茫o, as implementa莽玫es tradicionais do `UNIX` distinguem duas categorias de processos: processos privilegiados (cujo ID de usu谩rio efetivo 茅 `0`, referido como superusu谩rio ou root) e processos sem privil茅gios (cujo UID efetivo 茅 diferente de zero). Os processos privilegiados ignoram todas as verifica莽玫es de permiss茫o do kernel, enquanto os processos n茫o privilegiados est茫o sujeitos 脿 verifica莽茫o completa de permiss茫o com base nas credenciais do processo (geralmente: UID efetivo, GID efetivo e lista de grupos suplementares).

A partir do Linux 2.2, o Linux divide os privil茅gios tradicionalmente associados ao superusu谩rio em unidades distintas, conhecidas como capacidades, que podem ser ativadas e desativadas de forma independente.

Cada capacidade representa um ou mais privil茅gios. Por exemplo, `CAP_DAC_READ_SEARCH` representa a capacidade de ignorar verifica莽玫es de permiss茫o para leitura de arquivos, bem como permiss玫es de leitura e execu莽茫o de diret贸rio. Se um usu谩rio com um UID efetivo `0` (usu谩rio root) n茫o tiver a capacidade `CAP_DAC_READ_SEARCH` ou superiores, isso significa que mesmo sendo root, ele n茫o pode ler arquivos 脿 vontade.

O Perfil root do SxKernelSU permite a personaliza莽茫o das capacidades do processo root ap贸s a execu莽茫o de `su`, concedendo assim "privil茅gios root" de forma parcial. Ao contr谩rio do UID e GID mencionados acima, certos apps root exigem um UID de `0` ap贸s usar `su`. Nesses casos, limitar as capacidades deste usu谩rio root com UID `0` pode restringir as opera莽玫es que ele pode realizar.

::: tip FORTE RECOMENDA脟脙O
A [documenta莽茫o oficial](https://man7.org/linux/man-pages/man7/capabilities.7.html) da capacidade do Linux fornece explica莽玫es detalhadas das habilidades representadas por cada capacidade. Se voc锚 pretende customizar as capacidade, 茅 altamente recomend谩vel que voc锚 leia este documento primeiro.
:::

### SELinux

SELinux 茅 um poderoso mecanismo do Controle de Acesso Obrigat贸rio (MAC). Ele opera com base no princ铆pio de **nega莽茫o padr茫o**. Qualquer a莽茫o n茫o explicitamente permitida 茅 negada.

O SELinux pode ser executado em dois modos globais:

1. Modo permissivo (Permissive): Os eventos de nega莽茫o s茫o registrados, mas n茫o aplicados.
2. Modo impondo (Enforcing): Os eventos de nega莽茫o s茫o registrados e aplicados.

::: warning AVISO
Os sistemas Android modernos dependem fortemente do SELinux para garantir a seguran莽a geral do sistema. 脡 altamente recomend谩vel n茫o usar nenhum sistema personalizado executado em "Modo permissivo", pois ele n茫o oferece vantagens significativas em rela莽茫o a um sistema completamente aberto.
:::

Explicar o conceito completo do SELinux 茅 complexo e est谩 al茅m do objetivo deste documento. Recomenda-se primeiro entender seu funcionamento atrav茅s dos seguintes recursos:

1. [Wikip茅dia](https://en.wikipedia.org/wiki/Security-Enhanced_Linux)
2. [Red Hat: O que 茅 SELinux?](https://www.redhat.com/pt-br/topics/linux/what-is-selinux)
3. [ArchLinux: SELinux](https://wiki.archlinux.org/title/SELinux)

O Perfil root do SxKernelSU permite a personaliza莽茫o do contexto SELinux do processo root ap贸s a execu莽茫o de `su`. Regras espec铆ficas de controle de acesso podem ser definidas para este contexto, possibilitando um controle refinado sobre os privil茅gios root.

Em cen谩rios t铆picos, quando um app executa `su`, ele alterna o processo para um dom铆nio SELinux com **acesso irrestrito**, como `u:r:ksu:s0`. Atrav茅s do Perfil root, esse dom铆nio pode ser mudado para um dom铆nio personalizado, como `u:r:app1:s0`, e uma s茅rie de regras podem ser definidas para esse dom铆nio:

```sh
type app1
enforce app1
typeattribute app1 mlstrustedsubject
allow app1 * * *
```

Observe que a regra `allow app1 * * *` 茅 usada apenas para fins de demonstra莽茫o. Na pr谩tica, esta regra n茫o deve ser usada extensivamente, pois n茫o difere muito do Modo permissivo.

### Escala莽茫o

Se a configura莽茫o do Perfil root n茫o estiver definida corretamente, poder谩 ocorrer um cen谩rio de escala莽茫o. As restri莽玫es impostas pelo Perfil root poder茫o falhar involuntariamente.

Por exemplo, se voc锚 conceder permiss茫o root a um usu谩rio ADB shell (que 茅 um caso comum) e, em seguida, conceder permiss茫o root a um app normal, mas configurar seu Perfil root com o UID 2000 (o UID do usu谩rio ADB shell), o app pode obter acesso root completo ao executar o comando `su` duas vezes:

1. A primeira execu莽茫o de `su` ser谩 sujeita ao Perfil do Aplicativo, e mudar谩 para o UID `2000` (ADB shell) em vez de `0` (root).
2. A segunda execu莽茫o de `su`, como o UID 茅 `2000` e voc锚 concedeu acesso root ao UID `2000` (ADB shell) na configura莽茫o, o app obter谩 privil茅gios root completo.

:::tip dica
Voc锚 pode habilitar a flag `NO_NEW_PRIVS` no seu `App Profile` personalizado.

Isso impede que o processo escape e eleve seus privil茅gios novamente usando o comando `su`.

No entanto, essa flag **apenas** impede que o SxKernelSU eleve os privil茅gios do processo; ele ainda pode escapar usando outros mecanismos do Linux.
Portanto, tenha muito cuidado com suas configura莽玫es de permiss茫o.
:::

## Perfil n茫o root

### Desmontar m贸dulos

O SxKernelSU fornece um mecanismo sem sistema para modificar parti莽玫es do sistema, obtido atrav茅s da montagem do OverlayFS. No entanto, alguns apps podem ser sens铆veis a esse comportamento. Nesse caso, podemos descarregar m贸dulos montados nesses apps configurando a op莽茫o "Desmontar m贸dulos".

Al茅m disso, a interface de configura莽玫es do gerenciador do SxKernelSU oferece a op莽茫o "Desmontar m贸dulos por padr茫o". Por padr茫o, essa op莽茫o est谩 **ativada**, o que significa que o SxKernelSU ou alguns m贸dulos descarregar茫o m贸dulos para este app, a menos que configura莽玫es adicionais sejam aplicadas. Se voc锚 n茫o preferir esta configura莽茫o ou se ela afetar determinados apps, voc锚 ter谩 as seguintes op莽玫es:

1. Manter a op莽茫o "Desmontar m贸dulos por padr茫o" ativada e desative individualmente a op莽茫o "Desmontar m贸dulos" no Perfil do Aplicativo para apps que exigem o carregamento do m贸dulo (agindo como uma "lista de permiss玫es").
2. Desativar a op莽茫o "Desmontar m贸dulos por padr茫o" e ativar individualmente a op莽茫o "Desmontar m贸dulos" no Perfil do Aplicativo para apps que exigem o descarregamento do m贸dulo (agindo como uma "lista negra").

::: info INFORMA脟脮ES
Em dispositivos que utilizam a vers茫o do kernel 5.10 ou superior, o kernel realiza qualquer a莽茫o adicional do descarregamento de m贸dulos. No entanto, para dispositivos que executam vers玫es do kernel abaixo de 5.10, essa op莽茫o 茅 apenas uma op莽茫o de configura莽茫o e o pr贸prio SxKernelSU n茫o executa nenhuma a莽茫o. Se voc锚 quiser usar a op莽茫o "Desmontar m贸dulos" em vers玫es do kernel anteriores a 5.10, 茅 necess谩rio portar a fun莽茫o `path_umount` em `fs/namespace.c`. Voc锚 pode obter mais informa莽玫es no final da p谩gina [Integra莽茫o para dispositivos n茫o-GKI](https://sxkernelsu1.netlify.app/pt_BR/guide/how-to-integrate-for-non-gki.html). Alguns m贸dulos, como ZygiskNext, tamb茅m podem usar essa op莽茫o para determinar se o descarregamento do m贸dulo 茅 necess谩rio.
:::
