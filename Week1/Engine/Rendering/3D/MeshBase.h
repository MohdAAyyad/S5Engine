#ifndef MESHBASE_H
#define MESHBASE_H

#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp> //Helps in returning the address of any data type 
#include "../../Camera/Camera.h"
#include "../../Graphics/MaterialHandler.h"

struct Vertex
{
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 textCoords; //Texture coordinates are always 2D cause you're essentially just looking at a picture
	glm::vec3 color; //Just a typing tip: Functions do NOT have a u while variables CAN have a u (color vs colour)

};

struct SubMesh
{
	std::vector<Vertex> vertexList;
	std::vector<int> meshIndices;
	Material material;
};


class MeshBase
{
public:
	virtual void Render(Camera* camera_, std::vector<glm::mat4> instances_) = 0;
	virtual void Render(Camera* camera_, glm::mat4 instance_) = 0;
protected:
	SubMesh subMesh;
};
#endif