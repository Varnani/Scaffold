#pragma once

#include <AppLayer.hpp>

class DemoLayer : public Scaffold::AppLayer
{
public:
    void OnRenderUI(float deltaTime) override;
};
