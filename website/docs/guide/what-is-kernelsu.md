# What is SxKernelSU?

SxKernelSU is an enhanced root solution for Android GKI devices, forked from [KernelSU](https://github.com/tiann/KernelSU). It works in kernel mode and grants root permission to userspace apps directly in kernel space, with a range of advanced security features.

## Features

The core feature of SxKernelSU is that it's **kernel-based**. SxKernelSU works in kernel space, enabling capabilities we never had before:

- Add hardware breakpoints to any process in kernel mode
- Access the physical memory of any process invisibly
- Intercept any system call within kernel space
- Hide SELinux enforcing status from detection-aware apps
- HOOK syscalls to inject custom init scripts

### 🔒 SELinux Hiding

SxKernelSU can hide the SELinux enforcing state from userspace apps. It implements this through:
- A fake SELinux status page that returns forged "enforcing" information
- Intercepting SELinux context write operations, responding with a backup policy
- Full support for 6.6+ kernels

### 📦 Metamodule System

SxKernelSU provides a [metamodule system](metamodule.md) — a pluggable architecture for module management. Unlike traditional root solutions that bake mounting logic into their core, SxKernelSU delegates this to metamodules. Install metamodules like [meta-overlayfs](https://github.com/linchuanlu56-dot/SxKernelSU/tree/main/userspace/meta-overlayfs) for systemless modifications to `/system` and other partitions.

### 🛡️ App Profile

Fine-grained control over each app's root permissions via [App Profile](app-profile.md) — including `uid`, `gid`, `groups`, `capabilities`, and SELinux rules.

### 🔐 Safe Mode

Press the **Volume Down key 3 times** during boot to enter safe mode, disabling all SxKernelSU functions.

### ⚙️ ADB Root Support

Built-in adbd root support for direct root access in adb shell.

## How to use SxKernelSU?

See [Installation](installation.md).

## How to build SxKernelSU?

See [How to build](how-to-build.md).

## Relationship with KernelSU

SxKernelSU is an enhanced fork of KernelSU that adds advanced features while maintaining upstream compatibility. See [Differences with KernelSU](difference-with-kernelsu) for details.

## Discussion

- Telegram: [@SxKernelSU](https://t.me/SxKernelSU)
