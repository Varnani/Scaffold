#pragma once

#include <string>

namespace Scaffold
{
    class AppLayer
    {
    public:
        std::string name = "";

        virtual void OnUpdate(float deltaTime) { ; }
        virtual void OnRenderUI(float deltaTime) { ; }
    };
} // namespace Scaffold
