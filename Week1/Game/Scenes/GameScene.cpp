#include "../../Engine/Math/CollisionHandler.h"
#include "GameScene.h"
#include "../../Engine/Rendering/3D/Components/PhysicsComponent.h"
#include "../../Engine/Rendering/GUI/GUIObject.h"
#include "../../Engine/Rendering/GUI/Components/GUIImageComponent.h"

GameScene::GameScene(): Scene()
{
}
GameScene:: ~GameScene()
{
	SceneGraph::GetInstance()->OnDestroy();
}
 bool GameScene::OnCreate()
 {
	 Debugger::Info("Created GameScene!", "GameScene.cpp", __LINE__);

	 CoreEngine::GetInstance()->SetCamera(new Camera);
	 CoreEngine::GetInstance()->GetCamera()->SetPosition(glm::vec3(0.0f, 0.0f, 4.0f));
	 CoreEngine::GetInstance()->GetCamera()->AddLightSource(new LightSource(glm::vec3(0.0f, 1.0f, 4.0f), glm::vec3(1.0f, 1.0f, 1.0f), 0.1f, 0.5f));
	 CollisionHandler::GetInstance()->OnCreate(100.0f);

	 //Creating the object -->This is not the correct way to do it, you should have an obj importer to get the vertices

	 Model* model = new Model("./Resources/Models/Apple.obj","./Resources/Materials/Apple.mtl", ShaderHandler::GetInstance()->GetShader("basicShader")); //Creating a pointer inside a function can cause a memory leak if you don't delete. In this case, however, this is ok because the model is linked with a gameobject which will delete it on destroy.	 shape = new GameObject(model);
	 Model* model2 = new Model("./Resources/Models/Dice.obj", "./Resources/Materials/Dice.mtl", ShaderHandler::GetInstance()->GetShader("basicShader")); //Creating a pointer inside a function can cause a memory leak if you don't delete. In this case, however, this is ok because the model is linked with a gameobject which will delete it on destroy.	 shape = new GameObject(model);

	 SceneGraph::GetInstance()->AddModel(model);
	 SceneGraph::GetInstance()->AddModel(model2);

	 GameObject* appleObj = new GameObject(model, glm::vec3(2.0f, -0.5f, 0.0f));
	 GameObject* appleObj2 = new GameObject(model, glm::vec3(0.0f, -0.5f, 0.0f));
	 
	 appleObj->SetScale(glm::vec3(0.4f, 0.4f, 0.4f));
	 appleObj->AddComponent<PhysicsComponent*>(new PhysicsComponent());
	 appleObj->GetComponent<PhysicsComponent*>()->SetupScenario(10, glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(glm::radians(45.0f)), glm::vec3(0.0f, 1.0f, 0.0f), 45.0f, 2.0f);

	 appleObj2->SetScale(glm::vec3(0.4f, 0.4f, 0.4f));
	 appleObj2->AddComponent<PhysicsComponent*>(new PhysicsComponent());
	 appleObj2->GetComponent<PhysicsComponent*>()->SetupScenario(10, glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(glm::radians(45.0f)), glm::vec3(0.0f, 1.0f, 0.0f), 45.0f, 2.0f);

	 SceneGraph::GetInstance()->AddGameObject(appleObj, "apple");
	 SceneGraph::GetInstance()->AddGameObject(appleObj2, "apple2");


	 GUIObject* guiObj = new GUIObject(glm::vec2(400.0f, 300.0f));
	 guiObj->AddComponent<GUIImageComponent*>(new GUIImageComponent());
	 GUIImageComponent* imageComp = guiObj->GetComponent<GUIImageComponent*>();
	 if (imageComp)
	 {
		 imageComp->OnCreate("BarYellow");
	 }

	 SceneGraph::GetInstance()->AddGUIObject(guiObj,"barYellow1");

	 //The OpenGL will color the corners and will interpolate the color in between the corners. This is because we only told it what the corners are colored as.
	 return true;
 }
 void GameScene::Update(const float deltaTime_)
 {
	 SceneGraph::GetInstance()->Update(deltaTime_);
 }
 void GameScene::Render()
 {
	 //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	 SceneGraph::GetInstance()->Render(CoreEngine::GetInstance()->GetCamera());
 }

 void GameScene::Draw()
 {
	 SceneGraph::GetInstance()->Draw(CoreEngine::GetInstance()->GetCamera());
 }
