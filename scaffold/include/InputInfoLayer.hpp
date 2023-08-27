#pragma once

#include <AppLayer.hpp>

#include <vector>

class InputInfoLayer : public Scaffold::AppLayer
{
public:
    void OnRenderUI(float deltaTime) override;
};