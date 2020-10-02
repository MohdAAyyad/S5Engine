#include "MouseEventListener.h"
#include "../Core/CoreEngine.h"

//Main listener. Listens to any type of mouse input

CoreEngine* MouseEventListener::engineInstance = nullptr;
glm::vec2  MouseEventListener::currentMouse = glm::vec2();
glm::vec2  MouseEventListener::prevMouse = glm::vec2();
bool MouseEventListener::firstUpdate = true;

MouseEventListener::~MouseEventListener()
{
	if (engineInstance) 
		engineInstance = nullptr; //No don't delete the singleton engine instance
}
void MouseEventListener::RegisterEngineObject(CoreEngine* engine_)
{
	engineInstance = engine_;
}
void MouseEventListener::Update(SDL_Event e_)
{
	switch (e_.type)
	{
	case SDL_MOUSEMOTION:
		UpdateMousePosition();
		NotifyOfMouseMove();
		break;
	case SDL_MOUSEBUTTONDOWN:
		UpdateMousePosition();
		NotifyOfMousePressed(e_.button.button);
	case SDL_MOUSEBUTTONUP:
		UpdateMousePosition();
		NotifyOfMouseReleased(e_.button.button); //1st button: the SDL event. 2nd button: which button was pressed
		break;
	case SDL_MOUSEWHEEL:
		NotifyOfMouseScroll(e_.wheel.y);//wheel: SDL wheel event // y: value
		break;
	default:	//No default case cause we are not checking for every SLD event
		break;
	}
}
void MouseEventListener::NotifyOfMousePressed(int buttonType_)
{
	if (engineInstance)
	{
		engineInstance->NotifyOfMousePressed(currentMouse);
	}
}
void MouseEventListener::NotifyOfMouseReleased(int buttonType_)
{
	if (engineInstance)
	{
		engineInstance->NotifyOfMouseReleased(currentMouse,buttonType_);
	}
}
void MouseEventListener::NotifyOfMouseMove()
{
	if (engineInstance)
	{
		engineInstance->NotifyOfMouseMove(currentMouse);
	}
}
void MouseEventListener::NotifyOfMouseScroll(int y_)
{
	if (engineInstance)
	{
		engineInstance->NotifyOfMouseScroll(y_);
	}
}
glm::vec2 MouseEventListener::GetMousePrevPos()
{
	return prevMouse;
}
glm::vec2 MouseEventListener::GetMouseCurrentPos()
{
	return currentMouse;
}
glm::vec2 MouseEventListener::GetMouseOffset()
{
	//SDL data start with zero at the top of the screen, but we've built the enigine with zero being at the bottom
	return (glm::vec2(currentMouse.x - prevMouse.x, prevMouse.y - currentMouse.y)); //That's why we're doing prev-current for the y value
}
void MouseEventListener::UpdateMousePosition()
{
	glm::ivec2 tmp;
	SDL_GetMouseState(&tmp.x, &tmp.y); 

	tmp.y = engineInstance->GetWindowSize().y - tmp.y; //Again we assume 0 is at the bottom

	if (firstUpdate)
	{
		prevMouse = currentMouse = tmp;
		firstUpdate = false;
	}
	else if(tmp.x != currentMouse.x || tmp.y != currentMouse.y)
	{
		prevMouse = currentMouse;
		currentMouse = tmp;
	}

}