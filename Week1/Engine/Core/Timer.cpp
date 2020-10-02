#include "Timer.h"

Timer::Timer(): prevTicks(0), currentTicks(0) {}


void Timer::Start()
{
	prevTicks = SDL_GetTicks();
	currentTicks = SDL_GetTicks();
}

void Timer::UpdateFrameTicks()
{
	prevTicks = currentTicks;
	currentTicks = SDL_GetTicks();
}

float Timer::GetDeltaTime() const //Return delta time
{
	return static_cast<float>(currentTicks - prevTicks) / 1000.0f; //Divide by 1000 cause SDL_GetTicks() returns time in milliseconds and we want it in seconds
	//(float) is a c cast, and forces the cast. If it doesn't work, that's your problem.
	//static_cast checks if the cast works, and if not, returns a nullptr 
}

unsigned int Timer::GetSleepTime(const unsigned int fps_) //Make sure you never run over the specified FPS
{
	unsigned int milliPerFrame = 1000 / fps_;
	
	if (milliPerFrame == 0) //more or equal to 1000. Int division.
	{
		return 0;
	}
	
	unsigned int sleepTime = milliPerFrame - SDL_GetTicks(); //SDL_GetTicks returns unsigned int in milliseconds
															 //Check the difference between the desired FPS and the current FPS basically
	if (sleepTime > milliPerFrame)
	{
		return milliPerFrame;
	}
	
	return sleepTime;
}

float Timer::GetCurrentTick() const //Returns total time
{
	return static_cast<float>(currentTicks);
}