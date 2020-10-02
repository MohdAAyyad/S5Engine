#include "../Math/CollisionHandler.h"
#include "CoreEngine.h"

std::unique_ptr<CoreEngine> CoreEngine::engineInstance = nullptr; //Initialize a static variable

CoreEngine::CoreEngine() : window(nullptr), isRunning(false), fps(120), gameInterface(nullptr), currentSceneNum(0), camera(nullptr)
{
}

CoreEngine::~CoreEngine()
{
	OnDestroy();
}

void CoreEngine::OnDestroy()
{
	//Always destroy from the inside out
	ShaderHandler::GetInstance()->OnDestroy();
	TextureHandler::GetInstance()->OnDestroy();
	SceneGraph::GetInstance()->OnDestroy();
	CollisionHandler::GetInstance()->OnDestroy();

	if (camera)
	{
		delete camera;
	}
	camera = nullptr;

	if (gameInterface)
	{
		delete gameInterface;
	}
	gameInterface = nullptr;


	if (window)
	{
		delete window;
	}
	window = nullptr;


	Debugger::Info("Destroying CoreEngine instance", "CoreEngine.cpp", __LINE__);

	SDL_Quit();
	exit(0);
}

bool CoreEngine::OnCreate(std::string name_, int width_, int height_)
{
	Debugger::DebugInit();
	Debugger::SetSeverity(MessageType::TYPE_INFO);
	window = new Window();

	if (!window->OnCreate(name_, width_, height_))
	{
		Debugger::Fatal("Failed to create window. Quitting program", "CoreEngine.cpp", __LINE__);
		return isRunning = false;
	}

	SDL_WarpMouseInWindow(window->GetWindow(), window->GetWidth() / 2, window->GetHeight() / 2); //Takes the mouse from wherever it is and automatically moves it to the center of the widnow //Integer division as the wrap function takes in integers
	MouseEventListener::RegisterEngineObject(this);

	ShaderHandler::GetInstance()->CreateProgram("colorShader",
												"Engine/Shaders/ColorVertexShader.glsl",
												"Engine/Shaders/ColorFragmentShader.glsl");

	ShaderHandler::GetInstance()->CreateProgram("basicShader", 
												"Engine/Shaders/VertexShader.glsl",
												"Engine/Shaders/FragmentShader.glsl");

	if (gameInterface)
	{
		if (!gameInterface->OnCreate())
		{
			Debugger::Error("Could not create game interface", "CoreEngine.cpp", __LINE__);
		}
	}

	timer.Start();

	Debugger::Info("Everything worked!", "CoreEngine.cpp", __LINE__); //You could use __FILE__ for the file name but that would write out the full directory of the function
	return isRunning = true;

}

void CoreEngine::Run()
{
	while (isRunning)
	{
		timer.UpdateFrameTicks();
		EventListener::Update();
		Update(timer.GetDeltaTime());
		if (gameInterface)
		{
			gameInterface->Update(timer.GetDeltaTime());
		}
		Render();
		SDL_Delay(timer.GetSleepTime(fps));
	}

	if (!isRunning)
	{
		OnDestroy();
	}
}

bool CoreEngine::GetIsRunning() const
{
	return isRunning;
}

CoreEngine * CoreEngine::GetInstance()
{
	if (engineInstance.get() == nullptr)
	{
		engineInstance.reset(new CoreEngine);
	}

	return engineInstance.get();
}

void CoreEngine::Update(const float deltaTime_)
{

}

void CoreEngine::Render()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //Clear any artifact windows //This tells OpenGl which buffers to clear on every Render call
	//Render Game
	if (gameInterface)
	{
		gameInterface->Render();
	}
	//End Render Game

	//Swap Window
	SDL_GL_SwapWindow(window->GetWindow());
}

void CoreEngine::SetGameInterface(GameInterface* gameInterface_)
{
	gameInterface = gameInterface_;
}

int CoreEngine::GetCurrentScene()
{
	return currentSceneNum;
}

void CoreEngine::SetCurrentScene(int sceneNum_)
{
	currentSceneNum = sceneNum_;
}

glm::vec2 CoreEngine::GetWindowSize() const
{
	return glm::vec2(window->GetWidth(), window->GetHeight());
}

void CoreEngine::Exit()
{
	isRunning = false;
}

Camera* CoreEngine::GetCamera() const
{
	return camera;
}
void CoreEngine::SetCamera(Camera* camera_)
{
	camera = camera_;
}


void CoreEngine::NotifyOfMousePressed(glm::vec2 mouse_)
{
	
}
void CoreEngine::NotifyOfMouseReleased(glm::vec2 mouse_, int buttonType_)
{
	CollisionHandler::GetInstance()->MouseUpdate(mouse_, buttonType_);
}
void CoreEngine::NotifyOfMouseMove(glm::vec2 mouse_)
{
	if(camera)
		camera->ProcessMouseMovement(MouseEventListener::GetMouseOffset());
}
void CoreEngine::NotifyOfMouseScroll(int y_)
{
	if (camera)
		camera->ProcessMouseZoom(y_);
}
