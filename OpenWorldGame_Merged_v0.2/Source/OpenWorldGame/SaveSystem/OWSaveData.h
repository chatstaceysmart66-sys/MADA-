#pragma once

#include <cmath>
#include <cstdint>
#include <limits>
#include <map>
#include <optional>
#include <sstream>
#include <string>

namespace OW
{
constexpr int CurrentSaveVersion = 2;

// Portable, engine-independent save schema used by host-side tests and migration checks.
// The in-game save currently uses Unreal's USaveGame class in OWGSaveGame.h.
struct SaveData
{
    int Version = CurrentSaveVersion;
    double PlayerX = 0.0;
    double PlayerY = 0.0;
    double PlayerZ = 0.0;
    std::string LastVehicleId;
    int FrameCap = 0; // 0 = automatic
    int MusicVolumePct = 70;
};

inline uint32_t Fnv1a(const std::string& Text)
{
    uint32_t Hash = 2166136261u;
    for (unsigned char Character : Text)
    {
        Hash ^= Character;
        Hash *= 16777619u;
    }
    return Hash;
}

inline std::string Serialize(const SaveData& Data)
{
    std::ostringstream Body;
    Body.precision(17);
    Body << "version=" << Data.Version << "\n"
         << "px=" << Data.PlayerX << "\n"
         << "py=" << Data.PlayerY << "\n"
         << "pz=" << Data.PlayerZ << "\n"
         << "vehicle=" << Data.LastVehicleId << "\n"
         << "framecap=" << Data.FrameCap << "\n"
         << "music=" << Data.MusicVolumePct << "\n";
    const std::string Payload = Body.str();
    return Payload + "crc=" + std::to_string(Fnv1a(Payload)) + "\n";
}

inline std::optional<SaveData> Deserialize(const std::string& Text)
{
    const size_t ChecksumPosition = Text.rfind("crc=");
    if (ChecksumPosition == std::string::npos ||
        (ChecksumPosition > 0 && Text[ChecksumPosition - 1] != '\n'))
    {
        return std::nullopt;
    }

    const std::string Payload = Text.substr(0, ChecksumPosition);
    std::string ChecksumText = Text.substr(ChecksumPosition + 4);
    if (!ChecksumText.empty() && ChecksumText.back() == '\n') ChecksumText.pop_back();
    if (!ChecksumText.empty() && ChecksumText.back() == '\r') ChecksumText.pop_back();
    if (ChecksumText.empty()) return std::nullopt;
    for (char Character : ChecksumText)
    {
        if (Character < '0' || Character > '9') return std::nullopt;
    }

    uint64_t ParsedChecksum = 0;
    try
    {
        ParsedChecksum = std::stoull(ChecksumText);
    }
    catch (...)
    {
        return std::nullopt;
    }
    if (ParsedChecksum > std::numeric_limits<uint32_t>::max() ||
        static_cast<uint32_t>(ParsedChecksum) != Fnv1a(Payload))
    {
        return std::nullopt;
    }

    std::map<std::string, std::string> Values;
    std::istringstream Input(Payload);
    std::string Line;
    while (std::getline(Input, Line))
    {
        const size_t Separator = Line.find('=');
        if (Separator == std::string::npos || Separator == 0) return std::nullopt;
        const std::string Key = Line.substr(0, Separator);
        const std::string Value = Line.substr(Separator + 1);
        if (!Values.emplace(Key, Value).second) return std::nullopt;
    }

    auto ParseInt = [&Values](const char* Key, int& Out) -> bool
    {
        const auto It = Values.find(Key);
        if (It == Values.end() || It->second.empty()) return false;
        try
        {
            size_t Used = 0;
            const long Parsed = std::stol(It->second, &Used);
            if (Used != It->second.size() || Parsed < std::numeric_limits<int>::min() ||
                Parsed > std::numeric_limits<int>::max()) return false;
            Out = static_cast<int>(Parsed);
            return true;
        }
        catch (...) { return false; }
    };

    auto ParseDouble = [&Values](const char* Key, double& Out) -> bool
    {
        const auto It = Values.find(Key);
        if (It == Values.end() || It->second.empty()) return false;
        try
        {
            size_t Used = 0;
            const double Parsed = std::stod(It->second, &Used);
            if (Used != It->second.size() || !std::isfinite(Parsed)) return false;
            Out = Parsed;
            return true;
        }
        catch (...) { return false; }
    };

    SaveData Data;
    if (!ParseInt("version", Data.Version) || Data.Version < 1 || Data.Version > CurrentSaveVersion ||
        !ParseDouble("px", Data.PlayerX) || !ParseDouble("py", Data.PlayerY) ||
        !ParseDouble("pz", Data.PlayerZ) || !ParseInt("music", Data.MusicVolumePct) ||
        Data.MusicVolumePct < 0 || Data.MusicVolumePct > 100)
    {
        return std::nullopt;
    }

    if (Data.Version >= 2)
    {
        const auto Vehicle = Values.find("vehicle");
        if (Vehicle == Values.end() || Vehicle->second.find_first_of("\r\n") != std::string::npos ||
            !ParseInt("framecap", Data.FrameCap)) return std::nullopt;
        const int Caps[] = {0, 30, 40, 45, 60, 90, 120};
        bool ValidCap = false;
        for (int Cap : Caps) ValidCap = ValidCap || Data.FrameCap == Cap;
        if (!ValidCap) return std::nullopt;
        Data.LastVehicleId = Vehicle->second;
    }
    else
    {
        Data.LastVehicleId.clear();
        Data.FrameCap = 0;
    }

    Data.Version = CurrentSaveVersion;
    return Data;
}

inline std::optional<SaveData> LoadWithFallback(const std::string& Primary,
                                                const std::string& Backup)
{
    if (auto Current = Deserialize(Primary)) return Current;
    return Deserialize(Backup);
}
}
