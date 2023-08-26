#pragma once

#include <chrono>
#include <string>
#include <vector>

namespace Scaffold
{
    typedef std::chrono::time_point<std::chrono::high_resolution_clock> TimePoint;
    typedef std::chrono::nanoseconds NanoSecs;

    struct Marker
    {
        std::string name;
        float durationAsMilliseconds = 0;
        float durationAsSeconds = 0;

        size_t measurementCount = 0;

        TimePoint start;
        TimePoint end;

        std::vector<Marker> subMarkers;
        Marker* parentMarker = nullptr;

        Marker(const std::string name);
        Marker();

        void StartMeasurement();
        void CompleteMeasurement();
    };
} // namespace Scaffold
