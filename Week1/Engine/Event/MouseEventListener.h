#ifndef MOUSEEVENTLISTENER_H
#define MOUSEEVENTLISTENER_H

#include <SDL.h>
#include <glm/glm.hpp>

class CoreEngine;
class MouseEventListener
{

	~MouseEventListener();
public:

	MouseEventListener() = delete; //Static class, doesn't need a constructor
	MouseEventListener(const MouseEventListener&) = delete; 
	MouseEventListener(MouseEventListener&&) = delete; 
	MouseEventListener& operator=(const MouseEventListener&) = delete; 
	MouseEventListener& operator=(MouseEventListener&&) = delete;


	static void RegisterEngineObject(CoreEngine* engine_); //The listener/observer design pattern requires to register the engine. It's not needed in our case as we have a core engine singleton, but what the hell
	static void Update(SDL_Event e_);

	static void NotifyOfMousePressed(int buttonType_); //buttonType_ will be populated by SDL
	static void NotifyOfMouseReleased(int buttonType_);
	static void NotifyOfMouseMove();
	static void NotifyOfMouseScroll(int y_); //y_ is populated by SDL

	static glm::vec2 GetMousePrevPos();
	static glm::vec2 GetMouseCurrentPos();
	static glm::vec2 GetMouseOffset(); //Current - previous

private:
	static CoreEngine* engineInstance;
	static glm::vec2 currentMouse, prevMouse;
	static bool firstUpdate; //If it's the first time the mouse has moved, its prev and current pos are the same
	static void UpdateMousePosition();
};

#endif // !MOUSEEVENTLISTENER_H

