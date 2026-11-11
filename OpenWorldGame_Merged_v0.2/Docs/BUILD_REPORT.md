# BUILD_REPORT — INCOMPLETE / BLOCKED

Date: 2026-11-11. Environment: Linux x86_64 container. The results below are the commands actually run in this merge session.

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
