# SxKernelSU 与 KernelSU 的差异

SxKernelSU 是基于 [KernelSU](https://github.com/tiann/KernelSU) 的增强分支。在保持与上游 KernelSU 核心框架兼容的同时，SxKernelSU 增加了一系列高级功能和安全增强。

## 相同之处

- **内核级 root**：两者都是基于内核模块的 root 方案，通过 LKM 方式工作
- **模块系统**：模块格式、安装目录（`/data/adb/modules`）、脚本生命周期完全兼容
- **App Profile**：两者都支持通过 App Profile 精细化控制应用 root 权限
- **白名单机制**：只有被授权的应用才能获取 root 权限
- **Manager 应用**：使用相同的管理应用框架

## SxKernelSU 独有的功能

### 1. 🔒 SELinux 隐藏
SxKernelSU 实现了完整的 SELinux 隐藏机制：
- **伪造 SELinux 状态页**：给用户态应用返回一个伪造的 "enforcing" 状态，让 root 权限对检测 SELinux 的应用不可见
- **HOOK write_op**：拦截 `SEL_CONTEXT` 和 `SEL_ACCESS` 写入，使用备份策略响应
- **备份策略**：保存一份 SELinux 策略备份，用于在隐藏模式下进行策略查询
- **对 6.6+ 内核完整支持**：完整实现了 `security_context_to_sid_with_policy`、`security_compute_av_user_with_policy` 等核心函数

### 2. 🚀 Init.rc 注入
在 init 进程读取 `/system/etc/init/hw/init.rc` 时，通过 HOOK `read` 和 `fstat` 系统调用动态注入自定义 init 脚本：
- 支持 `post-fs-data`、`boot_completed` 等阶段执行
- 支持从 `/metadata/[watchdog/]ksu/modules.rc` 加载模块提供的额外 rc 脚本
- 支持通过模块参数 `norc` 禁用

### 3. 🛠 增强的 IOCTL 接口
大幅扩展了内核与用户态的通信能力：

| 功能 | 说明 |
|------|------|
| 进程标记（Mark） | 标记/取消标记/刷新进程级 root 权限 |
| Feature 运行时开关 | 运行时开启/关闭内核特性（如 SELinux 隐藏） |
| Umount 列表管理 | 添加/删除/清空需卸载的挂载点 |
| 文件描述符包装 | 获取包装后的 FD，用于隐藏操作 |
| 内核安全模式检测 | 查询内核是否处于安全模式 |
| 设置 SEPolicy | 动态设置 SELinux 策略 |

### 4. ⚙ ADB Root 支持
内置 adbd root 支持，可在 adb shell 中直接获得 root 权限，方便开发和调试。

### 5. 🔐 安全模式
启动时按 **音量减键 3 次** 可进入安全模式，在该模式下：
- 禁用所有 SxKernelSU 功能
- 不会授予任何 root 权限
- 可用于排查 root 导致的启动问题

### 6. 🧩 内核卸载跟踪
通过 `kernel_umount` 模块跟踪内核中的卸载操作，增强对文件系统隐藏的控制。

### 7. 🔄 SU 兼容层
`susompat` 提供更好的 su 命令兼容性，确保更多应用能正常工作。

### 8. 📝 增强的 SU 日志
`sulog` 系统经过大幅增强，提供更详细的 su 使用审计日志。

### 9. 🐛 调试与模块参数
- **`allow_shell`**：允许 shell 用户获得 root（调试用）
- **`norc`**：禁用自定义 rc 注入
- **`CONFIG_SKS_DEBUG`**：编译时调试模式，打印详细日志

## 命名差异

| KernelSU | SxKernelSU |
|----------|------------|
| `ksud` | `sksud` |
| `ksuinit` | `sksuinit` |
| `kernel_su` 模块 | `sxkernelsu` 模块 |
| `/data/adb/ksu/` | `/data/adb/sksd/` |
| `ksu_*` 函数 | `sksu_*` 函数 |

## 内核兼容性

SxKernelSU 在内核兼容性方面维持与上游相同的水平：
- GKI 2.0 设备（内核 5.10+）
- 旧内核（4.14+）需要手动编译
- 支持 `arm64-v8a` 和 `x86_64` 架构

## 更新策略

SxKernelSU 通过定期合并上游 KernelSU 的更新来保持同步，同时维护自己的增强功能集。这意味着您可以同时获得上游的稳定性修复和 SxKernelSU 的额外功能。
