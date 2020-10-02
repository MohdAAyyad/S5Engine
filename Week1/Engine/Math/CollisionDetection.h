#ifndef COLLISIONDETECTION_H
#define COLLISIONDETECTION_H

#include "../Camera/Camera.h"

struct Ray;
struct BoundingBox;
class CollisionDetection
{
public:
	CollisionDetection() = delete;
	CollisionDetection(const CollisionDetection&) = delete; //Disable the copy constructor
	CollisionDetection(CollisionDetection&&) = delete; //Disable the move constructor
	CollisionDetection& operator=(const CollisionDetection&) = delete; //Disable operator = for copy and move
	CollisionDetection& operator=(CollisionDetection&&) = delete;

	~CollisionDetection();

	static Ray ScreenPosToWorldRay(glm::vec2 mouseCoords_, glm::vec2 screenSize_, Camera* camera_);
	static bool RayObbIntersection(Ray* ray_, BoundingBox* obb_);
};


#endif // !COLLISIONDETECTION_H
