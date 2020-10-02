#ifndef BOUNDINGBOX_H
#define BOUNDINGBOX_H

#include <glm/glm.hpp>

struct BoundingBox
{
	glm::vec3 maxVert, minVert;
	glm::mat4 transform;

	inline BoundingBox()
	{
		maxVert = minVert = glm::vec3();
		transform = glm::mat4();
	}
	inline BoundingBox(glm::vec3 maxVert_, glm::vec3 minVert_, glm::mat4 transform_)
	{
		maxVert = maxVert_;
		minVert = minVert_;
		transform = transform_;
	}

	inline bool Intersects(BoundingBox* box_)
	{
		glm::vec3 minCorner = GetTransformPoint(minVert, transform);
		glm::vec3 maxCorner = GetTransformPoint(maxVert, transform);

		glm::vec3 otherMinCorner = GetTransformPoint(box_->minVert, box_->transform);
		glm::vec3 otherMaxCorner = GetTransformPoint(box_->maxVert, box_->transform);

		//Return the result of collision detection between the two boxes (AABB collision detection)
		return 
			(minCorner.x <= otherMaxCorner.x && maxCorner.x >= otherMinCorner.x) &&
			(minCorner.y <= otherMaxCorner.x && maxCorner.y >= otherMinCorner.y) &&
			(minCorner.z <= otherMaxCorner.y && maxCorner.z >= otherMinCorner.z);
	}

private:
	inline glm::vec3 GetTransformPoint(glm::vec3 point_, glm::mat4 transform_)
	{
		return glm::vec3(transform[3].x,transform[3].y,transform[3].z) + point_; //Translate the point based on the OOB's position
	}
};
#endif // !BOUNDINGBOX_H

