#pragma once

#include <string>

namespace Scaffold
{
    struct Manifest
    {
        std::string title;

        int initialWidth;
        int initialHeight;

        bool dockspaceOverViewport = false;
        bool useProfilerLayer = false;
    };
} // namespace Scaffold