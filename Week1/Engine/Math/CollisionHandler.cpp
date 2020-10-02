#include "../Core/CoreEngine.h"
#include "CollisionHandler.h"


std::unique_ptr<CollisionHandler> CollisionHandler::instance = nullptr;
std::vector<GameObject*> CollisionHandler::prevCollisions = std::vector<GameObject*>();
OctSpatialPartition* CollisionHandler::scenePartition = nullptr;

CollisionHandler::CollisionHandler()
{
	prevCollisions.reserve(10);
}

CollisionHandler::~CollisionHandler()
{
	OnDestroy();
}

void CollisionHandler::OnDestroy()
{
	if (prevCollisions.size() > 0)
	{
		for (auto m : prevCollisions)
		{
			m = nullptr; //GameObjects will be deleted from the ScenGraph
		}
		prevCollisions.clear();
	}

	if (scenePartition)
	{
		delete scenePartition;
		scenePartition = nullptr;
	}
}

void CollisionHandler::OnCreate(float worldSize_)
{
	prevCollisions.clear();
	scenePartition = new OctSpatialPartition(worldSize_);
}
void CollisionHandler::AddObject(GameObject* obj_)
{
	scenePartition->AddObject(obj_);
}
void CollisionHandler::MouseUpdate(glm::vec2 mousePos_, int buttonType_)
{
	Ray mouseRay = CollisionDetection::ScreenPosToWorldRay(mousePos_, CoreEngine::GetInstance()->GetWindowSize(), CoreEngine::GetInstance()->GetCamera());

	if (scenePartition)
	{
		GameObject* hitResult = scenePartition->GetCollision(mouseRay);

		if (hitResult)
		{
			hitResult->SetHit(true, buttonType_);
		}

		for (auto c : prevCollisions)
		{
			if (c != hitResult && c != nullptr)
			{
				c->SetHit(false, buttonType_);
			}
			c = nullptr;
		}
		prevCollisions.clear();

		if (hitResult)
		{
			prevCollisions.push_back(hitResult);
		}
	}	
}


CollisionHandler* CollisionHandler::GetInstance()
{
	if (!instance)
		instance.reset(new CollisionHandler());
	return instance.get();
}