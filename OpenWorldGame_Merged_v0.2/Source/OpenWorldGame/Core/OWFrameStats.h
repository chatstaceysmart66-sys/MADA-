#pragma once

#include <deque>
#include <vector>

namespace OW
{
// Measures FPS from elapsed frame times. It never returns the selected frame cap.
class FrameStats
{
public:
    explicit FrameStats(double WindowSeconds = 1.0)
        : Window(WindowSeconds > 0.0 ? WindowSeconds : 1.0)
    {
    }

    void AddFrame(double DeltaSeconds)
    {
        if (DeltaSeconds <= 0.0) return;
        Frames.push_back(DeltaSeconds);
        Total += DeltaSeconds;
        while (Total > Window && Frames.size() > 1)
        {
            Total -= Frames.front();
            Frames.pop_front();
        }
    }

    bool HasMeasurement() const { return Total >= Window * 0.5 && !Frames.empty(); }

    // Returns -1 until enough real frame-time data has accumulated.
    double MeasuredFps() const
    {
        return HasMeasurement() ? static_cast<double>(Frames.size()) / Total : -1.0;
    }

private:
    double Window;
    double Total = 0.0;
    std::deque<double> Frames;
};

// Host-side policy helper only. Runtime UI must still ask the platform frame pacer
// and inspect the real display mode before exposing device-specific caps.
inline std::vector<int> AllowedFrameCaps(int RefreshHz)
{
    static const int Candidates[] = {30, 40, 45, 60, 90, 120};
    std::vector<int> Out;
    for (int Cap : Candidates)
    {
        if (RefreshHz > 0 && Cap <= RefreshHz && RefreshHz % Cap == 0)
        {
            Out.push_back(Cap);
        }
    }
    return Out;
}

inline double FrameBudgetMs(int Fps)
{
    return Fps > 0 ? 1000.0 / static_cast<double>(Fps) : 0.0;
}
}
