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

bool CollisionDetection::GJK(GameObject* gm1_, GameObject* gm2_)
{
	SubMesh m1 = gm1_->GetModel()->GetSubMeshes()[0]->GetSubMesh();
	SubMesh m2 = gm2_->GetModel()->GetSubMeshes()[0]->GetSubMesh();

	glm::vec3 d1 = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 d2 = glm::vec3(0.0f, 0.0f, 0.0f);

	glm::vec3 direction(glm::vec3(0.0f, 0.0f, 0.0f));
	// direction calculation

	//NEED TO REDO THIS PART
	for (auto m : gm1_->GetModel()->GetSubMeshes())
	{
		//m1 = m->GetSubMesh();
		for (auto v : m1.vertexList)
		{
			d1 += v.position;
		}
	}

	d1 /= m1.vertexList.size();

	// same process for the second model
	for (auto m : gm2_->GetModel()->GetSubMeshes())
	{
		//m2 = m->GetSubMesh();
		for (auto v : m2.vertexList)
		{
			d2 += v.position;
		}
	}

	d2 /= m2.vertexList.size();




	// set our direction
	direction = d1 - d2;

	// now perform simplex
	glm::vec3 point1 = Support(m1.vertexList, m2.vertexList, direction);
	// inverse the direction so we go in the opposite direction
	direction *= -1.0f;
	glm::vec3 point2 = Support(m1.vertexList, m2.vertexList, direction);

	// now find a point perpendicular to the two points

	glm::vec3 ab = point2 - point1;
	glm::vec3 ao = point1 - glm::vec3(0.0f, 0.0f, 0.0f);

	direction = (ab * ao) * ab;

	glm::vec3 point3 = Support(m1.vertexList, m2.vertexList, direction);


	while (true)
	{
		glm::vec3 ac = point3 - point1;

		glm::vec3 normal = glm::cross(ab, ac);
		// okay so now we got three points, lets find the fourth, to do so we need to find the fourth point in a separate function and check if it encloses the origin

		std::vector<glm::vec3> result = IsInOrigin(point1, point2, point3, FindFourthPoint(point1, point2, point3, normal, m1.vertexList, m2.vertexList));
		if (result.size() == 4)
		{
			return true;
		}
		else if (result.size() == 3)
		{
			// if we found more than three points than we have a triangle that encloses the origin go and try to find the point
			point1 = result[0];
			point2 = result[1];
			point3 = result[2];
			continue;
		}
		else
		{
			// we couldn't find more than three points so it's useless go home
			return false;
		}
	}

	return false;
}

glm::vec3 CollisionDetection::MinkowskiDifference(glm::vec3 v1_, glm::vec3 v2_)
{

	return v1_ - v2_;
}

glm::vec3 CollisionDetection::Support(std::vector<Vertex> v1_, std::vector<Vertex> v2_, glm::vec3 direction_)
{
	// note we don't have to normalize the direction we can input any direction and it will get us a point on the other side
	glm::vec3 p1;
	glm::vec3 p2;

	float current1 = 0;
	float current2 = 0;

	bool first = true;
	for (auto v : v1_)
	{
		if (first)
		{
			current1 = glm::dot(v.position, direction_);
			p1 = v.position;
			first = false;
		}
		else if(int i = glm::dot(v.position, direction_) > current1)
		{
			current1 = i;
			p1 = v.position;
		}
	}

	first = true;

	// repeate for second point but instead look for the smallest point
	for (auto v : v2_)
	{
		if (first)
		{
			current2 = glm::dot(v.position, direction_);
			p2 = v.position;
			first = false;
		}
		else if (int i = glm::dot(v.position, direction_) < current2)
		{
			current2 = i;
			p2 = v.position;
		}
	}

	// calculate the minkowski difference between the two points

	return MinkowskiDifference(p1, p2);


}

glm::vec3 CollisionDetection::FindFourthPoint(glm::vec3 p1_, glm::vec3 p2_, glm::vec3 p3_, glm::vec3 normal_, std::vector<Vertex> v1_, std::vector<Vertex> v2_)
{
	glm::vec3 normal1 = -normal_;

	glm::vec3 T = (p1_ + p2_ + p3_) / 3.0f;

	glm::vec3 TO = -T;

	glm::vec3 normalFinal;


	// find t
	if (glm::dot(normal_, TO) > glm::dot(normal1, TO) && glm::dot(normal_, TO) > 0)
	{
		normalFinal = normal1;
	}
	else if (glm::dot(normal1, TO) > glm::dot(normal_, TO) && glm::dot(normal1, TO) > 0)
	{
		normalFinal = normal_;
	}
	else if (glm::dot(normal1, TO) < glm::dot(normal_, TO) && glm::dot(normal1, TO) < 0)
	{
		normalFinal = normal_;
	}
	else if (glm::dot(normal_, TO) < glm::dot(normal1, TO) && glm::dot(normal_, TO) < 0)
	{
		normalFinal = normal1;
	}


	return Support(v1_, v2_, normalFinal);
}

std::vector<glm::vec3> CollisionDetection::IsInOrigin(glm::vec3 p1_, glm::vec3 p2_, glm::vec3 p3_, glm::vec3 p4_)
{
	std::vector<glm::vec3> matching;
	
	glm::mat4 D0;
	D0[0][0] = p1_.x; D0[0][1] = p1_.y; D0[0][2] = p1_.z; D0[0][3] = 1.0f;
	D0[1][0] = p2_.x; D0[1][1] = p2_.y; D0[1][2] = p2_.z; D0[1][3] = 1.0f;
	D0[2][0] = p3_.x; D0[2][1] = p3_.y; D0[2][2] = p3_.z; D0[2][3] = 1.0f;
	D0[3][0] = p4_.x; D0[3][1] = p4_.y; D0[3][2] = p4_.z; D0[3][3] = 1.0f;

	float det0 = glm::determinant(D0);

	glm::mat4 D1;

	D1[0][0] = 0.0f; D1[0][1] = 0.0f; D1[0][2] = 0.0f; D1[0][3] = 1.0f;
	D1[1][0] = p2_.x; D1[1][1] = p2_.y; D1[1][2] = p2_.z; D1[1][3] = 1.0f;
	D1[2][0] = p3_.x; D1[2][1] = p3_.y; D1[2][2] = p3_.z; D1[2][3] = 1.0f;
	D1[3][0] = p4_.x; D1[3][1] = p4_.y; D1[3][2] = p4_.z; D1[3][3] = 1.0f;

	float det1 = glm::determinant(D1);

	glm::mat4 D2;

	D2[0][0] = p1_.x; D2[0][1] = p1_.y; D2[0][2] = p1_.z; D2[0][3] = 1.0f;
	D2[1][0] = 0.0f; D2[1][1] = 0.0f; D2[1][2] = 0.0f; D2[1][3] = 1.0f;
	D2[2][0] = p3_.x; D2[2][1] = p3_.y; D2[2][2] = p3_.z; D2[2][3] = 1.0f;
	D2[3][0] = p4_.x; D2[3][1] = p4_.y; D2[3][2] = p4_.z; D2[3][3] = 1.0f;

	float det2 = glm::determinant(D2);



	glm::mat4 D3;

	D3[0][0] = p1_.x; D3[0][1] = p1_.y; D3[0][2] = p1_.z; D3[0][3] = 1.0f;
	D3[1][0] = p2_.x; D3[1][1] = p2_.y; D3[1][2] = p2_.z; D3[1][3] = 1.0f;
	D3[2][0] = 0.0f; D3[2][1] = 0.0f; D3[2][2] = 0.0f; D3[2][3] = 1.0f;
	D3[3][0] = p4_.x; D3[3][1] = p4_.y; D3[3][2] = p4_.z; D3[3][3] = 1.0f;

	float det3 = glm::determinant(D3);


	glm::mat4 D4;

	D4[0][0] = p1_.x; D4[0][1] = p1_.y; D4[0][2] = p1_.z; D4[0][3] = 1.0f;
	D4[1][0] = p2_.x; D4[1][1] = p2_.y; D4[1][2] = p2_.z; D4[1][3] = 1.0f;
	D4[2][0] = p3_.x; D4[2][1] = p3_.y; D4[2][2] = p3_.z; D4[2][3] = 1.0f;
	D4[3][0] = 0.0f; D4[3][1] = 0.0f; D4[3][2] = 0.0f; D4[3][3] = 1.0f;

	float det4 = glm::determinant(D4);

	//check to see if each of the determinants enclose the origin

	// how many determinants match the sign of our d0

	if (det0 >= 0)
	{
		if (det1 >= 0)
		{
			// match
			matching.push_back(p1_);
		}

		if (det2 >= 0)
		{
			// match
			matching.push_back(p2_);
		}

		if (det3 >= 0)
		{
			// match
			matching.push_back(p3_);
		}

		if (det4 >= 0)
		{
			// match
			matching.push_back(p4_);
		}
	}
	else if (det0 < 0)
	{
		if (det1 < 0)
		{
			// match
			matching.push_back(p1_);
		}

		if (det2 < 0)
		{
			// match
			matching.push_back(p2_);
		}

		if (det3 < 0)
		{
			// match
			matching.push_back(p3_);
		}

		if (det4 < 0)
		{
			// match
			matching.push_back(p4_);
		}
	}


	// if all four points share the same sign then we for sure have a collision
	return matching;
}



