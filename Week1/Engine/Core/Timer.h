#ifndef TIMER_H
#define TIMER_H

#include <SDL.h>

class Timer
{
public:
	Timer();

	Timer(const Timer&) = delete; //Disable the copy constructor
	Timer(Timer&&) = delete; //Disable the move constructor
	Timer& operator=(const Timer&) = delete; //Disable operator = for copy and move
	Timer& operator=(Timer&&) = delete;

	void Start();
	void UpdateFrameTicks();
	float GetDeltaTime() const; //Return delta time
	unsigned int GetSleepTime(const unsigned int fps_); //Make sure you never run over the specified FPS
	float GetCurrentTick() const; //Returns total time

private:
	unsigned int prevTicks, currentTicks;

};

#endif // !TIMER_H
