#include "CollisionDetection.h"
#include "Ray.h"
#include "BoundingBox.h"
#include "../Core/CoreEngine.h"

Ray CollisionDetection::ScreenPosToWorldRay(glm::vec2 mouseCoords_, glm::vec2 screenSize_, Camera* camera_)
{

	glm::vec4 rayStart_NDC((mouseCoords_.x / screenSize_.x - 0.5f)*2.0f, // -0.5f makes sure NDC is from -1 to 1 and not from -1 to 0
					       (mouseCoords_.y / screenSize_.y - 0.5f)*2.0f, -1.0f, 1.0f);

	glm::vec4 rayEnd_NDC((mouseCoords_.x / screenSize_.x - 0.5f)*2.0f,
		(mouseCoords_.y / screenSize_.y - 0.5f)*2.0f, 0.0f, 1.0f); // The end is zero i.e. the middle of the NDC cube (remember NDC cube goes from -1 to 1)

	glm::mat4 inverse = glm::inverse(camera_->GetPerspective() * camera_->GetView()); //Going to clip from world we go view then projection so to go from clip to world, we go inverse of projection then view 

	glm::vec4 rayStart_World = inverse * rayStart_NDC;
	rayStart_World /= rayStart_World.w; //going from vec4 to vec3, divide by W

	glm::vec4 rayEnd_World = inverse * rayEnd_NDC;
	rayEnd_World /= rayEnd_World.w;

	glm::vec3 rayDir_World(rayEnd_World - rayStart_World);
	rayDir_World = glm::normalize(rayDir_World);

	return Ray(glm::vec3(rayStart_World), rayDir_World);
}

CollisionDetection::~CollisionDetection()
{

}
bool CollisionDetection::RayObbIntersection(Ray* ray_, BoundingBox* box_)
{
	float tMin = CoreEngine::GetInstance()->GetCamera()->GetClippingPlanes().x; //The largest near intersection found
	float tMax = CoreEngine::GetInstance()->GetCamera()->GetClippingPlanes().y; //The smallest far intersection found (it's the futhest negative possible number)

	glm::vec3 oobPosition_World(box_->transform[3].x, box_->transform[3].y, box_->transform[3].z); //The third column is the position of the model
	//std::cout << oobPosition_World.x << " " << oobPosition_World.y << " " << oobPosition_World.z << std::endl;
	glm::vec3 delta = oobPosition_World - ray_->origin;

	//X axis
	glm::vec3 xAxis(box_->transform[0].x, box_->transform[0].y, box_->transform[0].z);

	float e = glm::dot(xAxis, delta);
	float f = glm::dot(ray_->direction, xAxis);
	if (fabs(f) > 0.001f)
	{
		//Left plane
		float t1 = (e + box_->minVert.x) / f;
		//Right plane
		float t2 = (e + box_->maxVert.x) / f;

		//If the left is larger than the right, swap
		if (t1 > t2)
		{
			float w = t1;
			t1 = t2;
			t2 = w;
		}
		
		if (t2 < tMax)
		{
			tMax = t2; //tMax is the smallest far intersection
		}

		if (t1 > tMin)
		{
			tMin = t1; //tMin is the largest near intersection
		}

		if (tMax < tMin) //If the far is closer than the min, then there's no collision, return false
		{
			return false;
		}
	}
	else //Check for the rare case when the ray is parallel to the box
	{
		if (-e + box_->minVert.x > 0.0f || -e + box_->maxVert.x < 0.0f)
		{
			return false;
		}
	}

	//Y axis
	glm::vec3 yAxis(box_->transform[1].x, box_->transform[1].y, box_->transform[1].z);

	 e = glm::dot(yAxis, delta);
	 f = glm::dot(ray_->direction, yAxis);
	if (fabs(f) > 0.001f)
	{
		//Bottom plane
		float t1 = (e + box_->minVert.y) / f;
		//Right plane
		float t2 = (e + box_->maxVert.y) / f;

		//If the left is larger than the right, swap
		if (t1 > t2)
		{
			float w = t1;
			t1 = t2;
			t2 = w;
		}

		if (t2 < tMax)
		{
			tMax = t2; //tMax is the smallest far intersection
		}

		if (t1 > tMin)
		{
			tMin = t1; //tMin is the largest near intersection
		}

		if (tMax < tMin) //If the far is closer than the min, then there's no collision, return false
		{
			return false;
		}
	}
	else //Check for the rare case when the ray is parallel to the box
	{
		if (-e + box_->minVert.y > 0.0f || -e + box_->maxVert.y < 0.0f)
		{
			return false;
		}
	}

	//Zaxis

	glm::vec3 zAxis(box_->transform[2].x, box_->transform[2].y, box_->transform[2].z);

	e = glm::dot(zAxis, delta);
	f = glm::dot(ray_->direction, zAxis);
	if (fabs(f) > 0.001f)
	{
		//Left plane
		float t1 = (e + box_->minVert.z) / f;
		//Right plane
		float t2 = (e + box_->maxVert.z) / f;

		//If the left is larger than the right, swap
		if (t1 > t2)
		{
			float w = t1;
			t1 = t2;
			t2 = w;
		}

		if (t2 < tMax)
		{
			tMax = t2; //tMax is the smallest far intersection
		}

		if (t1 > tMin)
		{
			tMin = t1; //tMin is the largest near intersection
		}

		if (tMax < tMin) //If the far is closer than the min, then there's no collision, return false
		{
			return false;
		}
	}
	else //Check for the rare case when the ray is parallel to the box
	{
		if (-e + box_->minVert.z > 0.0f || -e + box_->maxVert.z < 0.0f)
		{
			return false;
		}
	}

	//We got a collision

	ray_->intersectionDistance = tMin;
	return true;

}