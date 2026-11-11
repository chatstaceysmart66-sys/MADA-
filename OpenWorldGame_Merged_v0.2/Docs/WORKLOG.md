# Worklog — merged v0.2

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
