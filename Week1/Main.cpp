#include "Engine/Core/CoreEngine.h"
#include "Game/Game1.h"

int main(int argc, char* argv[])
{
	CoreEngine::GetInstance()->SetRendType(RendererType::OPENGL);
	CoreEngine::GetInstance()->SetGameInterface(new Game1);
	if (!CoreEngine::GetInstance()->OnCreate("Week 1", 800, 600))
	{
		Debugger::Fatal("Failed to create engine. Quitting program", "Main.cpp", __LINE__);
		return -1;
	}

	CoreEngine::GetInstance()->Run();
	return 0; //We are not calling delete engine cause well we can't. So this is forcefully shutting down the program which would get the unique ptr out of scope which then would call the default_delete<CoreEngine> that we assigned for the unique ptr 
			  // Yes this means, the core engine destructor is not called i.e. OnDestroy() is not being called i.e. the window ptr is leaking right now.
			  // To solve this, add input and when X is pressed, the destructor will be called
}