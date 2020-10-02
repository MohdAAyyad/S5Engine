#ifndef RAY_H
#define RAY_H

#include <glm/glm.hpp>
#include "BoundingBox.h"
#include "CollisionDetection.h"

struct Ray
{
	glm::vec3 origin;
	glm::vec3 direction;

	float intersectionDistance; //The distance between the start of the ray until collision

	inline Ray()
	{
		origin = glm::vec3();
		direction = glm::vec3();
		intersectionDistance = 0.0f;
	}

	inline Ray(glm::vec3 origin_, glm::vec3 direction_)
	{
		origin = origin_;
		direction = direction_;
		intersectionDistance = 0.0f; //It's always zero to make sure each ray has its own unique distance
	}

	Ray& operator=(const Ray& ray_)
	{
		origin = ray_.origin;
		direction = ray_.direction;
		intersectionDistance = 0.0f;
	}

	inline bool IsColliding(BoundingBox* box_)
	{
		intersectionDistance = -1.0f;
		//Call collision system

		return CollisionDetection::RayObbIntersection(this,box_);
	}
};

#endif // !RAY_H
