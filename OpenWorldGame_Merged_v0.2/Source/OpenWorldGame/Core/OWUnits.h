#pragma once
namespace OW
{
// Unreal world velocity is centimetres/second; the UI displays kilometres/hour.
inline constexpr double CmPerSecToKmh(double CmPerSec) { return CmPerSec * 0.036; }
inline constexpr double KmhToCmPerSec(double Kmh) { return Kmh / 0.036; }
}
