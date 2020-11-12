#ifndef MESH_H
#define MESH_H

#include <glew.h>
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

class Mesh
{
public:
	Mesh(SubMesh submesh_, GLuint shaderProgram_);
	~Mesh();
	void Render(Camera* camera_, std::vector<glm::mat4> instances_);
	void Render(Camera* camera_, glm::mat4 instance_);
	SubMesh GetSubMesh();
private:
	void GenerateBuffers();
	SubMesh subMesh;
	GLuint VAO, VBO; //Check slides //VBO holds the data. //VAO tells opengl how the data is separated
	GLuint shaderProgram;
	GLuint modelLoc, viewLoc, projLoc;	
	GLuint textureLoc;
	GLuint cameraPosLoc;
	GLuint lightPosLoc;
	GLuint lightAmbientLoc;
	GLuint lightDiffuseLoc;
	GLuint lightColorLoc;
	GLuint matAmbientLoc;
	GLuint matDiffuseLoc;
	GLuint matSpecularLoc;
	GLuint matShininessLoc;
	GLuint matTransparencyLoc;
};

#endif // !MESH_H

