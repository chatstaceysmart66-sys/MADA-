# Known Issues — merged v0.2

- Unreal Engine 5.8 has not been used to run UHT, compile, open the Editor, or play the prototype. C++/UHT/API compatibility remains unverified.
- No Android APK exists. Android SDK/NDK and a physical Android test device were unavailable in the merge environment.
- The vehicle is an experimental rigid-body pawn, not a configured Chaos Vehicle.
- The greybox is generated from engine primitives. There are no licensed final vehicle/character/environment assets, animation sets, sounds, or music.
- No actual city/rural open world, World Partition streaming, HLOD, NPCs, traffic, missions, weather, or day/night simulation is implemented.
- Touch controls and safe-area behavior have not been verified on devices.
- Runtime frame-cap selection is limited to Auto/30/40/60. The host helper for refresh-rate filtering is not proof of runtime device-mode detection.
- `OWSaveData.h` is a portable test/migration model; the game currently persists state through Unreal `USaveGame`.
- No external assets are included, so asset-license review is pending for any future imported art/audio.
