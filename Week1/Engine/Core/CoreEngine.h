#ifndef COREENGINE_H
#define COREENGINE_H

#include "Window.h"
#include <memory>
#include "TImer.h"
#include "Debugger.h"
#include "GameInterface.h"
#include "Scene.h"
#include "../Graphics/ShaderHandler.h"
#include "../Graphics/TextureHandler.h"
#include "../Rendering/SceneGraph.h"
#include "../Camera/Camera.h"
#include "../Event/EventListener.h"
#include "../Audio/AudioHandler.h"
#include "../Rendering/Renderer.h"

class CoreEngine
{
public:
	CoreEngine();
	~CoreEngine();
	CoreEngine(const CoreEngine&) = delete; //Disable the copy constructor
	CoreEngine(CoreEngine&&) = delete; //Disable the move constructor
	CoreEngine& operator=(const CoreEngine&) = delete; //Disable operator = for copy and move
	CoreEngine& operator=(CoreEngine&&) = delete; 

	bool OnCreate(std::string name_, int width_, int height_);
	void Run();
	bool GetIsRunning() const;

	static CoreEngine* GetInstance();

	void SetGameInterface(GameInterface* gameInterface_);

	int GetCurrentScene();
	void SetCurrentScene(int sceneNum_);

	glm::vec2 GetWindowSize() const;

	Camera* GetCamera() const;
	void SetCamera(Camera* camera_);

	void Exit();

	void NotifyOfMousePressed(glm::vec2 mouse_);
	void NotifyOfMouseReleased(glm::vec2 mouse_, int buttonType_); //We need buttonType here as events usually fire on release
	void NotifyOfMouseMove(glm::vec2 mouse_);
	void NotifyOfMouseScroll(int y_);

	Renderer* GetRenderer();
	RendererType GetRendType();
	void SetRendType(RendererType type_);
	Window* GetWindow() const;

private:
	void Update(const float deltaTime_);
	void Render();
	void Draw();

	//Singleton
	static std::unique_ptr<CoreEngine> engineInstance; //The instance. Unique pointers are a type of Smart Pointers. Smart Pointers take "ownership" of the object they're pointing to. Unique pointers are used to make sure there's only one instance of the object. unique_ptr objects automatically delete the object they manage (using a deleter) as soon as they themselves are destroyed, or as soon as their value changes either by an assignment operation or by an explicit call to unique_ptr::reset.
													   //A unique ptr has a deleter component which is used to delete the object it is pointing. It defaults to default_delete hence why we make sure the default_delete is of type CoreEngine in the next line.
	friend std::default_delete<CoreEngine>; //Used with smart pointers. Once the ptr goes out of scope, the default delete is called in case a deleter is not assigned to the pointer. This default deleter is of type CoreEngine
											//We need the friend keyword to allow it to access private members.

	void OnDestroy();

	//Window
	Window* window;
	bool isRunning;

	//Timer
	Timer timer;
	unsigned int fps;

	//GameInterface
	GameInterface* gameInterface;

	//Scene
	int currentSceneNum;

	//Camera
	Camera* camera;

	Renderer* renderer;
	RendererType rendType;
};

#endif

