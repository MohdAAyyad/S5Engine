#include "OctSpatialPartition.h"
#include "../Rendering/3D/GameObject.h"

int OctNode::childNum = 0;

OctNode::OctNode(glm::vec3 pos_, float size_, OctNode* parent_): octBox(nullptr), parent(nullptr), children(), //Notice how you initialze an empty array in the pre constructor initilization
																 objectList(std::vector<GameObject*>())
{
	objectList.reserve(10);
	octBox = new BoundingBox();
	octBox->minVert = pos_;
	octBox->maxVert = pos_ + glm::vec3(size_);
	size = size_;
	parent = parent_;
	for (int i = 0; i < CHILDREN_NUMBER; i++)
	{
		children[i] = nullptr;
	}
}
OctNode::~OctNode()
{
	OnDestroy();
}
void OctNode::OnDestroy()
{
	delete octBox;
	octBox = nullptr;

	if (objectList.size() > 0)
	{
		for (auto obj : objectList)
			obj = nullptr; //No deletion of gameobjects here. It's done in the scene graph

		objectList.clear();
	}

	for (int i = 0; i < CHILDREN_NUMBER; i++)
	{
		if (children[i])
		{
			delete children[i];
			children[i] = nullptr;
		}
	}
	
}
void OctNode::Octify(int depth_)//Create all the children. Recursive function
{
	if (depth_ > 0 && this)
	{
		float half = size / 2.0f;
		children[static_cast<int>(OctChildren::OCT_TLF)] = new OctNode
		(glm::vec3(octBox->minVert.x, octBox->minVert.y + half, octBox->minVert.z + half), half, this);
		//TODO
		//Finish up the other 7 children
		children[static_cast<int>(OctChildren::OCT_TLR)] = new OctNode
		(glm::vec3(octBox->minVert.x, octBox->minVert.y + half, octBox->minVert.z), half, this);

		children[static_cast<int>(OctChildren::OCT_TRF)] = new OctNode
		(glm::vec3(octBox->minVert.x + half, octBox->minVert.y + half, octBox->minVert.z + half), half, this);

		children[static_cast<int>(OctChildren::OCT_TRR)] = new OctNode
		(glm::vec3(octBox->minVert.x + half, octBox->minVert.y + half, octBox->minVert.z), half, this);

		children[static_cast<int>(OctChildren::OCT_BLF)] = new OctNode
		(glm::vec3(octBox->minVert.x, octBox->minVert.y, octBox->minVert.z + half), half, this);

		children[static_cast<int>(OctChildren::OCT_BRF)] = new OctNode
		(glm::vec3(octBox->minVert.x + half, octBox->minVert.y, octBox->minVert.z + half), half, this);

		children[static_cast<int>(OctChildren::OCT_BLR)] = new OctNode
		(glm::vec3(octBox->minVert.x, octBox->minVert.y, octBox->minVert.z), half, this);

		children[static_cast<int>(OctChildren::OCT_BRR)] = new OctNode
		(glm::vec3(octBox->minVert.x + half, octBox->minVert.y, octBox->minVert.z), half, this);

		childNum += 8;

		for (int i = 0; i < CHILDREN_NUMBER; i++)
		{
			children[i]->Octify(depth_ - 1); //Each child will go and create its own children until the depth is negative
		}
	}
} 
OctNode* OctNode::GetParent()
{
	return parent;

}
OctNode* OctNode::GetChild(OctChildren childPos_)
{
	return children[static_cast<int>(childPos_)];
}
void OctNode::AddCollisionObject(GameObject* obj_)
{
	objectList.push_back(obj_);
}
int OctNode::GetObjectCount() const
{
	return objectList.size();
}
bool OctNode::IsLeaf() const
{
	if (children[0] == nullptr) //This has no children i.e. a leaf
		return true;
	return false;

}
BoundingBox* OctNode::GetBoundingBox() const
{
	return octBox;
}
int OctNode::GetChildCount() const
{
	return childNum;
}

OctSpatialPartition::OctSpatialPartition(float worldSize_): root(nullptr), rayIntesectionList(std::vector<OctNode*>())
{
	root = new OctNode(glm::vec3(-(worldSize_ / 2.0f)), worldSize_, nullptr);
	root->Octify(3);
	std::cout << "There are " << root->GetChildCount() << " child nodes" << std::endl;

	rayIntesectionList.reserve(20);
}
void OctSpatialPartition::OnDestroy()
{
	if (rayIntesectionList.size() > 0)
	{
		for (auto c : rayIntesectionList)
			c = nullptr;
		rayIntesectionList.clear();
	}

	delete root;
	root = nullptr;
}
OctSpatialPartition::~OctSpatialPartition()
{

}
void OctSpatialPartition::AddObject(GameObject* obj_)
{
	AddObjectToCell(root, obj_); //Check to see at a leaf level what object the ray collided with (i.e. collision is checked only on leaves)
}
GameObject* OctSpatialPartition::GetCollision(Ray ray_)
{
	if (rayIntesectionList.size() > 0)
	{
		for (auto c : rayIntesectionList)
			c = nullptr;
		rayIntesectionList.clear();
	}
	PrepareCollisionQuery(root, ray_);
	float shortestDistance = FLT_MAX;
	GameObject* result = nullptr;
	for (auto c : rayIntesectionList)
	{
		for (auto obj : c->objectList)//We can get object list directily because octspatialpartition is a friend class
		{
			//For eact object inside that node, does the ray collide with it?
			if (ray_.IsColliding(&obj->GetBoundingBox()))
			{
				if (ray_.intersectionDistance < shortestDistance) //Is the collision we're looking at the closest collision to the ray?
				{
					result = obj;
					shortestDistance = ray_.intersectionDistance;
				}
			}
		}
		if (result)
			return result;
	}
	return nullptr;
}
void OctSpatialPartition::AddObjectToCell(OctNode* cell_, GameObject* obj_)
{
	//Check to see if this gameobject intersect with the cell that's passed in
	if (cell_ && obj_)
	{
		if (obj_->GetBoundingBox().Intersects(cell_->GetBoundingBox())) //Check for collision
		{
			if (cell_->IsLeaf()) //Is it a leaf?
			{
				cell_->AddCollisionObject(obj_);  //if it is, add the object to the leaf node
				return;
			}
			for (int i = 0; i < CHILDREN_NUMBER; i++) //Otherwise, check its children
			{
				AddObjectToCell(cell_->children[i], obj_);
			}
		}
	}
}
void OctSpatialPartition::PrepareCollisionQuery(OctNode* cell_, Ray ray_)
{
	//Check to see if the ray collides with the cell's bounding box
	if (cell_)
	{
		if (ray_.IsColliding(cell_->GetBoundingBox()))
		{
			if (cell_->IsLeaf()) //Is it a leaf?
			{
				rayIntesectionList.push_back(cell_);//If yes, add it the ray to the intersection list
				//std::cout << "Added ray to intersection list" << std::endl;
				return;
			}
			for (int i = 0; i < CHILDREN_NUMBER; i++) //Otherwise, check its children
			{
				PrepareCollisionQuery(cell_->children[i], ray_);
			}
		}
	}
}