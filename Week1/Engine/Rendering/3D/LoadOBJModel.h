#ifndef LOADOBJMODEL_H
#define LOADOBJMODEL_H

#include "Mesh.h"
#include "../../FX/MaterialLoader.h"
#include "../../Math/BoundingBox.h"
#include <sstream>

class LoadOBJModel
{
public:
	LoadOBJModel();
	~LoadOBJModel();

	void LoadModel(const std::string& objFilePath_, const std::string& mtlFilePath_);
	void LoadModel(const std::string& filePath_);
	std::vector<Vertex> GetVerts();
	std::vector<int> GetIndices();
	std::vector<SubMesh> GetSubmeshes();
	BoundingBox GetBoundingBox();
	void OnDestroy();

private:
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec2> textureCoords;
	std::vector<int> indices;
	std::vector<int> normalIndices;
	std::vector<int> texIndices;
	std::vector<Vertex> meshVertices;
	std::vector<SubMesh>subMeshes;

	void PostProcessing();
	void LoadMaterial(const std::string& matName_);
	void LoadMaterialLibrary(const std::string& matFilePath_);
	
	
	Material currentMaterial;

	BoundingBox box;
	glm::vec3 FindMinVert();
	glm::vec3 FindMaxVert();

};

#endif // !LOADOBJMODEL_H
