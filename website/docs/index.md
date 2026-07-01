---
layout: home
title: Home

hero:
  name: SxKernelSU
  text: A kernel-based root solution for Android
  tagline: ""
  image:
    src: /logo.png
    alt: SxKernelSU
  actions:
    - theme: brand
      text: Get started
      link: /guide/what-is-sxkernelsu
    - theme: alt
      text: View on GitHub
      link: https://github.com/linchuanlu56-dot/SxKernelSU

features:
  - title: Kernel-based
    details: SxKernelSU runs inside the Linux kernel, giving it more control over userspace apps.
  - title: Root access control
    details: Only permitted apps can access or see su; all other apps remain unaware of it.
  - title: Customizable root privileges
    details: Customize su's uid, gid, groups, capabilities, and SELinux rules — lock up root power in a cage.
  - title: SELinux Hiding
    details: Hide SELinux enforcing status from userspace apps, bypassing root detection mechanisms.
  - title: Metamodule system
    details: Pluggable module infrastructure for systemless modifications. Install meta-overlayfs to enable module mounting.
  - title: Safe Mode
    details: Press Volume Down 3 times during boot to enter safe mode, disabling all root functions.
