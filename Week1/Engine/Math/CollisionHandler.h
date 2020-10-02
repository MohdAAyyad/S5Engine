#ifndef COLLISIONHANDLER_H
#define COLLISIONHANDLER_H

#include "Ray.h"
#include "../Core/OctSpatialPartition.h"
#include <memory>


class GameObject;
class CollisionHandler
{
public:
	CollisionHandler(const CollisionHandler&) = delete; //Disable the copy constructor
	CollisionHandler(CollisionHandler&&) = delete; //Disable the move constructor
	CollisionHandler& operator=(const CollisionHandler&) = delete; //Disable operator = for copy and move
	CollisionHandler& operator=(CollisionHandler&&) = delete;

	static CollisionHandler* GetInstance();

	void OnCreate(float worldSize_);
	void AddObject(GameObject* obj_);
	void MouseUpdate(glm::vec2 mousePos_, int buttonType_);
	void OnDestroy();

private:
	CollisionHandler();
	~CollisionHandler();

	static std::unique_ptr<CollisionHandler> instance;
	friend std::default_delete<CollisionHandler>;

	static std::vector<GameObject*> prevCollisions;
	static OctSpatialPartition* scenePartition;
};

#endif // !COLLISIONHANDLER_H

