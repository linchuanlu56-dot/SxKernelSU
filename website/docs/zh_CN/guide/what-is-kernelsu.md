# 什么是 SxKernelSU？ {#introduction}

SxKernelSU 是基于 [KernelSU](https://github.com/tiann/KernelSU) 的增强版 root 解决方案，工作在内核模式，直接在内核空间中为用户空间应用程序授予 root 权限，并增加了一系列高级安全功能。

## 功能 {#features}

SxKernelSU 的核心特性是它是**基于内核的**。SxKernelSU 运行在内核空间，因此可以提供前所未有的内核接口，例如：

- 在内核模式下为任何进程添加硬件断点
- 在任何进程的物理内存中访问而无人知晓
- 在内核空间拦截任何系统调用
- 隐藏 SELinux 强制状态，使 root 对检测应用不可见
- HOOK 系统调用注入自定义 init 脚本

### 🔒 SELinux 隐藏

SxKernelSU 可以隐藏 `enforcing` 模式下的 SELinux 状态，使用户态应用无法检测 root 状态。它通过以下方式实现：
- 伪造 SELinux 状态页，返回 fake `enforcing` 信息
- 拦截 SELinux 上下文写入操作，使用备份策略响应
- 对 6.6+ 内核完整支持

### 📦 Metamodule 模块系统

SxKernelSU 提供 [metamodule 系统](metamodule.md)，这是一个可插拔的模块管理架构。与将挂载逻辑内置到核心的传统 root 方案不同，SxKernelSU 将此功能委托给 metamodule。您可以安装 [meta-overlayfs](https://github.com/linchuanlu56-dot/SxKernelSU/tree/main/userspace/meta-overlayfs) 等 metamodule，以提供对 `/system` 分区和其他分区的无系统修改。

### 🛡️ App Profile

通过 [App Profile](app-profile.md) 精细化控制每个应用的 root 权限——包括 `uid`、`gid`、`groups`、`capabilities` 和 SELinux 规则，真正实现"把 root 关进笼子里"。

### 🔐 安全模式

启动时按 **音量减键 3 次** 进入安全模式，禁用所有 SxKernelSU 功能以排查启动问题。

### ⚙️ ADB Root 支持

内置 adbd root 支持，可在 adb shell 中直接获得 root 权限。

## 如何使用 {#how-to-use}

请参考: [安装](installation)

## 如何构建 {#how-to-build}

请参考: [如何构建](how-to-build)

## 与 KernelSU 的关系

SxKernelSU 是 KernelSU 的增强分支，在保持与上游兼容的同时，增加了 SELinux 隐藏、init.rc 注入、安全模式等高级功能。详细信息请参考 [SxKernelSU 与 KernelSU 的差异](difference-with-kernelsu)。

## 讨论 {#discussion}

- Telegram: [@SxKernelSU](https://t.me/SxKernelSU)
