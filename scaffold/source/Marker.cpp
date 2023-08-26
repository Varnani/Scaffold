#include <Marker.hpp>

// ------
Scaffold::TimePoint GetTime()
{
    return std::chrono::high_resolution_clock::now();
}

float GetElapsed(Scaffold::TimePoint start, Scaffold::TimePoint end)
{
    Scaffold::NanoSecs measurement = std::chrono::duration_cast<Scaffold::NanoSecs>(end - start);
    float duration = measurement.count() * 0.000001;

    return duration;
}
// ------

Scaffold::Marker::Marker(const std::string name)
{
    this->name = name;
    duration = 0;

    StartMeasurement();
}

Scaffold::Marker::Marker()
{
    this->name = "Unnamed";
    duration = 0;

    StartMeasurement();
}

void Scaffold::Marker::StartMeasurement()
{
    start = GetTime();
}

void Scaffold::Marker::CompleteMeasurement()
{
    measurementCount++;

    end = GetTime();
    duration += GetElapsed(start, end);

    if (measurementCount > 1)
    {
        duration *= 0.5f;
    }
}