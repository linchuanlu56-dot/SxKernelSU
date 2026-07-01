# Differences Between SxKernelSU and KernelSU

SxKernelSU is an enhanced fork of [KernelSU](https://github.com/tiann/KernelSU). While maintaining compatibility with the upstream KernelSU core framework, SxKernelSU adds a series of advanced features and security enhancements.

## Similarities

- **Kernel-level root**: Both are kernel module-based root solutions working via LKM
- **Module system**: Module format, installation directory (`/data/adb/modules`), and script lifecycle are fully compatible
- **App Profile**: Both support fine-grained root permission control via App Profiles
- **Allowlist mechanism**: Only authorized apps can obtain root permissions
- **Manager app**: Uses the same manager application framework

## SxKernelSU Exclusive Features

### 1. 🔒 SELinux Hiding
SxKernelSU implements a complete SELinux hiding mechanism:
- **Fake SELinux status page**: Returns a fake "enforcing" status to userspace apps, making root invisible to SELinux-detecting apps
- **HOOK write_op**: Intercepts `SEL_CONTEXT` and `SEL_ACCESS` writes, responding with a backup policy
- **Backup policy**: Maintains a backup SELinux policy for policy queries in hidden mode
- **Full 6.6+ kernel support**: Complete reimplementation of `security_context_to_sid_with_policy`, `security_compute_av_user_with_policy`, and other core functions

### 2. 🚀 Init.rc Injection
When init reads `/system/etc/init/hw/init.rc`, SxKernelSU dynamically injects custom init scripts by HOOKing the `read` and `fstat` syscalls:
- Supports execution during `post-fs-data`, `boot_completed`, and other stages
- Supports loading additional rc scripts from `/metadata/[watchdog/]ksu/modules.rc`
- Can be disabled via the `norc` module parameter

### 3. 🛠 Enhanced IOCTL Interface
The kernel-userspace communication interface has been significantly extended:

| Feature | Description |
|---------|-------------|
| Process Marking | Mark/unmark/refresh per-process root permissions |
| Runtime Feature Toggle | Enable/disable kernel features at runtime (e.g., SELinux hiding) |
| Umount List Management | Add/remove/wipe mount points to be unmounted |
| File Descriptor Wrapping | Get wrapped FDs for stealthy operations |
| Kernel Safe Mode Detection | Query whether the kernel is in safe mode |
| Set SEPolicy | Dynamically set SELinux policies |

### 4. ⚙ ADB Root Support
Built-in adbd root support, allowing direct root access in adb shell for development and debugging.

### 5. 🔐 Safe Mode
Press the **Volume Down key 3 times** during boot to enter safe mode:
- All SxKernelSU functions are disabled
- No root permissions are granted
- Useful for troubleshooting boot issues caused by root

### 6. 🧩 Kernel Umount Tracking
The `kernel_umount` module tracks umount operations in the kernel, enhancing control over filesystem hiding.

### 7. 🔄 SU Compatibility Layer
`susompat` provides improved su command compatibility, ensuring more apps work correctly.

### 8. 📝 Enhanced SU Logging
The `sulog` system has been significantly enhanced, providing more detailed su usage audit logs.

### 9. 🐛 Debugging and Module Parameters
- **`allow_shell`**: Allow shell users to get root (for debugging)
- **`norc`**: Disable custom rc injection
- **`CONFIG_SKS_DEBUG`**: Compile-time debug mode with verbose logging

## Naming Differences

| KernelSU | SxKernelSU |
|----------|------------|
| `ksud` daemon | `sksud` daemon |
| `ksuinit` | `sksuinit` |
| `kernel_su` module | `sxkernelsu` module |
| `/data/adb/ksu/` | `/data/adb/sksd/` |
| `ksu_*` functions | `sksu_*` functions |

## Kernel Compatibility

SxKernelSU maintains the same level of kernel compatibility as upstream:
- GKI 2.0 devices (kernel 5.10+)
- Older kernels (4.14+) require manual compilation
- Supports `arm64-v8a` and `x86_64` architectures

## Update Strategy

SxKernelSU stays in sync with upstream by regularly merging KernelSU updates while maintaining its own enhanced feature set. This means you get both upstream stability fixes and SxKernelSU's additional features.
