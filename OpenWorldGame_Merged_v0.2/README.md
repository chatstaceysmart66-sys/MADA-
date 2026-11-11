# OpenWorldGame — merged Android-first Unreal starter

Merged Unreal Engine 5.8 C++ source prototype for an original offline single-player Android open-world driving/exploration game.

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
