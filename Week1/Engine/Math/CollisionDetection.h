#ifndef COLLISIONDETECTION_H
#define COLLISIONDETECTION_H

#include "../Camera/Camera.h"
#include "../Rendering/3D/Mesh.h"
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
	static bool GJK(class GameObject* gm1_, class GameObject* gm2_);
	static bool GJKCalculations(glm::mat4 t1_, glm::mat4 t2_, std::vector<Vertex> v1_, std::vector<Vertex> v2_);
	static glm::vec3 FindFurthestPointInDirection(std::vector<Vertex> v_, glm::vec3 dir_);
	// performs the minkowski difference between two vertex positions
	// helper function for GJK
	static glm::vec3 MinkowskiDifference(glm::vec3 v1_, glm::vec3 v2_);
	static glm::vec3 Support(std::vector<Vertex> v1_, std::vector<Vertex> v2_, glm::vec3 dir_);
	static bool ContainsOrigin(glm::vec3 p1_, glm::vec3 p2_, glm::vec3 p3_, glm::vec3 p4_);



	static void AddToSimplex(glm::vec3 point);
private:
	static std::vector<glm::vec3> simplex;


	
};


#endif // !COLLISIONDETECTION_H
