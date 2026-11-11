#!/usr/bin/env python3
"""Read-only environment probe. It does not install or modify anything."""
from __future__ import annotations
import os
import shutil
from pathlib import Path


def locate_engine() -> list[Path]:
    candidates = []
    for key in ("UE_5_8_ROOT", "UNREAL_ENGINE_ROOT", "UE_ROOT"):
        value = os.environ.get(key)
        if value:
            candidates.append(Path(value))
    candidates.extend([
        Path("C:/Program Files/Epic Games/UE_5.8"),
        Path("C:/Program Files/Epic Games/UE_5.8/Engine"),
        Path("/opt/UnrealEngine/Engine"),
        Path("/UnrealEngine/Engine"),
    ])
    return [p for p in candidates if p.exists()]


def main() -> int:
    print("OWG read-only build environment check")
    print(f"OS: {os.name}")
    print(f"JAVA_HOME: {os.environ.get('JAVA_HOME', 'not set')}")
    print(f"ANDROID_HOME: {os.environ.get('ANDROID_HOME', 'not set')}")
    print(f"ANDROID_SDK_ROOT: {os.environ.get('ANDROID_SDK_ROOT', 'not set')}")
    for tool in ("UnrealEditor", "UnrealEditor-Cmd", "UnrealBuildTool", "adb", "java", "clang++", "cl"):
        print(f"{tool}: {shutil.which(tool) or 'not found on PATH'}")
    engines = locate_engine()
    print("Engine roots:")
    if engines:
        for engine in engines:
            print(f"  {engine}")
    else:
        print("  Unreal Engine root not detected")
    print("This probe does not validate Android packaging or a device.")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
