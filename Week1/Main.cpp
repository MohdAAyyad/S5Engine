#include "Engine/Core/CoreEngine.h"
#include "Game/Game1.h"
#include "Engine/JSON Reader/JSONReader.h"
#include <iostream>

int main(int argc, char* argv[])
{
	int graphicsFlag = -1;

	std::cout << "Please choose what type of graphics pipeline you wish to use (0 for OpenGL, 1 for Vulkan )" << std::endl;

	while (graphicsFlag != 0 && graphicsFlag != 1)
	{
		std::cin >> graphicsFlag;

		if (graphicsFlag != 0 && graphicsFlag != 1)
		{
			std::cout << "Incorrect input detected. Please choose what type of graphics pipeline you wish to use (0 for Vulkan, 1 for OpenGL)" << std::endl;
		}
		else
		{
			CoreEngine::GetInstance()->SetRendType(static_cast<RendererType>(graphicsFlag));
		}
	}

	
	CoreEngine::GetInstance()->SetGameInterface(new Game1);
	JSONReader::GetInstance()->LoadFile("./Resources/JSONFiles/Test.json");
	std::string  windowTitle = JSONReader::GetInstance()->FindValue<std::string>("window");
	int width = JSONReader::GetInstance()->FindValue<int>("width");
	int height = JSONReader::GetInstance()->FindValue<int>("height");
	int test = JSONReader::GetInstance()->FindNestedValue<int>("scone");


	std::cout << "Loaded Window Name = " << windowTitle << " Loaded Height = " << height << " Loaded Width" << width << std::endl;
	std::cout << test << std::endl;

	if (!CoreEngine::GetInstance()->OnCreate(windowTitle, width, height))
	{
		Debugger::Fatal("Failed to create engine. Quitting program", "Main.cpp", __LINE__);
		return -1;
	}

	CoreEngine::GetInstance()->Run();
	return 0; //We are not calling delete engine cause well we can't. So this is forcefully shutting down the program which would get the unique ptr out of scope which then would call the default_delete<CoreEngine> that we assigned for the unique ptr 
			  // Yes this means, the core engine destructor is not called i.e. OnDestroy() is not being called i.e. the window ptr is leaking right now.
			  // To solve this, add input and when X is pressed, the destructor will be called
}