# Merge Decisions — v0.2

1. **Project base:** Keep the Android Starter's Unreal C++ gameplay prototype; the separate scaffold is mostly audit/test scaffolding and has no implemented player/vehicle/menu gameplay.
2. **Module registration:** Use one `IMPLEMENT_PRIMARY_GAME_MODULE` in the standard module-root source file. Do not keep both the starter's `Private/OpenWorldGame.cpp` and the scaffold's root-level file.
3. **Plugins:** Do not enable `ChaosVehiclesPlugin` or `EnhancedInput` just because the scaffold descriptor enabled them. The merged gameplay code still uses a rigid-body pawn and `InputCore`; plugin integration is a future implementation task.
4. **Android settings:** Keep the original GameMode/GameInstance map setup and merge arm64-only, Vulkan-enabled, Landscape, target/minimum SDK and empty extra-permission settings.
5. **Measured FPS:** Use a rolling window of real widget frame deltas. The displayed FPS is not the selected frame cap. Runtime screen-refresh probing and frame-pacer compatibility are still not implemented.
6. **Vehicle transitions:** Reuse tested pure logic to gate entry by distance/speed/seat availability and inspect four candidate exit spots. Actual physics/possession remains a Runtime test requirement.
7. **Save formats:** Preserve Unreal `USaveGame` for game state. Keep the portable `SaveData` serializer as an independent host-test/migration utility; do not pretend it is wired to runtime slots.
8. **Content:** Do not add art, maps, audio, music or other third-party assets. Both source archives contain no final art assets; only greybox Engine primitives are used.
9. **Git metadata:** Exclude the scaffold archive's `.git` directory; the user needs a clean source project, not an imported empty/incomplete repository database.
10. **Delivery status:** Host tests and static audits can pass while Unreal/Android remain blocked. Reports explicitly keep those statuses separate.
