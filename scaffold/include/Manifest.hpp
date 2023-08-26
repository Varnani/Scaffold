#pragma once

#include <string>

namespace Scaffold
{
    struct Manifest
    {
        std::string title;

        int initialWidth;
        int initialHeight;

        float eventWaitTimeout = -1;

        bool dockspaceOverViewport = false;
        bool useProfilerLayer = false;
        bool useInputInfoLayer = false;
        bool useDemoLayer = false;
    };
} // namespace Scaffold