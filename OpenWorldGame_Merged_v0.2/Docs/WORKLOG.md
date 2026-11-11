# Worklog — merged v0.2
## 2026-11-11 — prepare Android tools in Codespaces

- Inspected the active Codespace before downloading: persistent `/workspaces` was 32 GiB total with 19 GiB free; 2 vCPU, 7.8 GiB RAM, Ubuntu 24.04.5, glibc 2.39, and Hyper-V DRM with no verified 3D GPU.
- Installed Microsoft JDK 21.0.3 and Google Android SDK/NDK in `/workspaces/.toolchains`, outside Git. Installed SDK 34/35, Build Tools 35.0.1, Platform Tools 37.0.1, and NDK r27c. Post-install free space: 16 GiB; toolchain directory: 3.3 GiB.
- Verified JDK SHA-256, Google command-line tools SHA-1, Java/Javac, ADB, AAPT2, NDK revision, ARM64 Clang syntax probe; no attached device found.
- Added `/workspaces/.toolchains/env.sh`; source it to set Java/SDK/NDK variables in a terminal.
- UE remains blocked: Unreal tools absent, Epic source repo inaccessible to the current GitHub identity, current host below Epic Linux quickstart recommendations. No engine download, APK, or device test was attempted.

## 2026-11-11 — protect existing progress on New Game

- Base state verified at `ced4a05` on `main`; working tree was clean before this task. No commit or push was performed in this execution.
- Added `UOWGGameInstance::HasProgressSave()` and an Arabic UMG confirmation panel when a save slot exists. Confirm starts the existing reset/travel flow; Cancel returns to the menu without calling it; a missing save starts directly.
- Added a Python integration test for save detection, confirmation visibility, and Confirm/Cancel button bindings.
- Validation from `OpenWorldGame_Merged_v0.2`: `python3 Scripts/validate_project.py` PASS; `python3 Scripts/audit_files.py` PASS 27/27; `python3 -m unittest discover -s Tests -v` PASS 16/16; `bash Scripts/run_host_tests.sh` PASS 42/42; `git diff --check` PASS.
- Unreal Engine/UBT, Android SDK/NDK, ADB, physical device, and Blender are unavailable. UHT, PIE interaction, Android APK, and device checks remain BLOCKED/NOT RUN. No `.umap`, `.uasset`, APK, or source archive was created.
- Next exact action: on an Unreal Engine 5.8 workstation, compile the Editor target, then test New Game with a valid save (Cancel preserves; Confirm replaces), no save, and failed level travel before expanding to vehicle/world work.

## 2026-11-11 — official MADA branding and startup screen

- Set the product identity to MADA | مَدَى with the tagline BEYOND THE HORIZON; kept the OpenWorldGame module and package ID stable for compatibility.
- Added centralized UTF-8 brand constants and a text startup screen with developer credit and a working continue-to-menu button.
- Set the Unreal project name and Android display name to MADA.
- Synced the displayed frame-cap option with the active game-user setting at startup and after loading a save.
- Added Escape/touch Pause menu with Resume and Save, and split New Game from Continue. New Game removes the old slot, reloads the current level, then starts play; Continue loads a valid save or falls back to New Game.
- Persisted the selected frame limit through `UGameUserSettings::SaveSettings()`.
- Added host-side coverage; Python integrity tests pass 14/14 and C++ host tests pass 42/42.
- Unreal compilation, Android packaging, and device validation remain NOT RUN because the required tools and device are unavailable.

## 2026-11-11 — source merge and hardening

- Examined the full Arabic master specification and both ZIP archives before combining them.
- Used `OWG_Android_Starter_v0.1` as the project base because it contains the actual Unreal gameplay source prototype.
- Added the scaffold's engine-independent Core helpers, portable save migration model, C++ host tests, file audit and host-test runner.
- Kept a single standard module entry point at `Source/OpenWorldGame/OpenWorldGame.cpp`; removed duplicate module registration from the Private directory.
- Connected shared developer-credit, speed conversion and measured-frame-time helpers to runtime source.
- Added vehicle enter checks (seat, distance, speed), four candidate exit locations and possession-failure recovery.
- Hardened the portable save parser against checksum suffixes, duplicate fields, non-finite coordinates, invalid audio volume and unsupported frame caps.
- Merged Android arm64/Vulkan/offline configuration without losing GameMode/GameInstance startup configuration.
- Added combined documentation, manifests, device matrix, workflow and hash manifest.
- Ran host-side Python/C++ tests and static file audits; exact outcomes recorded in the reports/logs.
- Unreal Engine 5.8, Android SDK/NDK, ADB and a physical Android device were unavailable; no Unreal build or APK was produced.
