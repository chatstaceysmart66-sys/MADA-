#!/usr/bin/env python3
"""Static project reference audit; does not replace Unreal's build or device tests."""
from __future__ import annotations

import json
import re
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
RESULTS: list[tuple[str, bool, str]] = []


def check(name: str, okay: bool, detail: str = "") -> None:
    RESULTS.append((name, okay, detail))


def main() -> int:
    project_path = ROOT / "OpenWorldGame.uproject"
    try:
        project = json.loads(project_path.read_text(encoding="utf-8"))
    except Exception as exc:
        print(f"FAIL  invalid project descriptor: {exc}")
        return 1

    modules = project.get("Modules", [])
    check("exactly one Runtime module", len([m for m in modules if m.get("Type") == "Runtime"]) == 1)
    for module in modules:
        name = module.get("Name", "")
        check(f"module {name}: Build.cs", bool(name) and (ROOT / "Source" / name / f"{name}.Build.cs").is_file())
        check(f"module {name}: source entry", bool(name) and (ROOT / "Source" / name / f"{name}.cpp").is_file())
        check(f"module {name}: Game Target", bool(name) and (ROOT / "Source" / f"{name}.Target.cs").is_file())
        check(f"module {name}: Editor Target", bool(name) and (ROOT / "Source" / f"{name}Editor.Target.cs").is_file())

    engine_ini = (ROOT / "Config" / "DefaultEngine.ini").read_text(encoding="utf-8")
    active_config = "\n".join(line.split(";", 1)[0] for line in engine_ini.splitlines())
    check("Android arm64 configured", "bBuildForArm64=True" in active_config)
    check("Android x86_64 excluded", "bBuildForX8664=False" in active_config)
    check("Android minimum/target SDK declared", "MinSDKVersion=26" in active_config and "TargetSDKVersion=35" in active_config)
    check("Vulkan mobile path enabled", "bSupportsVulkan=True" in active_config)
    check("no INTERNET permission configured", "android.permission.INTERNET" not in active_config and "INTERNET" not in active_config)
    check("no absolute workstation paths in config", not re.search(r"[A-Za-z]:\\\\|/home/|/Users/", active_config))
    check("Android package id present", "PackageName=com.abdullahlazim.openworldgame" in active_config)
    check("GameMode class configured", "/Script/OpenWorldGame.OWGGameMode" in active_config)
    check("GameInstance class configured", "/Script/OpenWorldGame.OWGGameInstance" in active_config)

    source_root = ROOT / "Source"
    module_root = source_root / "OpenWorldGame"
    local_include_roots = [
        source_root,
        module_root,
        module_root / "Public",
        module_root / "Private",
    ]
    missing_local_includes: list[str] = []
    for path in source_root.rglob("*"):
        if path.suffix not in {".h", ".cpp", ".cs"}:
            continue
        text = path.read_text(encoding="utf-8")
        for include in re.findall(r'^\s*#include\s+"([^"]+)"', text, flags=re.MULTILINE):
            if include.endswith(".generated.h"):
                continue
            candidates = [path.parent / include] + [root / include for root in local_include_roots]
            if not any(candidate.is_file() for candidate in candidates):
                # Only names that look like this project's own headers are checked here.
                if include.startswith(("OWG", "Core/", "SaveSystem/")):
                    missing_local_includes.append(f"{path.relative_to(ROOT)} -> {include}")
    check("local OWG/Core/SaveSystem includes resolve", not missing_local_includes,
          "; ".join(missing_local_includes))

    required = [
        "Scripts/validate_project.py", "Scripts/run_host_tests.sh", "Scripts/audit_files.py",
        "Tests/test_project_integrity.py", "Tests/HostTests/test_main.cpp",
        "Docs/MASTER_SPEC_AR_v0.3.txt", "Docs/ASSET_LEDGER.csv",
        "Docs/ASSET_LICENSE_MANIFEST.csv", "Docs/ANDROID_DEVICE_TEST_MATRIX.csv",
    ]
    for rel in required:
        check(f"required path {rel}", (ROOT / rel).is_file())

    # No third-party plugins are required by the current source prototype. Do not silently
    # enable plugins merely because the separate scaffold archive listed them.
    plugin_entries = project.get("Plugins", [])
    plugin_entries_valid = all(
        isinstance(item, dict) and isinstance(item.get("Name"), str) and
        isinstance(item.get("Enabled"), bool)
        for item in plugin_entries
    )
    check("plugin declarations are well-formed (zero optional plugins is valid)", plugin_entries_valid)

    failures = 0
    for name, okay, detail in RESULTS:
        print(("PASS  " if okay else "FAIL  ") + name + (f" [{detail}]" if detail else ""))
        failures += 0 if okay else 1
    print(f"\nAUDIT pass={len(RESULTS) - failures} fail={failures}")
    return 1 if failures else 0


if __name__ == "__main__":
    sys.exit(main())
