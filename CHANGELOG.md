# Changelog

[**English**](CHANGELOG.md) | [**简体中文**](CHANGELOG_CN.md)

All notable changes to SxKernelSU will be documented in this file.

## [v2.0.0] — 2026-07-01 — Ghost Release

### 🧠 Ghost Architecture — 6 Layers of Root Hiding

**Layer 1: Module Invisibility**
- Kernel module delisted from kernel module chain (`list_del_init`)
- Module name scrambled in memory (can't be found via `lsmod`)
- Sysfs entry removed (`/sys/module/kernelsu/` invisible)

**Layer 2: Hook Protection**
- 30-second timer validates syscall hook integrity
- Hooks automatically restored if overwritten by detection tools
- Kprobes hidden from debugfs

**Layer 3: Process & FD Hiding**
- `/proc/self/maps` output filtered per-uid (hides ksud, libkernelsu, libsu)
- `/proc/self/mountinfo` filtered per-uid (hides KSU mountpoints)
- KSU-related PIDs hidden from process listings
- ksud process name spoofed to `init_sched`

**Layer 4: Timing Jitter**
- `clock_gettime` results randomly jittered by ±5ms for protected apps
- Breaks timing-based root detection without affecting normal operation

**Layer 5: Memory Protection**
- Critical strings XOR-encrypted in kernel memory
- Kernel symbol lookups for ksu_/kernelsu symbols return zero

**Layer 6: Userspace Masquerading**
- ksud renames itself to `init_sched` at startup
- `/proc/self/comm` and cmdline sanitized

### ✨ Per-Task Virtual File System
Not mount namespaces — real VFS-layer filtering. Protected apps see `/data/adb/` where `ksu/` and `modules/` directories simply don't exist. Different processes see different directory contents based on UID.

### 👻 Shadow Syscall Table
The real `sys_call_table` is never modified. Instead, a shadow copy is created in randomized vmalloc memory. A kprobe on `el0_svc_common` redirects only protected processes to the shadow table. The real table stays pristine — any comparison check passes.

### 🧬 Biological Immune System
- Positive selection: root only granted to whitelisted apps
- Negative selection: scanning/probing triggers emergency shutdown
- Threat memory: remembers detected threats, responds faster next time

---

## [v1.5.0] — 2026-07-01 — Anime Release

### 🎨 Anime Theme Overhaul
- Default theme color changed to Sakura Pink (`#FF6B9D`) with Vibrant palette
- `SparkleBackground` — animated floating stars on home & install screens
- Dual-mode anime wallpapers (light/dark) at 25% opacity
- All cards (StatusCard, InfoCard, DonateCard, LearnMoreCard) tinted in pink, candy, and lavender
- Staggered fade-in + slide-up card entrance animations
- Bouncy page transitions with `Spring.DampingRatioMediumBouncy`
- Custom SxKernelSU vector adaptive icon
- Install page: all 4 cards anime-colored + sparkle effects

### 📱 Manager Enhancements
- Package renamed from `me.weishu.kernelsu` to `com.sxkernelsu`
- Full JNI function name update for new package
- `BootCompletedReceiver` references fixed
- `SksCli` class name normalized
- Uninstall dialog: TEMPORARY option uncommented, TODO removed

---

## [v1.4.0] — 2026-06-30 — Lock & Load

### 🔒 Atomic Module Installation
- Modules now installed atomically with automatic backup & rollback
- If installation fails, the previous version is fully restored
- No more bricked devices from partial module installation

### 🛠️ Debugfs Interface
- `/sys/kernel/debug/ksu/version` — KSU version info
- `/sys/kernel/debug/ksu/hooks` — Hook status

### 🐳 Docker Dev Environment
- `Dockerfile` with full Android/Rust cross-compilation toolchain
- `docker-compose.yml` for one-command setup

### 🚀 Process-Level Hiding Infrastructure
- Per-app hide profiles (`/proc/self/maps`, mountinfo, PID filtering)
- File-system-level hiding (vfs_stat/vfs_readdir interception)
- Foundation for Ghost architecture

---

## [v1.3.0] — 2026-06-30 — Stability Release

### 🛡️ Kernel Stability
- `selinux_hide.c`: All 13 `BUG_ON()/BUG()` calls replaced with `WARN_ON()/WARN()`. SELinux policy anomalies no longer cause kernel panics
- `ksud_integration.c`: Module exit sequence reordered — syscall hooks stop first, then kprobes, then resources. Prevents use-after-free on module unload
- `init_event.rs`: Module post-fs-data scripts now have a 60-second timeout. Hung scripts are killed automatically, boot no longer blocks

### 🔧 Symbol Resolution
- Added `/proc/kallsyms` fallback parser when `kallsyms_on_each_symbol()` is unavailable (< 5.19 or hardened builds)
- Dramatically improves compatibility across kernel versions

### ⚙️ Workqueue Migration
- Allowlist persistence migrated from `task_work_add(init)` to system workqueue
- Eliminates potential deadlock on init process, multiple requests coalesced

### 📦 Build System
- CI: ccache caching for LKM builds across all 7 KMI targets
- `justfile`: Added `dev-setup`, `build-kmi`, `clippy-fix`, `clean-all`

---

## [v1.2.0] — 2026-06-30 — Initial Fork

- Forked from KernelSU
- All original KernelSU v1.0.2 features preserved
- Project infrastructure set up for SxKernelSU
