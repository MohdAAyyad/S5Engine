#include "Game1.h"

Game1::Game1():GameInterface(), currentSceneNum(0),currentScene(nullptr)
{

}
Game1:: ~Game1()
{
	delete currentScene;
	currentScene = nullptr;
}
void Game1::Update(const float deltaTime_)
{
	if (currentSceneNum != CoreEngine::GetInstance()->GetCurrentScene()) //Always make sure you're matching the engine's scene number
	{
		BuildScene();
	}

	currentScene->Update(deltaTime_);
}
void Game1::Render()
{
	if (currentScene)
	{
		currentScene->Render();
	}

}
bool Game1::OnCreate()
{
	if (CoreEngine::GetInstance()->GetCurrentScene() == 0) 
	{
		currentScene = new StartScene();
		currentSceneNum = 0;
		return (currentScene->OnCreate());
	}

	Debugger::Fatal("Engine scene is not initialized to zero", "Game1.cpp", __LINE__);
	return false;
 }

void Game1::BuildScene()
{
	//Reset the current scene pointer as the current scene is not the same one as the core engine so it needs to be deleted
	delete currentScene;
	currentScene = nullptr;

	switch (CoreEngine::GetInstance()->GetCurrentScene())
	{
	case 1:
		currentScene = new GameScene();
		break;
	default:
		currentScene = new StartScene();
		break;
	}

	currentSceneNum = CoreEngine::GetInstance()->GetCurrentScene();
	
	if (!currentScene->OnCreate()) 
	{
		Debugger::Fatal("Failed to create scene in Build Scene", "Game1.cpp", __LINE__);
		CoreEngine::GetInstance()->Exit();//If something has gone wrong creating the scene, then exit.
	}
}