#include "StartScene.h"

StartScene::StartScene(): Scene() //Call the parent's constructor first. Yes we always create a parent object with every child object. Memory-consuming but that's how it is. Another school of thought: templates
{

}
StartScene::~StartScene()
{

}
bool StartScene::OnCreate()
{
	Debugger::Info("Created a start scene!", "StartScene.cpp", __LINE__);
	CoreEngine::GetInstance()->SetCurrentScene(1); //Switch scene to scene 1
	return true;
}
void StartScene::Update(const float deltaTime_)
{

}
void StartScene::Render()
{

}