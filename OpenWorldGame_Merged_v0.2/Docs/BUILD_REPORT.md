# BUILD_REPORT — INCOMPLETE / BLOCKED

Date: 2026-11-11. Environment: Linux x86_64 container. The first tables record the initial merge-session inventory; the later Codespaces setup section supersedes Android SDK/NDK availability, but not the blocked Unreal build or APK status.

## Host-side commands

| Target | Exact command | Result | Status |
|---|---|---|---|
| C++ portable core | `bash Scripts/run_host_tests.sh` (script compiles with `g++ -std=c++17 -Wall -Wextra -Werror -O1`) | compile exit 0; run exit 0; 42/42 tests | PASS |
| C++ sanitizers | `g++ -std=c++17 -Wall -Wextra -Werror -O1 -fsanitize=address,undefined -fno-omit-frame-pointer Tests/HostTests/test_main.cpp -o /tmp/owg_host_tests_sanitized` then `ASAN_OPTIONS=detect_leaks=1 /tmp/owg_host_tests_sanitized` | 42/42 tests; no sanitizer report; exit 0 | PASS |
| Python integrity tests | `python3 -m unittest discover -s Tests -v` | 14 tests; exit 0 | PASS |
| Project static validator | `python3 Scripts/validate_project.py` | `STATIC INTEGRITY: PASS`; exit 0 | PASS |
| File/dependency audit | `python3 Scripts/audit_files.py` | 27 pass / 0 fail; exit 0 | PASS |

Logs: `Docs/logs/host_tests.log`, `Docs/logs/host_tests_sanitized.log`, `Docs/PYTHON_TEST_RESULTS.txt`, `Docs/STATIC_VALIDATION_RESULTS.txt`, and `Docs/logs/file_audit.log`.

## Unreal / Android targets

| Target | Result | Status |
|---|---|---|
| UnrealHeaderTool / UnrealBuildTool | Not run; Unreal Engine 5.8 is not installed | BLOCKED / NOT RUN |
| Unreal Editor / Game compile | Not run | BLOCKED / NOT RUN |
| Android arm64 Shipping APK | Not built; Unreal and Android SDK/NDK are unavailable | BLOCKED / NOT BUILT |
| ADB install and phone launch | Not run; ADB and physical Android device unavailable | BLOCKED / NOT RUN |
| Airplane/offline gameplay | Not run | BLOCKED / NOT RUN |
| CPU/GPU/RAM/FPS/thermal profiling | Not run | BLOCKED / NOT RUN |

## Toolchain recorded

- g++ 14.2.0; Python 3.13.5; OpenJDK 21.0.12.1; Git 2.47.3.
- `UnrealEditor`, `UnrealBuildTool`, `adb`, and `gradle` were not found in PATH.
- Android SDK/NDK were not detected; no physical Android device was available.

The C++ host tests exercise only engine-independent helper code. They do not prove UnrealHeaderTool compatibility, an Unreal build, actual gameplay, an APK, or device performance. The project remains an incomplete source prototype.

## Follow-up execution — 2026-11-11

From `OpenWorldGame_Merged_v0.2`, the following were rerun after the frame-cap UI change:

| Target | Exact command | Result | Status |
|---|---|---|---|
| Static project validator | `python3 Scripts/validate_project.py` | `STATIC INTEGRITY: PASS`; engine compile reported NOT RUN | PASS (static check only) |
| File/dependency audit | `python3 Scripts/audit_files.py` | 27 pass / 0 fail | PASS |
| Python integrity tests | `python3 -m unittest discover -s Tests -v` | 15 tests; exit 0 | PASS |
| C++ portable core | `bash Scripts/run_host_tests.sh` | 42/42; exit 0 | PASS |
| Unreal C++/UHT | Not run | Unreal Engine/UBT unavailable | BLOCKED / NOT RUN |
| Android APK and device tests | Not run | Android SDK/NDK, ADB, and physical device unavailable | BLOCKED / NOT RUN |

The current environment reports g++ 13.3.0, Java 21, and Gradle available. This differs from the earlier handoff's g++ 14.2.0 / Gradle-not-found inventory; the direct tool checks in this follow-up are the current evidence. The new Frame Pacer integration has not been compiled by Unreal and its Android display-mode behavior remains unverified.

## Codespaces toolchain setup — 2026-11-11

- Working directory: `/workspaces/MADA-/OpenWorldGame_Merged_v0.2`; project base commit `ced4a05`; tool installation path `/workspaces/.toolchains` on the persistent `/workspaces` mount.
- Resource inspection: Ubuntu 24.04.5, kernel `6.8.0-1070-azure`, glibc 2.39, AMD EPYC 7763 with 2 vCPU, 7.8 GiB RAM (5.1 GiB available initially), persistent filesystem 32 GiB with 19 GiB free before install and 16 GiB afterward.
- Installed Microsoft OpenJDK 21.0.3+9 and Google Android SDK Platform 34/35, Build Tools 35.0.1, Platform Tools 37.0.1, NDK r27c (`27.2.12479018`), and CLI tools. Official JDK SHA-256 verified before extraction; Google command-line tools SHA-1 verified. Exact paths, hashes, sizes, and environment exports are in `Docs/BUILD_ENVIRONMENT_REPORT.txt` and `/workspaces/.toolchains/env.sh`.
- `source /workspaces/.toolchains/env.sh`; Java/Javac, ADB, AAPT2, NDK revision, and ARM64 Clang were verified. A stdin-only `aarch64-linux-android26-clang -fsyntax-only` probe passed. `adb devices -l` returned no physical device.
- Unreal remains uninstalled: Epic documentation recommends Ubuntu 22.04, 32 GiB RAM, and an 8 GiB GPU; this host is Ubuntu 24.04, 7.8 GiB RAM, 2 vCPU, and exposes only Hyper-V DRM. `gh repo view EpicGames/UnrealEngine` returned 404-style “Could not resolve to a Repository”; no official authenticated Installed Build was accessible. No engine download or source build was started.
- UE UHT/UBT, Android cook/package, APK, device install, gameplay, and performance tests remain BLOCKED/NOT RUN. No release artifact or checksum exists.

## Save-protection follow-up — 2026-11-11

- Base commit: `ced4a05`; working directory: `/workspaces/MADA-/OpenWorldGame_Merged_v0.2`.
- `python3 Scripts/validate_project.py`: PASS (static only); `python3 Scripts/audit_files.py`: PASS 27/27; `python3 -m unittest discover -s Tests -v`: PASS 16/16; `bash Scripts/run_host_tests.sh`: PASS 42/42; `git diff --check`: PASS.
- No Unreal Editor/Build Tool, Android SDK/NDK, ADB, physical device, `.umap`, `.uasset`, or APK was available. UHT/UBT, package, install, and runtime validation remain BLOCKED/NOT RUN.
- Current g++ is 13.3.0; Java 21 and Gradle are present. No APK size, ABI artifact, release signature, or SHA-256 exists because no APK was produced.

## Validator accuracy follow-up — 2026-11-11

- Updated `Scripts/validate_project.py` to detect SDK Platform 35 and the NDK through the active environment while independently reporting APK packaging as NOT BUILT.
- Focused Python test suite: PASS 17/17; static validator: PASS. This only tests reporting/integrity and does not run Unreal.
