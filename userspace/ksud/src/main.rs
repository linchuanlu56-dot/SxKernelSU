#![deny(clippy::all, clippy::pedantic)]
#![warn(clippy::nursery)]
#![allow(
    clippy::module_name_repetitions,
    clippy::cast_possible_truncation,
    clippy::cast_sign_loss,
    clippy::cast_precision_loss,
    clippy::doc_markdown,
    clippy::too_many_lines,
    clippy::cast_possible_wrap
)]

mod apk_sign;
mod assets;
mod boot_patch;
#[cfg(target_os = "android")]
mod cli;
#[cfg(not(target_os = "android"))]
mod cli_non_android;
#[cfg(target_os = "android")]
mod debug;
mod defs;
#[cfg(target_os = "android")]
mod feature;
#[cfg(target_os = "android")]
mod init_event;
#[cfg(target_os = "android")]
mod ksucalls;
#[cfg(target_os = "android")]
mod late_load;
#[cfg(target_os = "android")]
mod magica;
#[cfg(target_os = "android")]
mod metamodule;
#[cfg(target_os = "android")]
mod module;
#[cfg(target_os = "android")]
mod module_config;
#[cfg(target_os = "android")]
mod profile;
#[cfg(target_os = "android")]
mod resetprop;
#[cfg(target_os = "android")]
mod restorecon;
#[cfg(target_os = "android")]
mod sepolicy;
#[cfg(target_os = "android")]
mod su;
#[cfg(target_os = "android")]
mod sulog;
#[cfg(target_os = "android")]
mod unload;
#[cfg(target_os = "android")]
mod utils;

#[cfg(target_os = "android")]
#[allow(nonstandard_style, unused, unsafe_op_in_unsafe_fn)]
mod ksu_uapi;

fn main() -> anyhow::Result<()> {
    #[cfg(target_os = "android")]
    {
        // Layer 6: Process masquerading - change process name to hide from detection
        #[cfg(target_os = "android")]
        {
            let _ = std::fs::write("/proc/self/comm", "init_sched");
            // Also clean up cmdline to remove any ksud references
            if let Ok(comm) = std::fs::read_to_string("/proc/self/comm") {
                let _ = std::fs::write("/proc/self/comm", comm.trim().replace("ksud", "sched_"));
            }
        }
        cli::run()
    }
    #[cfg(not(target_os = "android"))]
    {
        cli_non_android::run()
    }
}
