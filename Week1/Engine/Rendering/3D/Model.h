#ifndef MODEL_H
#define MODEL_H

#include "Mesh.h"
#include <glm/gtc/matrix_transform.hpp>
#include <string>
#include "LoadOBJModel.h"

class Model
{
public:
	Model(const std::string& objFilePath_, const std::string& mtlFilePath_, GLuint shaderProgram_);
	~Model();
	void Render(Camera* camera_);
	void RenderInstance(Camera* camera_, int index_);
	void AddMesh(Mesh* mesh); //This helps us independantly load the meshes that compose a model. That's why we pass in vertex lists to Mesh, but a mesh pointer to Model. This way the model is created of a series of meshes rather than getting to know all the vertices



	int CreateInstance(glm::vec3 pos_, float angle_, glm::vec3 rotation_, glm::vec3 scale_); // Create an instance of this model and return its transform matrix
	void UpdateInstance(int index_, glm::vec3 pos_, float angle_, glm::vec3 rotation_, glm::vec3 scale_); //Update the transform matrix of the model instance
	void UpdateInstance(int index_, glm::vec3 pos_, glm::mat4 quatMat_, glm::vec3 rotation_, glm::vec3 scale_);
	glm::mat4 GetTransform(int index_) const; //Get transform of the model of index index_
	BoundingBox GetBoundingBox();

	GLuint GetShaderProgram() const;
	int GetModelInstancesSize();
private:
	std::vector<Mesh*> subMeshes; //The meshes that compose the model
	std::vector<glm::mat4> modelInstance; //Holds the transform matrices of all the instantiated models
	glm::mat4 GetTransform(glm::vec3 pos_, float angle_, glm::vec3 rotation_, glm::vec3 scale_) const;
	glm::mat4 GetTransform(glm::mat4 modelMatrix_, glm::mat4 quatMat) const;
	glm::mat4 GetTransform(glm::vec3 pos_, glm::mat4 quatMat_, glm::vec3 rotation_, glm::vec3 scale_) const;
	void LoadModel();
	GLuint shaderProgram;

	LoadOBJModel* objLoader;

	BoundingBox box;

};

#endif // !MODEL_H

