import json
import subprocess
import sys
import unittest
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]


class ProjectIntegrityTests(unittest.TestCase):
    def test_project_descriptor_is_valid(self):
        data = json.loads((ROOT / "OpenWorldGame.uproject").read_text(encoding="utf-8"))
        self.assertEqual(data["EngineAssociation"], "5.8")
        self.assertEqual(data["Modules"][0]["Name"], "OpenWorldGame")
        self.assertIn("MADA | مَدَى", data["Description"])

    def test_android_configuration_is_present(self):
        config = (ROOT / "Config/DefaultEngine.ini").read_text(encoding="utf-8")
        self.assertIn("PackageName=com.abdullahlazim.openworldgame", config)
        self.assertIn("ApplicationDisplayName=MADA", config)
        self.assertIn("bBuildForArm64=True", config)
        self.assertIn("Orientation=Landscape", config)

    def test_mada_branding_is_configured_for_startup_and_menu(self):
        game_config = (ROOT / "Config/DefaultGame.ini").read_text(encoding="utf-8")
        widget = (ROOT / "Source/OpenWorldGame/Private/OWGMainMenuWidget.cpp").read_text(encoding="utf-8")
        self.assertIn("ProjectName=MADA", game_config)
        self.assertIn("GameNameEnglishUtf8", widget)
        self.assertIn("GameNameArabicUtf8", widget)
        self.assertIn("GameTaglineUtf8", widget)
        self.assertIn("StartupBackdrop->SetBrushColor", widget)
        self.assertIn("StartupBackdrop->SetVisibility(ESlateVisibility::Collapsed)", widget)
        self.assertIn("HandleStartupContinueClicked", widget)
        self.assertIn("void UOWGMainMenuWidget::SyncFrameCapSelection()", widget)
        self.assertIn("SyncFrameCapSelection();", widget)

    def test_pause_menu_has_keyboard_and_touch_paths(self):
        input_config = (ROOT / "Config/DefaultInput.ini").read_text(encoding="utf-8")
        controller = (ROOT / "Source/OpenWorldGame/Private/OWGPlayerController.cpp").read_text(encoding="utf-8")
        widget = (ROOT / "Source/OpenWorldGame/Private/OWGMainMenuWidget.cpp").read_text(encoding="utf-8")
        self.assertIn('ActionName="PauseMenu",Key=Escape', input_config)
        self.assertIn('BindAction(TEXT("PauseMenu")', controller)
        self.assertIn("InputComponent->bExecuteWhenPaused = true", controller)
        self.assertIn("void UOWGMainMenuWidget::TogglePauseMenu()", widget)
        self.assertIn("StartupBackdrop->SetVisibility(bPauseMenuVisible", widget)
        self.assertIn("PauseButton->OnClicked.AddDynamic", widget)
        self.assertIn("HandleResumeClicked", widget)
        self.assertIn("SyncFrameCapSelection();", widget)

    def test_frame_rate_limit_is_persisted_locally(self):
        game_instance = (ROOT / "Source/OpenWorldGame/Private/OWGGameInstance.cpp").read_text(encoding="utf-8")
        self.assertIn("Settings->SetFrameRateLimit", game_instance)
        self.assertIn("Settings->ApplySettings(false)", game_instance)
        self.assertIn("Settings->SaveSettings()", game_instance)

    def test_new_game_resets_save_and_restarts_current_level(self):
        game_instance = (ROOT / "Source/OpenWorldGame/Private/OWGGameInstance.cpp").read_text(encoding="utf-8")
        widget = (ROOT / "Source/OpenWorldGame/Private/OWGMainMenuWidget.cpp").read_text(encoding="utf-8")
        self.assertIn("UGameplayStatics::DeleteGameInSlot(SaveSlotName, 0)", game_instance)
        self.assertIn("if (!UGameplayStatics::DeleteGameInSlot", game_instance)
        self.assertIn("UGameplayStatics::OpenLevel(this, FName(*CurrentLevelName))", game_instance)
        self.assertIn("ConsumeStartNewGameAfterTravel()", widget)
        self.assertIn("HandleNewGameClicked", widget)

    def test_required_gameplay_sources_exist(self):
        required = [
            "Source/OpenWorldGame/Private/OWGCharacter.cpp",
            "Source/OpenWorldGame/Private/OWGVehiclePawn.cpp",
            "Source/OpenWorldGame/Private/OWGGameInstance.cpp",
            "Source/OpenWorldGame/Private/OWGMainMenuWidget.cpp",
            "Source/OpenWorldGame/Private/OWGGameMode.cpp",
        ]
        for item in required:
            with self.subTest(item=item):
                self.assertTrue((ROOT / item).is_file())
                self.assertGreater((ROOT / item).stat().st_size, 100)

    def test_static_validator_passes(self):
        result = subprocess.run(
            [sys.executable, str(ROOT / "Scripts/validate_project.py")],
            cwd=ROOT,
            capture_output=True,
            text=True,
            check=False,
        )
        self.assertEqual(result.returncode, 0, msg=result.stdout + result.stderr)
        self.assertIn("STATIC INTEGRITY: PASS", result.stdout)

    def test_unreal_and_apk_are_not_misreported_as_built(self):
        report = (ROOT / "Docs/IMPLEMENTATION_STATUS_AR.md").read_text(encoding="utf-8")
        self.assertIn("NOT BUILT", report)
        self.assertIn("NOT RUN", report)

    def test_required_ummg_handlers_are_declared_and_implemented(self):
        header = (ROOT / "Source/OpenWorldGame/Public/OWGMainMenuWidget.h").read_text(encoding="utf-8")
        source = (ROOT / "Source/OpenWorldGame/Private/OWGMainMenuWidget.cpp").read_text(encoding="utf-8")
        import re
        declared = re.findall(r"UFUNCTION\(\)\s+void\s+(Handle\w+)\(\);", header)
        self.assertGreaterEqual(len(declared), 15)
        for name in declared:
            with self.subTest(handler=name):
                self.assertIn(f"UOWGMainMenuWidget::{name}()", source)

    def test_core_runtime_flow_is_connected(self):
        game_mode = (ROOT / "Source/OpenWorldGame/Private/OWGGameMode.cpp").read_text(encoding="utf-8")
        character = (ROOT / "Source/OpenWorldGame/Private/OWGCharacter.cpp").read_text(encoding="utf-8")
        vehicle = (ROOT / "Source/OpenWorldGame/Private/OWGVehiclePawn.cpp").read_text(encoding="utf-8")
        game_instance = (ROOT / "Source/OpenWorldGame/Private/OWGGameInstance.cpp").read_text(encoding="utf-8")
        self.assertIn("AOWGCharacter::StaticClass()", game_mode)
        self.assertIn("AOWGVehiclePawn::StaticClass()", game_mode)
        self.assertIn("ClosestVehicle->EnterVehicle(this)", character)
        self.assertIn("PlayerController->Possess(this)", vehicle)
        self.assertIn("PlayerController->Possess(Character)", vehicle)
        self.assertIn("SaveGameToSlot", game_instance)
        self.assertIn("LoadGameFromSlot", game_instance)


    def test_engine_independent_core_is_integrated(self):
        required = [
            "Source/OpenWorldGame/Core/OWCredits.h",
            "Source/OpenWorldGame/Core/OWUnits.h",
            "Source/OpenWorldGame/Core/OWFrameStats.h",
            "Source/OpenWorldGame/Core/OWVehicleEntry.h",
            "Source/OpenWorldGame/SaveSystem/OWSaveData.h",
            "Tests/HostTests/test_main.cpp",
            "Scripts/run_host_tests.sh",
        ]
        for item in required:
            with self.subTest(item=item):
                self.assertTrue((ROOT / item).is_file())

        widget = (ROOT / "Source/OpenWorldGame/Private/OWGMainMenuWidget.cpp").read_text(encoding="utf-8")
        vehicle = (ROOT / "Source/OpenWorldGame/Private/OWGVehiclePawn.cpp").read_text(encoding="utf-8")
        self.assertIn("OW::FrameStats", (ROOT / "Source/OpenWorldGame/Public/OWGMainMenuWidget.h").read_text(encoding="utf-8"))
        self.assertIn("OW::DeveloperCreditUtf8", widget)
        self.assertIn("OW::CmPerSecToKmh", vehicle)
        self.assertIn("OW::CanEnterVehicle", vehicle)
        self.assertIn("OW::FindFirstFreeExitSpot", vehicle)

    def test_offline_permissions_and_mobile_render_config(self):
        config = (ROOT / "Config/DefaultEngine.ini").read_text(encoding="utf-8")
        active_lines = [line.split(";", 1)[0].strip() for line in config.splitlines()]
        active_config = "\n".join(active_lines)
        self.assertIn("ExtraPermissions=", active_config)
        self.assertNotIn("android.permission.INTERNET", active_config)
        self.assertIn("bSupportsVulkan=True", active_config)
        self.assertIn("bBuildForX8664=False", active_config)

    def test_host_core_test_sources_cover_save_migration(self):
        source = (ROOT / "Tests/HostTests/test_main.cpp").read_text(encoding="utf-8")
        self.assertIn("save v1 migrates to v2", source)
        self.assertIn("save corruption detected", source)
        self.assertIn("save checksum trailing garbage rejected", source)
        self.assertIn("save unsupported cap rejected", source)


if __name__ == "__main__":
    unittest.main()
