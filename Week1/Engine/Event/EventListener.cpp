#include "EventListener.h"
#include "../Core/CoreEngine.h"

EventListener::~EventListener() 
{

}
void EventListener::Update()
{
	SDL_Event sdlEvent;

	while (SDL_PollEvent(&sdlEvent))
	{
		switch (sdlEvent.type)
		{
		case SDL_QUIT:
			CoreEngine::GetInstance()->Exit();
		case SDL_MOUSEBUTTONDOWN:
		case SDL_MOUSEBUTTONUP:
		case SDL_MOUSEMOTION:
		case SDL_MOUSEWHEEL:
			MouseEventListener::Update(sdlEvent);
			break;
		default:
			break;
		}
	}
}