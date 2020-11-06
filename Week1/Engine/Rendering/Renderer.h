#ifndef RENDERER_H
#define RENDERER_H

#include <string>
#include <SDL.h>

enum RendererType
{
	OPENGL = 0,
	VULKAN = 1
};

class Renderer
{
public:
	virtual SDL_Window* CreateWindow(std::string name_, int width_, int height_) = 0;
};

#endif