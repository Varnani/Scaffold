#include <Profiler.hpp>

#include <iostream>
#include <sstream>
#include <iomanip>
#include <stack>

using namespace Scaffold;

Profiler::Profiler()
{
	m_finishedRoot = Marker("Frame");
}

void Profiler::BeginFrame()
{
	m_activeRoot = Marker("Frame");
	m_activeMarker = &m_activeRoot;
}

void Profiler::EndFrame()
{
	EndMarker();
	m_finishedRoot = m_activeRoot;
}

void Profiler::BeginMarker(const std::string name)
{
	m_activeMarker->subMarkers.emplace_back(name);

	Marker* marker = &m_activeMarker->subMarkers.back();
	marker->parentMarker = m_activeMarker;
	m_activeMarker = marker;
}

void Profiler::EndMarker()
{
	m_activeMarker->CompleteMeasurement();
	m_activeMarker = m_activeMarker->parentMarker;
}

Marker& Profiler::GetRootMarker()
{
	return m_finishedRoot;
}

std::string Profiler::GenerateReport()
{
	Marker& rootMarker = GetRootMarker();
	if (rootMarker.subMarkers.size() == 0) return "Report is empty.";

	std::stack<Marker*> markerStack;
	std::stack<bool> visitStack;

	markerStack.push(&rootMarker);
	visitStack.push(false);

	std::string report;

	int indentLevel = 0;
	while (!markerStack.empty())
	{
		Marker* marker = markerStack.top();
		markerStack.pop();

		bool isVisited = visitStack.top();
		visitStack.pop();

		if (isVisited)
		{
			indentLevel--;
			continue;
		}

		std::stringstream formattedMarkerLabel;
		formattedMarkerLabel << "- " << marker->name << " | " << std::fixed << std::setprecision(4) << marker->durationAsMilliseconds << "ms\n";

		std::string spaces(indentLevel * 2, ' ');
		std::string formattedString = formattedMarkerLabel.str();

		std::string markerText = spaces + formattedString;

		report.append(markerText);

		int subMarkerCount = marker->subMarkers.size();
		if (subMarkerCount > 0)
		{
			markerStack.push(marker);
			visitStack.push(true);

			for (int i = subMarkerCount - 1; i > -1; i--)
			{
				markerStack.push(&marker->subMarkers[i]);
				visitStack.push(false);
			}

			indentLevel++;
		}
	}

	return report;
}
