#pragma once
#include "Renderer.h"
#include <glew.h>
class OpenGLRenderer :	public Renderer
{
public:
	OpenGLRenderer();
	~OpenGLRenderer();
	SDL_Window* CreateWindow(std::string name_, int width_, int height_) override;

	void OnDestroy();

	void Render() override {};

private:
	SDL_GLContext context;

	void SetPreAttributes(); //Before the window is created
	void SetPostAttributes(); //After the window is created


	
};

