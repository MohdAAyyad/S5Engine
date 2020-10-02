#include "Model.h"

Model::Model(const std::string& objFilePath_, const std::string& mtlFilePath_, GLuint shaderProgram_):subMeshes(std::vector<Mesh*>()), shaderProgram(0), modelInstance(std::vector<glm::mat4>())
{
	subMeshes.reserve(10);
	modelInstance.reserve(5);
	shaderProgram = shaderProgram_;
	objLoader = new LoadOBJModel();
	objLoader->LoadModel(objFilePath_, mtlFilePath_);
	this->LoadModel();
}
Model::~Model()
{
	if (subMeshes.size() > 0)
	{
		for (auto m : subMeshes) //foreach
		{
			delete m;
			m = nullptr;
		}
		subMeshes.clear();
	}

	if (modelInstance.size() > 0)
	{
		modelInstance.clear();
	}
}
void  Model::Render(Camera* camera_)
{
	for (auto m : subMeshes)
	{
		m->Render(camera_,modelInstance);
	}
}

void Model::RenderInstance(Camera* camera_, int index_)
{
	for (auto m : subMeshes)
	{
		m->Render(camera_, modelInstance[index_]);
	}
}
void Model::AddMesh(Mesh* mesh)
{
	subMeshes.push_back(mesh);
}

int Model::CreateInstance(glm::vec3 pos_, float angle_, glm::vec3 rotation_, glm::vec3 scale_) // Create an instance of this model and return its transform matrix
{
	modelInstance.push_back(GetTransform(pos_, angle_, rotation_, scale_));
	return modelInstance.size() - 1;
}
void Model::UpdateInstance(int index_, glm::vec3 pos_, float angle_, glm::vec3 rotation_, glm::vec3 scale_) //Update the transform matrix of the model instance
{
	if (index_ < modelInstance.size() && index_ >= 0)
	{
		modelInstance[index_] = GetTransform(pos_, angle_, rotation_, scale_);
	}
}

glm::mat4 Model::GetTransform(int index_) const
{
	if(index_ >= 0 && index_ < modelInstance.size())
		return modelInstance[index_];
	return glm::mat4();
}

glm::mat4 Model::GetTransform(glm::vec3 pos_, float angle_, glm::vec3 rotation_, glm::vec3 scale_) const
{
	glm::mat4 model;
	model = glm::translate(model, pos_); //Move the model matrix by the amount of position (matrix * vec3 and returns matrix)
	model = glm::rotate(model, angle_, rotation_);
	model = glm::scale(model, scale_);
	//Or do a multiplication translate * rotate * scale ----- Yes we're scaling, rotating, and then positioning
	return model;
}

void Model::LoadModel()
{
	//Loops through all meshes inside the objloader and create submeshes out of those meshes

	for (int i = 0; i < objLoader->GetSubmeshes().size(); i++)
	{
		subMeshes.push_back(new Mesh(objLoader->GetSubmeshes()[i], shaderProgram));
	}
	box = objLoader->GetBoundingBox(); //Update the bounding box	
	delete objLoader; //Objloader takes a lot of memory. Delete it as it won't be needed from here on out
	objLoader = nullptr;
}

BoundingBox Model ::GetBoundingBox()
{
	return box;
}

GLuint Model::GetShaderProgram() const
{
	return shaderProgram;
}

int Model::GetModelInstancesSize()
{
	return modelInstance.size();
}