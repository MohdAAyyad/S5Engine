#include "Window.h"
#include "../Rendering/Renderer.h"

Window::Window():width(0), height(0), window(nullptr) //This part is done before the constructor is called
{
}


Window::~Window()
{
	OnDestroy();
}

bool Window::OnCreate(Renderer* renderer_, std::string name_, int width_, int height_)
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		Debugger::Fatal("Failed to initialize SDL. Quitting program", "Window.cpp", __LINE__);
		return false;
	}

	width = width_;
	height = height_;
	
	renderer = renderer_;
	if(renderer)
		window = renderer->CreateWindow(name_, width_, height_);


	return (window != nullptr);
}

void Window::OnDestroy()
{
	Debugger::Info("Destroying window", "Window.cpp", __LINE__);
	SDL_DestroyWindow(window); //This is equal to delete window --> Destroy any variables or objects associated with the pointer. Always delete a pointor before making equal to nullptr
	window = nullptr;
}

int Window::GetWidth()
{
	return width;
}

int Window::GetHeight()
{
	return height;
}

SDL_Window* Window::GetWindow() const
{
	return window;
}