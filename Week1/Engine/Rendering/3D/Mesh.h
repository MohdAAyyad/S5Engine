#ifndef MESH_H
#define MESH_H

#include <glew.h>
#include "MeshBase.h"


class Mesh : public MeshBase
{
public:
	Mesh(SubMesh submesh_, GLuint shaderProgram_);
	~Mesh();
	void Render(Camera* camera_, std::vector<glm::mat4> instances_);
	void Render(Camera* camera_, glm::mat4 instance_);
	SubMesh GetSubMesh() override;
private:
	void GenerateBuffers();
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

