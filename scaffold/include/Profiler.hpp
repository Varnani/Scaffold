#pragma once

#include <Marker.hpp>

#include <memory>
#include <map>

namespace Scaffold
{
	class Profiler
	{
	public:
		void BeginFrame();
		void EndFrame();

		void BeginMarker(const std::string name);
		void EndMarker();

		Marker& GetRootMarker();
		std::string GenerateReport();

	public:
		static Profiler& GetInstance();

	private:
		Profiler();

		Profiler(const Profiler&) = delete;
		Profiler(Profiler&&) = delete;

		Marker m_finishedRoot;
		Marker m_activeRoot;

		Marker* m_activeMarker;
		Marker* m_activeLoopMarker;
	};
} // namespace Scaffold



