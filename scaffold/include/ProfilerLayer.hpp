#pragma once

#include <AppLayer.hpp>

class ProfilerLayer : public Scaffold::AppLayer
{
public:
	void OnRenderUI(float deltaTime) override;
};