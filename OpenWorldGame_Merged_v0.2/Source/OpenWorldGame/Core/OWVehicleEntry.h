#pragma once

#include <array>
#include <optional>

namespace OW
{
struct EntryRules
{
    double MaxDoorDistanceCm = 200.0;
    double MaxVehicleSpeedKmh = 5.0;
};

inline bool CanEnterVehicle(double DoorDistanceCm, double VehicleSpeedKmh, bool SeatFree,
                            const EntryRules& Rules = {})
{
    return SeatFree && DoorDistanceCm >= 0.0 && DoorDistanceCm <= Rules.MaxDoorDistanceCm &&
           VehicleSpeedKmh >= 0.0 && VehicleSpeedKmh <= Rules.MaxVehicleSpeedKmh;
}

// Receives four candidate points in preferred order and returns the first unblocked one.
// The point type is generic so the same tested logic can be used with FVector in Unreal.
template <typename Point, typename IsBlocked>
inline std::optional<Point> FindFirstFreeExitSpot(const std::array<Point, 4>& Candidates,
                                                  IsBlocked&& IsPointBlocked)
{
    for (const Point& Candidate : Candidates)
    {
        if (!IsPointBlocked(Candidate)) return Candidate;
    }
    return std::nullopt;
}
}
