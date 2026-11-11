# MADA | مَدَى — Android-first Unreal starter

**BEYOND THE HORIZON**

Unreal Engine 5.8 C++ source prototype for an original offline single-player Android driving and exploration game. The official in-game identity is MADA | مَدَى, with developer credit «تطوير: عبدالله لازم».

**Status: INCOMPLETE / SOURCE STARTER. No APK has been built and Unreal gameplay has not been run.**

Start with `Docs/START_HERE_AR.md`. Read the complete specification at `Docs/MASTER_SPEC_AR_v0.3.txt` and agent rules in `AGENTS.md`.

Run the available host-side checks from the project root:

```bash
python3 Scripts/validate_project.py
python3 Scripts/audit_files.py
python3 -m unittest discover -s Tests -v
bash Scripts/run_host_tests.sh
```

These checks do not replace a real Unreal Engine 5.8 build or Android device validation.

In the current Codespace, Android SDK/NDK/JDK are installed outside the repository at `/workspaces/.toolchains`. Run `source /workspaces/.toolchains/env.sh` to configure the current terminal. Unreal Engine 5.8 is not installed, so no APK build is currently possible; see `Docs/BUILD_ENVIRONMENT_REPORT.txt`.
