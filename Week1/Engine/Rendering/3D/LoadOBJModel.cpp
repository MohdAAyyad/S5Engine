#include "LoadOBJModel.h"

LoadOBJModel::LoadOBJModel()
{
	currentMaterial = Material();
	vertices.reserve(200);
	normals.reserve(200);
	textureCoords.reserve(200);
	indices.reserve(200);
	normalIndices.reserve(200);
	texIndices.reserve(200);
	meshVertices.reserve(200);
	subMeshes.reserve(10);

}
LoadOBJModel::~LoadOBJModel()
{
	OnDestroy();
}

void LoadOBJModel::OnDestroy()
{
	if(vertices.size()>0)
		vertices.clear();
	if (normals.size() > 0)
		 normals.clear();
	if (textureCoords.size() > 0)
		 textureCoords.clear();
	if (indices.size() > 0)
		 indices.clear();
	if (normalIndices.size() > 0)
		 normalIndices.clear();
	if (texIndices.size() > 0)
		 texIndices.clear();
	if (meshVertices.size() > 0)
		 meshVertices.clear();
	if (subMeshes.size() > 0)
		 subMeshes.clear();
}

std::vector<Vertex> LoadOBJModel::GetVerts()
{
	return meshVertices;
}
std::vector<int> LoadOBJModel::GetIndices()
{
	return indices;
}
std::vector<SubMesh> LoadOBJModel::GetSubmeshes()
{
	return subMeshes;
}

void LoadOBJModel::LoadModel(const std::string& objFilePath_, const std::string& mtlFilePath_)
{
	LoadMaterialLibrary(mtlFilePath_);
	LoadModel(objFilePath_);
}

void LoadOBJModel::LoadModel(const std::string& filePath_)
{
	std::ifstream in(filePath_.c_str(), std::ios::in);

	if (!in)
	{
		Debugger::Error("Could not open obj file path " + filePath_, "LoadOBJModel", __LINE__);
		return;
	}

	std::string line;
	std::string segment;
	std::vector<std::string> tempSegmentedLine;
	std::vector<std::string> segmentedLine;

	while (std::getline(in, line))
	{
		//Vertex data
		if (line.substr(0, 2) == "v ")
		{
			std::stringstream v(line.substr(2));
			float x, y, z;
			v >> x >> y >> z; //Put the first float into x, the second into y, and the third into z
			vertices.push_back(glm::vec3(x, y, z));
		}

		//TODO

		//Normal data

		else if (line.substr(0, 3) == "vn ")
		{
			std::stringstream v(line.substr(3));
			float x, y, z;
			v >> x >> y >> z; //Put the first float into x, the second into y, and the third into z
			normals.push_back(glm::vec3(x, y, z));
		}

		//Texture Data

		else if (line.substr(0, 3) == "vt ")
		{
			std::stringstream v(line.substr(3));
			float x, y;
			v >> x >> y; //Put the first float into x, and  the second into y
			textureCoords.push_back(glm::vec2(x, y));
		}

		//Face Data

		else if (line.substr(0, 2) == "f ")
		{
			std::stringstream v(line.substr(2)); //Take the line after f 

			while (std::getline(v, segment, ' ')) //Remove the spaces. Now you have [1/1/1][/2/2/2][/3/3/3]
			{
				tempSegmentedLine.push_back(segment);
			}
			for (int i = 0; i < tempSegmentedLine.size(); i++) //Take each segment and segement it further by removing the /
			{
				v = std::stringstream(tempSegmentedLine[i].substr(0));

				while (std::getline(v, segment, '/'))
				{
					segmentedLine.push_back(segment); //Each push will give a single number [1][1][1][2][2][2][3][3][3]
				}
			}
			
			//Push the faces
			indices.push_back(std::stoi(segmentedLine[0]));
			indices.push_back(std::stoi(segmentedLine[3]));
			indices.push_back(std::stoi(segmentedLine[6]));

			normalIndices.push_back(std::stoi(segmentedLine[2]));
			normalIndices.push_back(std::stoi(segmentedLine[5]));
			normalIndices.push_back(std::stoi(segmentedLine[8]));

			texIndices.push_back(std::stoi(segmentedLine[1]));
			texIndices.push_back(std::stoi(segmentedLine[4]));
			texIndices.push_back(std::stoi(segmentedLine[7]));

			segmentedLine.clear();
			tempSegmentedLine.clear();
			segment.clear();
		}
		//Check if there's a second mesh
		else if (line.substr(0, 7) == "usemtl ")
		{
			if (indices.size() > 0) //If we've already loaded faces, process them first
			{
				PostProcessing();
			}
			LoadMaterial(line.substr(7));
		}
	}

	PostProcessing(); //Called after the end of the while loop to load any final mesh that was added to the indices

}

void LoadOBJModel::PostProcessing() //Convert face data into actual vertex object which can be passed into OpenGL
{
	for (int i = 0; i < indices.size(); i++)
	{
		Vertex vert;
		vert.position = vertices[indices[i] - 1];
		vert.normal = normals[normalIndices[i] - 1];
		vert.textCoords = textureCoords[texIndices[i] - 1];

		meshVertices.push_back(vert);
	}

	SubMesh submesh;
	submesh.vertexList = meshVertices;
	submesh.meshIndices = indices;
	submesh.material = currentMaterial;
	subMeshes.push_back(submesh);

	//You need to clear the indices as the obj files do not reset if there are multiple objects
	//Look at the apple obj file. The second object starts at 43, so we're resetting here as it doesn't reset itself
	indices.clear();
	normalIndices.clear();
	texIndices.clear();
	meshVertices.clear();
	currentMaterial = Material();
}
BoundingBox LoadOBJModel::GetBoundingBox()
{
	box.minVert = FindMinVert();
	box.maxVert = FindMaxVert();
	return box;
}

glm::vec3 LoadOBJModel::FindMinVert()
{
	glm::vec3 min = subMeshes[0].vertexList[0].position;

	for (int i = 0; i < subMeshes.size(); i++)
	{
		for (int j = 0; j < subMeshes[i].vertexList.size(); j++)
		{
			if (subMeshes[i].vertexList[j].position.x < min.x)
				min.x = subMeshes[i].vertexList[j].position.x;
			if (subMeshes[i].vertexList[j].position.y < min.y)
				min.y = subMeshes[i].vertexList[j].position.y;
			if (subMeshes[i].vertexList[j].position.z < min.z)
				min.z = subMeshes[i].vertexList[j].position.z;
		}
	}

	return min;
}
glm::vec3 LoadOBJModel::FindMaxVert()
{
	glm::vec3 max = subMeshes[0].vertexList[0].position;

	for (int i = 0; i < subMeshes.size(); i++)
	{
		for (int j = 0; j < subMeshes[i].vertexList.size(); j++)
		{
			if (subMeshes[i].vertexList[j].position.x > max.x)
				max.x = subMeshes[i].vertexList[j].position.x;
			if (subMeshes[i].vertexList[j].position.y > max.y)
				max.y = subMeshes[i].vertexList[j].position.y;
			if (subMeshes[i].vertexList[j].position.z > max.z)
				max.z = subMeshes[i].vertexList[j].position.z;
		}
	}
	return max;
}

void LoadOBJModel::LoadMaterial(const std::string& matName_)
{
	currentMaterial = MaterialHandler::GetInstance()->GetMaterial(matName_);
}
void LoadOBJModel::LoadMaterialLibrary(const std::string& matFilePath_)
{
	MaterialLoader::LoadMaterial(matFilePath_);
}