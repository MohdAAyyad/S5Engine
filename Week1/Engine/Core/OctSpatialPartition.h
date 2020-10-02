#ifndef OCTSPATIALPARTITION_H
#define OCTSPATIALPARTITION_H

#define CHILDREN_NUMBER 8

#include "../Math/Ray.h"


class GameObject;

enum class OctChildren
{
	OCT_TLF, //Top left front
	OCT_BLF, //bottom left front
	OCT_BRF, //bottom right front
	OCT_TRF, //top right front
	OCT_TLR, //top left rear
	OCT_BLR, //bottom left rear
	OCT_BRR, //bottom right rear
	OCT_TRR  //top right rear
};

class OctNode
{
public:
	OctNode(glm::vec3 pos_, float size_, OctNode* parent_);
	~OctNode();
	void OnDestroy();
	void Octify(int depth_); //Create all the children. Recursive function
	OctNode* GetParent();
	OctNode* GetChild(OctChildren childPos_);
	void AddCollisionObject(GameObject* obj_);
	int GetObjectCount() const;
	bool IsLeaf() const;
	BoundingBox* GetBoundingBox() const;
	int GetChildCount() const;
private:
	friend class OctSpatialPartition; //Make OctSpatialPartition can access the private functions of OctNode
	BoundingBox* octBox;
	OctNode* parent;
	OctNode* children[CHILDREN_NUMBER]; //An array and not a vector as we already know it's always gonna be 8 children no more no less
	std::vector<GameObject*>objectList;
	float size;
	static int childNum; //Used to confirm the total number of children in a single tree

};


class OctSpatialPartition
{
public:
	OctSpatialPartition(float worldSize_);
	~OctSpatialPartition();
	void AddObject(GameObject* obj_);
	GameObject* GetCollision(Ray ray_);
private:
	OctNode* root;
	std::vector<OctNode*> rayIntesectionList;
	void AddObjectToCell(OctNode* cell_, GameObject* obj_); //Helper function for AddObject
	void PrepareCollisionQuery(OctNode* cell_, Ray ray_);
	void OnDestroy();
};


#endif // !OCTSPATIALPARTITION

