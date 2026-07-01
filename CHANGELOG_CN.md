# 更新日志

## [v2.0.0] — 2026-07-01 — Ghost 版本

### 🧠 Ghost 架构 — 6 层 Root 隐藏

**第一层：模块完全隐身**
- 内核模块从模块链摘除（`list_del_init`）
- 模块名在内存中擦除（`lsmod` 找不到）
- sysfs 条目删除（`/sys/module/kernelsu/` 不可见）

**第二层：Hook 保护**
- 30 秒定时器校验 syscall hook 完整性
- Hook 被覆盖时自动恢复
- Kprobe 从 debugfs 隐藏

**第三层：进程 & FD 隐藏**
- `/proc/self/maps` per-uid 过滤（隐藏 ksud、libkernelsu、libsu）
- `/proc/self/mountinfo` per-uid 过滤（隐藏 KSU 挂载点）
- KSU 相关 PID 从进程列表隐藏
- ksud 进程名伪装为 `init_sched`

**第四层：时序抖动**
- `clock_gettime` 结果对受保护应用加入 ±5ms 随机抖动
- 打破时序检测，不影响正常功能

**第五层：内存保护**
- 关键字符串在内存中 XOR 加密
- `kallsyms` 对 ksu_/kernelsu 符号返回 0

**第六层：用户态伪装**
- ksud 启动后重命名为 `init_sched`
- `/proc/self/comm` 和 cmdline 清理

### ✨ Per-Task VFS（独创）
不是 mount namespace — 真正的 VFS 层过滤。受保护应用看 `/data/adb/` 时，`ksu/` 和 `modules/` 目录根本不存在。不同进程根据 UID 看到不同的目录内容。

### 👻 影子系统调用表（独创）
真正的 `sys_call_table` 从未被修改。在随机化的 vmalloc 内存中创建影子表，用 kprobe on `el0_svc_common` 只重定向受保护进程到影子表。真正的表完全纯净——任何对比检测都通过。

### 🧬 仿生免疫系统（独创）
- 正选择：仅白名单应用可获取 root
- 负选择：扫描/探测触发紧急停机
- 威胁记忆：记住检测过的威胁，下次更快响应

## [v1.5.0] — 2026-07-01 — 二次元版本

### 🎨 二次元主题改造
- 默认主题色改为樱花粉（`#FF6B9D`），Vibrant 风格
- `SparkleBackground` — 主页和安装页浮动星星动画
- 双模式二次元壁纸（亮/暗）25% 透明度
- 所有卡片（StatusCard、InfoCard、DonateCard、LearnMoreCard）粉色/薰衣草紫配色
- 依次弹跳淡入卡片入场动画
- 弹跳页面切换（`Spring.DampingRatioMediumBouncy`）
- 自定义 SxKernelSU Vector 自适应图标
- 安装页：全部 4 张卡片二次元配色 + 星星效果

### 📱 Manager 增强
- 包名从 `me.weishu.kernelsu` 改为 `com.sxkernelsu`
- JNI 函数名全部更新适配新包名
- `BootCompletedReceiver` 引用修复
- `SksCli` 类名规范化
- 卸载对话框：TEMPORARY 选项启用，TODO 移除

## [v1.4.0] — 2026-06-30 — 锁定与加载

### 🔒 原子模块安装
- 模块安装前自动备份旧版，失败时自动还原
- 不再有模块安装中断导致设备变砖的问题

### 🛠️ Debugfs 接口
- `/sys/kernel/debug/ksu/version` — KSU 版本信息
- `/sys/kernel/debug/ksu/hooks` — Hook 状态

### 🐳 Docker 开发环境
- 完整的 Android/Rust 交叉编译工具链 Dockerfile
- docker-compose.yml 一键启动

### 🚀 进程级隐藏基础设施
- 每应用隐藏配置
- 文件系统级隐藏（vfs_stat/vfs_readdir 拦截）
- 为 Ghost 架构奠定基础

## [v1.3.0] — 2026-06-30 — 稳定版本

### 🛡️ 内核稳定性
- `selinux_hide.c`：全部 13 处 `BUG_ON()/BUG()` 替换为 `WARN_ON()/WARN()`。SELinux 策略异常不再导致内核崩溃
- `ksud_integration.c`：模块卸载顺序重新编排 → syscall hook 先停，然后 kprobe，最后释放资源。防止卸载时的 use-after-free
- `init_event.rs`：模块 post-fs-data 脚本现在有 60 秒超时。卡住的脚本会被自动 kill，启动不再阻塞

### 🔧 符号解析
- 添加 `/proc/kallsyms` 回退解析器（当 `kallsyms_on_each_symbol()` 不可用时）
- 大幅提升跨内核版本的兼容性

### ⚙️ Workqueue 迁移
- 白名单持久化从 `task_work_add(init)` 迁移到系统 workqueue
- 消除 init 进程上的潜在死锁，多次请求合并执行

### 📦 构建系统
- CI：全部 7 个 KMI 目标的 LKM 构建添加 ccache 缓存
- `justfile`：添加 `dev-setup`、`build-kmi`、`clippy-fix`、`clean-all`

## [v1.2.0] — 2026-06-30 — 初始 Fork

- 从 KernelSU Fork
- 保留所有原始 KernelSU v1.0.2 功能
- 建立 SxKernelSU 项目基础设施
