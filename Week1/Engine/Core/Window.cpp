#include "Window.h"

Window::Window():width(0), height(0), window(nullptr) //This part is done before the constructor is called
{
}


Window::~Window()
{
	OnDestroy();
}

bool Window::OnCreate(std::string name_, int width_, int height_)
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		Debugger::Fatal("Failed to initialize SDL. Quitting program", "Window.cpp", __LINE__);
		return false;
	}

	width = width_;
	height = height_;
	
	SetPreAttributes();//Contains  glewExperimental = GL_TRUE; --> Needs to be called before glewInit()
	window = SDL_CreateWindow(name_.c_str(), //Title
							 SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, //X, Y pos
							 width, height, //Width and height
							 SDL_WINDOW_OPENGL); //This window can run OpenGL

	//Check if window was created correctly
	if (!window)
	{
		Debugger::Fatal("Failed to create Window. Quitting program", "Window.cpp", __LINE__);
		return false;
	}

	context = SDL_GL_CreateContext(window);
	SetPostAttributes(); //Set Depth after context is created
	GLenum error = glewInit(); 
	if (error != GLEW_OK)
	{
		Debugger::Warning("Failed to initialize glew. Quitting program", "Window.cpp", __LINE__);
		return false;
	}

	glEnable(GL_DEPTH_TEST); //If depth is not enabled, OpenGl will render all it gets on FIFO basis. Enabling it allows it to check for Z depth
	glViewport(0.0f, 0.0f, width, height); //This function needs to be called if you resize window

	std::cout << "Graphics Card: " << glGetString(GL_VENDOR) << std::endl;
	std::cout << "Open GL version" << glGetString(GL_VERSION) << std::endl;

	return true;
}

void Window::OnDestroy()
{
	Debugger::Info("Destroying window", "Window.cpp", __LINE__);
	SDL_GL_DeleteContext(context);
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

void Window::SetPreAttributes()
{
	//SDL_GL_SetAttribute(attribute to set, value)
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE); //Which OpenGl profile to use? Core does not allow deprected functions
	//Open GL 4.6
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);//A frame behind a frame so you switch between them when you're rendering. 
												//When you use double buffering, drawing is done in an offscreen buffer and the image is moved from the offscreen buffer to the screen

	SDL_GL_SetSwapInterval(1);// 1 means Update frames is synchronized with vertical retrace.
							  // vertical retrace is a measurement of time related to video displays. It represents the amount of time between the end of one frame being drawn, and the beginning of the next frame.

	glewExperimental = GL_TRUE; //Must be done before glew is initialized
}

void Window::SetPostAttributes()
{
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 32); //Depth size is in bits size so this is 32 bits
}