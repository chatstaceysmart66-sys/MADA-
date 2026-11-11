// Engine-independent tests. They do not prove Unreal compilation, Android packaging, or gameplay.
#include <algorithm>
#include <array>
#include <cmath>
#include <cstdio>
#include <cstring>
#include <limits>
#include <string>
#include <vector>

#include "../../Source/OpenWorldGame/Core/OWCredits.h"
#include "../../Source/OpenWorldGame/Core/OWUnits.h"
#include "../../Source/OpenWorldGame/Core/OWFrameStats.h"
#include "../../Source/OpenWorldGame/Core/OWVehicleEntry.h"
#include "../../Source/OpenWorldGame/SaveSystem/OWSaveData.h"

static int Pass = 0;
static int Fail = 0;

#define CHECK(Name, Condition) \
    do { \
        if (Condition) { ++Pass; std::printf("PASS  %s\n", Name); } \
        else { ++Fail; std::printf("FAIL  %s\n", Name); } \
    } while (0)

struct TestPoint { double X = 0.0; double Y = 0.0; };

static std::string WithChecksum(const std::string& Payload)
{
    return Payload + "crc=" + std::to_string(OW::Fnv1a(Payload)) + "\n";
}

int main()
{
    using namespace OW;

    CHECK("credit text exact", std::strcmp(DeveloperCreditUtf8, "تطوير: عبدالله لازم") == 0);
    CHECK("units 1000cm/s = 36km/h", std::fabs(CmPerSecToKmh(1000.0) - 36.0) < 1e-9);
    CHECK("units roundtrip", std::fabs(KmhToCmPerSec(CmPerSecToKmh(1234.5)) - 1234.5) < 1e-9);

    FrameStats Stats(1.0);
    CHECK("fps: no data => unavailable", Stats.MeasuredFps() < 0.0);
    Stats.AddFrame(0.0);
    Stats.AddFrame(-0.02);
    CHECK("fps: non-positive frame times ignored", Stats.MeasuredFps() < 0.0);
    for (int Index = 0; Index < 40; ++Index) Stats.AddFrame(1.0 / 40.0);
    CHECK("fps: 40 fps measured ~40", std::fabs(Stats.MeasuredFps() - 40.0) < 1.5);
    FrameStats StatsFromInvalidWindow(0.0);
    for (int Index = 0; Index < 60; ++Index) StatsFromInvalidWindow.AddFrame(1.0 / 60.0);
    CHECK("fps: non-positive window uses safe default", std::fabs(StatsFromInvalidWindow.MeasuredFps() - 60.0) < 1.5);
    FrameStats CapIndependentStats(1.0);
    for (int Index = 0; Index < 200; ++Index) CapIndependentStats.AddFrame(1.0 / 40.0);
    CHECK("fps: measured independently of selected cap", std::fabs(CapIndependentStats.MeasuredFps() - 40.0) < 1.5);
    CHECK("budget 60fps=16.67ms", std::fabs(FrameBudgetMs(60) - 16.6667) < 1e-3);
    CHECK("budget 120fps=8.33ms", std::fabs(FrameBudgetMs(120) - 8.3333) < 1e-3);
    CHECK("invalid frame budget is zero", FrameBudgetMs(0) == 0.0);

    const auto Caps60 = AllowedFrameCaps(60);
    const auto Caps90 = AllowedFrameCaps(90);
    const auto Caps120 = AllowedFrameCaps(120);
    CHECK("caps 60Hz = {30,60}", Caps60 == std::vector<int>({30, 60}));
    CHECK("caps 90Hz = {30,45,90}", Caps90 == std::vector<int>({30, 45, 90}));
    CHECK("caps 120Hz = {30,40,60,120}", Caps120 == std::vector<int>({30, 40, 60, 120}));
    CHECK("120 never offered on 60Hz", std::find(Caps60.begin(), Caps60.end(), 120) == Caps60.end());
    CHECK("caps invalid refresh => empty", AllowedFrameCaps(0).empty());

    EntryRules Rules;
    CHECK("enter valid", CanEnterVehicle(100.0, 0.0, true));
    CHECK("enter exact max door distance", CanEnterVehicle(200.0, 5.0, true));
    CHECK("enter too far", !CanEnterVehicle(201.0, 0.0, true));
    CHECK("enter negative distance rejected", !CanEnterVehicle(-1.0, 0.0, true));
    CHECK("enter while moving fast rejected", !CanEnterVehicle(100.0, 5.1, true));
    CHECK("enter negative speed rejected", !CanEnterVehicle(100.0, -1.0, true));
    CHECK("enter occupied seat rejected", !CanEnterVehicle(100.0, 0.0, false));
    Rules.MaxDoorDistanceCm = 350.0;
    CHECK("custom entry range", CanEnterVehicle(300.0, 1.0, true, Rules));

    const std::array<TestPoint, 4> ExitCandidates = {{{-150, 0}, {150, 0}, {0, -150}, {0, 150}}};
    const auto FirstFree = FindFirstFreeExitSpot(ExitCandidates, [](const TestPoint&) { return false; });
    CHECK("exit: first free candidate selected", FirstFree && FirstFree->X == -150.0);
    const auto RearFallback = FindFirstFreeExitSpot(ExitCandidates, [](const TestPoint& Point) { return Point.X != 0.0; });
    CHECK("exit: falls back to rear candidate", RearFallback && RearFallback->Y == -150.0);
    const auto NoExit = FindFirstFreeExitSpot(ExitCandidates, [](const TestPoint&) { return true; });
    CHECK("exit: all blocked => no unsafe placement", !NoExit.has_value());

    SaveData Data;
    Data.PlayerX = 12.5;
    Data.PlayerY = -3.0;
    Data.PlayerZ = 100.0;
    Data.LastVehicleId = "car_01";
    Data.FrameCap = 60;
    Data.MusicVolumePct = 40;
    const std::string Serialized = Serialize(Data);
    const auto RoundTrip = Deserialize(Serialized);
    CHECK("save roundtrip", RoundTrip && RoundTrip->PlayerX == 12.5 &&
          RoundTrip->LastVehicleId == "car_01" && RoundTrip->FrameCap == 60 &&
          RoundTrip->MusicVolumePct == 40);

    std::string Corrupted = Serialized;
    Corrupted[8] = (Corrupted[8] == '9' ? '8' : '9');
    CHECK("save corruption detected", !Deserialize(Corrupted).has_value());
    CHECK("save truncated rejected", !Deserialize(Serialized.substr(0, Serialized.size() / 2)).has_value());
    CHECK("save checksum trailing garbage rejected", !Deserialize(Serialized + "junk").has_value());

    const std::string V1Payload = "version=1\npx=1\npy=2\npz=3\nmusic=55\n";
    const auto Migrated = Deserialize(WithChecksum(V1Payload));
    CHECK("save v1 migrates to v2", Migrated && Migrated->Version == CurrentSaveVersion &&
          Migrated->PlayerZ == 3.0 && Migrated->FrameCap == 0 && Migrated->MusicVolumePct == 55);

    const std::string V9Payload = "version=9\npx=1\npy=2\npz=3\nmusic=55\nvehicle=\nframecap=0\n";
    CHECK("save newer version rejected", !Deserialize(WithChecksum(V9Payload)).has_value());

    const std::string BadNumberPayload = "version=2\npx=nan\npy=2\npz=3\nvehicle=car\nframecap=60\nmusic=55\n";
    CHECK("save non-finite coordinate rejected", !Deserialize(WithChecksum(BadNumberPayload)).has_value());
    const std::string BadVolumePayload = "version=2\npx=1\npy=2\npz=3\nvehicle=car\nframecap=60\nmusic=101\n";
    CHECK("save invalid volume rejected", !Deserialize(WithChecksum(BadVolumePayload)).has_value());
    const std::string BadCapPayload = "version=2\npx=1\npy=2\npz=3\nvehicle=car\nframecap=75\nmusic=55\n";
    CHECK("save unsupported cap rejected", !Deserialize(WithChecksum(BadCapPayload)).has_value());
    const std::string DuplicatePayload = "version=2\nversion=2\npx=1\npy=2\npz=3\nvehicle=car\nframecap=60\nmusic=55\n";
    CHECK("save duplicate field rejected", !Deserialize(WithChecksum(DuplicatePayload)).has_value());

    const auto Fallback = LoadWithFallback(Corrupted, Serialized);
    CHECK("save falls back to backup when primary corrupt", Fallback && Fallback->LastVehicleId == "car_01");
    CHECK("save both corrupt => none", !LoadWithFallback(Corrupted, Corrupted).has_value());

    std::printf("\nSUMMARY pass=%d fail=%d\n", Pass, Fail);
    return Fail == 0 ? 0 : 1;
}
