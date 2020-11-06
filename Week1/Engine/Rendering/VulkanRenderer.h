#pragma once
#include "Renderer.h"
#include <SDL.h>
class VulkanRenderer : public Renderer
{
public:
	VulkanRenderer();
	~VulkanRenderer();
	SDL_Window* CreateWindow(std::string name_, int width_, int height_) override;
};

