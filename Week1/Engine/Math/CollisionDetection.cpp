#include "CollisionDetection.h"
#include "Ray.h"
#include "BoundingBox.h"
#include "../Core/CoreEngine.h"





std::vector<glm::vec3> CollisionDetection::simplex = std::vector<glm::vec3>();

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
	Model* m1 = gm1_->GetModel();
	Model* m2 = gm2_->GetModel();

	for (auto v : m1->GetSubMeshes())
	{
		// first vertex list
		std::vector<Vertex> vl1 = v->GetSubMesh().vertexList;
		for (auto v2 : m2->GetSubMeshes())
		{
			std::vector<Vertex> vl2 = v2->GetSubMesh().vertexList;

			if (GJKCalculations(vl1, vl2))
			{
				return true;
			}
			else
			{
				return false;
			}
		}
	}

	simplex.clear();
	return false;
}

bool CollisionDetection::GJKCalculations(std::vector<Vertex> v1_, std::vector<Vertex> v2_)
{
	glm::vec3 add1(0.0f,0.0f,0.0f);
	glm::vec3 add2(0.0f,0.0f,0.0f);

	for (auto a : v1_)
	{
		add1 += a.position;
	}
	for (auto a : v2_)
	{
		add2 += a.position;
	}

	glm::vec3 c1 = add1 / (float)v1_.size();
	glm::vec3 c2 = add2 / (float)v2_.size();


	glm::vec3 dir = c2 - c1;
	// the four points of our simplex


	// I calculate the first two points here because they don't need anything special
	AddToSimplex((Support(v1_, v2_, dir)));

	dir = -dir;

	AddToSimplex(Support(v1_, v2_, dir));


	while (true)
	{
		// is in origin could not find > 3 points so go home
		if (simplex.size() == 0)
		{
			return false;
		}
		if (simplex.size() > 0 && glm::dot(simplex.back(), dir) <= 0)
		{
			// if the last point added was not past the origin then it can't have collided
			return false;
		}
		else
		{
			if (simplex.size() == 4 && ContainsOrigin(simplex[0], simplex[1], simplex[2], simplex[3]))
			{
				return true;
			}
			if (simplex.size() == 2)
			{
				glm::vec3 AB = simplex[1] - simplex[0];
				glm::vec3 AO = glm::vec3(0.0f, 0.0f, 0.0f) - simplex[0];
				dir = (AB * AO) * AB;
				// point3
				AddToSimplex((Support(v1_, v2_, dir)));
			}
			if (simplex.size() == 3)
			{
				// calculate the normal and minius normal
				glm::vec3 AB = simplex[1] - simplex[0];
				glm::vec3 AC = simplex[2] - simplex[0];
				glm::vec3 normal = glm::cross(AB, AC);
				glm::vec3 minusNormal = -normal;

				// find t
				glm::vec3 T = (simplex[0] + simplex[1] + simplex[2]) / 3.0f;
				glm::vec3 TO = glm::vec3(0.0f, 0.0f, 0.0f) - T;

				float n1 = glm::dot(TO, normal);
				float n2 = glm::dot(TO, minusNormal);

				if (glm::dot(TO, normal) > 0)
				{
					dir = normal;
				}
				else
				{
					dir = minusNormal;
				}


				// add the fourth and final point
				AddToSimplex(Support(v1_, v2_, dir));

			}
		}
	}
}

glm::vec3 CollisionDetection::FindFurthestPointInDirection(std::vector<Vertex>v_, glm::vec3 dir_)
{

	bool first = true;
	float furthest;
	glm::vec3 point;
	for (auto v : v_)
	{
		float comp;
		if (first)
		{
			comp = glm::dot(v.position, dir_);
			furthest = comp;
			first = false;
			point = v.position;
		}
		else
		{
			comp = glm::dot(v.position, dir_);

			if (comp > furthest)
			{
				furthest = comp;
				point = v.position;
			}
		}
	}

	return point;

}

glm::vec3 CollisionDetection::MinkowskiDifference(glm::vec3 v1_, glm::vec3 v2_)
{

	return v1_ - v2_;
}

glm::vec3 CollisionDetection::Support(std::vector<Vertex> v1_, std::vector<Vertex> v2_, glm::vec3 dir_)
{
	glm::vec3 p1 = FindFurthestPointInDirection(v1_, dir_);
	glm::vec3 p2 = FindFurthestPointInDirection(v2_, -dir_);

	return MinkowskiDifference(p1, p2);


}

bool CollisionDetection::ContainsOrigin(glm::vec3 p1_, glm::vec3 p2_, glm::vec3 p3_, glm::vec3 p4_)
{
	// create all the matrices needed
	glm::mat4 D0;
	glm::mat4 D1;
	glm::mat4 D2;
	glm::mat4 D3;
	glm::mat4 D4;

	float det0;
	float det1;
	float det2;
	float det3;
	float det4;

	// build the 4 matrices
	D0[0][0] = p1_.x; D0[0][1] = p1_.y; D0[0][2] = p1_.z; D0[0][3] = 1.0f;
	D0[1][0] = p2_.x; D0[1][1] = p2_.y; D0[1][2] = p2_.z; D0[1][3] = 1.0f;
	D0[2][0] = p3_.x; D0[2][1] = p3_.y; D0[2][2] = p3_.z; D0[2][3] = 1.0f;
	D0[3][0] = p4_.x; D0[3][1] = p4_.y; D0[3][2] = p4_.z; D0[3][3] = 1.0f;

	D1[0][0] = 0.0f; D1[0][1] = 0.0f; D1[0][2] = 0.0f; D1[0][3] = 1.0f;
	D1[1][0] = p2_.x; D1[1][1] = p2_.y; D1[1][2] = p2_.z; D1[1][3] = 1.0f;
	D1[2][0] = p3_.x; D1[2][1] = p3_.y; D1[2][2] = p3_.z; D1[2][3] = 1.0f;
	D1[3][0] = p4_.x; D1[3][1] = p4_.y; D1[3][2] = p4_.z; D1[3][3] = 1.0f;

	D2[0][0] = p1_.x; D2[0][1] = p1_.y; D2[0][2] = p1_.z; D2[0][3] = 1.0f;
	D2[1][0] = 0.0f; D2[1][1] = 0.0f; D2[1][2] = 0.0f; D2[1][3] = 1.0f;
	D2[2][0] = p3_.x; D2[2][1] = p3_.y; D2[2][2] = p3_.z; D2[2][3] = 1.0f;
	D2[3][0] = p4_.x; D2[3][1] = p4_.y; D2[3][2] = p4_.z; D2[3][3] = 1.0f;

	D3[0][0] = p1_.x; D3[0][1] = p1_.y; D3[0][2] = p1_.z; D3[0][3] = 1.0f;
	D3[1][0] = p2_.x; D3[1][1] = p2_.y; D3[1][2] = p2_.z; D3[1][3] = 1.0f;
	D3[2][0] = 0.0f; D3[2][1] = 0.0f; D3[2][2] = 0.0f; D3[2][3] = 1.0f;
	D3[3][0] = p4_.x; D3[3][1] = p4_.y; D3[3][2] = p4_.z; D3[3][3] = 1.0f;

	D4[0][0] = p1_.x; D4[0][1] = p1_.y; D4[0][2] = p1_.z; D4[0][3] = 1.0f;
	D4[1][0] = p2_.x; D4[1][1] = p2_.y; D4[1][2] = p2_.z; D4[1][3] = 1.0f;
	D4[2][0] = p3_.x; D4[2][1] = p3_.y; D4[2][2] = p3_.z; D4[2][3] = 1.0f;
	D4[3][0] = 0.0f; D4[3][1] = 0.0f; D4[3][2] = 0.0f; D4[3][3] = 1.0f;

	det0 = glm::determinant(D0);
	det1 = glm::determinant(D1);
	det2 = glm::determinant(D2);
	det3 = glm::determinant(D3);
	det4 = glm::determinant(D4);

	std::cout << " DET0: " << det0 << " DET1: " << det1 << " DET2: " << det2 << " DET3: " << det3 << " DET4: " << det4 << std::endl;

	int matchCount = 0;

	// because 0,0,0 can be a valid position for checking to see if a match is found i use this number as a NULL
	glm::vec3 p1(-FLT_MAX, -FLT_MAX, -FLT_MAX);
	glm::vec3 p2(-FLT_MAX, -FLT_MAX, -FLT_MAX);
	glm::vec3 p3(-FLT_MAX, -FLT_MAX, -FLT_MAX);
	glm::vec3 p4(-FLT_MAX, -FLT_MAX, -FLT_MAX);



	// if det == 0 then there is no tetrahedron that can be formed

	//If by chance the D0=0, then your tetrahedron is degenerate (the points are coplanar).
	if (det0 > 0)
	{
		if (det1 > 0)
		{
			matchCount++;
			p1 = p1_;
		}
		if (det2 > 0)
		{
			matchCount++;
			p2 = p2_;
		}
		if (det3 > 0)
		{
			matchCount++;
			p3 = p3_;
		}
		if (det4 > 0)
		{
			matchCount++;
			p4 = p4_;
		}
	}
	else if (det0 < 0)
	{
		if (det1 < 0)
		{
			matchCount++;
			p1 = p1_;
		}
		if (det2 < 0)
		{
			matchCount++;
			p2 = p2_;
		}
		if (det3 < 0)
		{
			matchCount++;
			p3 = p3_;
		}
		if (det4 < 0)
		{
			matchCount++;
			p4 = p4_;
		}
	}

	if (matchCount == 4)
	{
		return true;
	}
	else if (matchCount == 3)
	{
		std::cout << "__________________________________" << std::endl;
		simplex.clear();

		if (p1 != glm::vec3(-FLT_MAX, -FLT_MAX, -FLT_MAX))
		{
			AddToSimplex(p1);
		}
		if (p2 != glm::vec3(-FLT_MAX, -FLT_MAX, -FLT_MAX))
		{
			AddToSimplex(p2);
		}
		if (p3 != glm::vec3(-FLT_MAX, -FLT_MAX, -FLT_MAX))
		{
			AddToSimplex(p3);
		}
		if (p4 != glm::vec3(FLT_MAX, FLT_MAX, FLT_MAX))
		{
			AddToSimplex(p4);
		}

		// if point 4 was the one that failed recalculate all the points
		if (p4 == glm::vec3(FLT_MAX, FLT_MAX, FLT_MAX))
		{

		}

		return false;
	}
	else
	{
		simplex.clear();

		return false;
	}
}

void CollisionDetection::AddToSimplex(glm::vec3 point)
{
	if (simplex.size() < 4)
	{
		simplex.push_back(point);
		//std::cout << " X: " << point.x << " Y: " << point.y << " Z: " << point.z << std::endl;
	}
	else
	{
		printf("More than 4 points pushed into simplex \n");
	}
}



