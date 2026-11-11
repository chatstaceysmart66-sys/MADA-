# Known Issues — merged v0.2

- Unreal Engine 5.8 has not been used to run UHT, compile, open the Editor, or play the prototype. C++/UHT/API compatibility remains unverified.
- No Android APK exists. SDK/NDK/JDK are installed under the persistent Codespaces toolchain path, but Unreal/UBT is unavailable and no physical Android test device is attached.
- The vehicle is an experimental rigid-body pawn, not a configured Chaos Vehicle.
- The greybox is generated from engine primitives. There are no licensed final vehicle/character/environment assets, animation sets, sounds, or music.
- No actual city/rural open world, World Partition streaming, HLOD, NPCs, traffic, missions, weather, or day/night simulation is implemented.
- Touch controls and safe-area behavior have not been verified on devices.
- Frame-cap options are filtered through `FPlatformRHIFramePacer::SupportsFramePace`, but the Unreal integration has not been compiled or verified against Android display modes.
- New Game now asks before replacing an existing save in source; the UMG flow has not been runtime-tested in Unreal.
- `OWSaveData.h` is a portable test/migration model; the game currently persists state through Unreal `USaveGame`.
- No external assets are included, so asset-license review is pending for any future imported art/audio.
