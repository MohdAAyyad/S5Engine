#include "../Math/CollisionHandler.h"
#include "SceneGraph.h"
#include "FrustumCullingHandle.h"

std::unique_ptr<SceneGraph> SceneGraph::sceneGraphInstance = nullptr;
std::map<GLuint, std::vector<Model*>> SceneGraph::sceneModels = std::map<GLuint, std::vector<Model*>>();
std::map<std::string, GameObject*> SceneGraph::sceneGameObjects = std::map<std::string, GameObject*>();

SceneGraph::SceneGraph()
{

}
SceneGraph::~SceneGraph()
{
	OnDestroy();
}

void SceneGraph::OnDestroy()
{
	if (sceneGameObjects.size() > 0)
	{
		for (auto e : sceneGameObjects)
		{
			delete e.second;
			e.second = nullptr;
		}
		
		sceneGameObjects.clear();
	}

	if (sceneModels.size() > 0)
	{
		for (auto m : sceneModels)
		{
			if (m.second.size() > 0)
			{
				for (auto n : m.second)
				{
					delete n;
					n = nullptr;
				}
				m.second.clear();
			}
		}
		sceneModels.clear();
	}
}
void SceneGraph::AddModel(Model* model_)
{
	if (sceneModels.find(model_->GetShaderProgram()) == sceneModels.end()) //Have we already created an element for that particular shaderprogram?
	{
		//If it doesn't exist, create it
		std::vector<Model*> tmp = std::vector<Model*>();
		tmp.reserve(10);
		tmp.push_back(model_);
		sceneModels.insert(std::pair<GLuint, std::vector<Model*>>(model_->GetShaderProgram(), tmp)); //The std::pair tells the insert what kind of data is being inserted into the map
	}
	else
	{
		sceneModels.at(model_->GetShaderProgram()).push_back(model_); //sceneModels.at(model_->GetShaderProgram()) gets you access to the vector associated with the shaderprogram key
	}
}
void SceneGraph::AddGameObject(GameObject* gameObject_, std::string tag_)
{
	//We need to check if the tag already exists, otherwise adding a gameobject replaces the one that already uses that tag
	
	if (tag_ == "") //If a tag is not passed, give it a default value
	{
		std::string newTag = "Gameobject " + std::to_string(sceneGameObjects.size() + 1);
		gameObject_->SetTag(newTag);
		sceneGameObjects[newTag] = gameObject_;
	}
	else if (sceneGameObjects.find(tag_) == sceneGameObjects.end()) //Tag doesn't exist
	{
		gameObject_->SetTag(tag_);
		sceneGameObjects[tag_] = gameObject_;
	}
	else
	{
		Debugger::Error("Trying to add new object with a tag (" + tag_ + ") that already exists ", "SceneGraph.cpp", __LINE__);

		std::string newTag = "Gameobject " + std::to_string(sceneGameObjects.size() + 1);
		gameObject_->SetTag(newTag);
		sceneGameObjects[newTag] = gameObject_;
	}

	CollisionHandler::GetInstance()->AddObject(gameObject_);
}
GameObject* SceneGraph::GetGameObject(std::string tag_)
{
	if (sceneGameObjects.find(tag_) != sceneGameObjects.end()) //Check to see if we can find the tag
	{
		return sceneGameObjects[tag_];
	}
	else
	{
		return nullptr;
	}
}

void SceneGraph::Update(const float deltaTime_)
{
	for (auto m : sceneGameObjects)
	{
		m.second->Update(deltaTime_);
	}
}
void SceneGraph::Render(Camera* camera_)
{
	std::map<std::string, GameObject*>::iterator it = sceneGameObjects.begin();
	//int t = 0;
	while (it != sceneGameObjects.end())
	{
			if (FrustumCullingHandle::GetInstance()->CheckModelCulling(it->second->GetModelInstanceTransform(), it->second->GetBoundingBox()))
			{
					glUseProgram(it->second->GetShaderProgram());
					it->second->RenderInstance(camera_); //Renders this gameobject's instance only
					//t++;
					//std::cout << "Number of objects rendered " << t << std::endl;
			}
		it++;

	}
	//t = 0;
		/*
		//We're calling render for the models not the gameobjects to prevent models from being rendered twice as we moved the code to gameobject - model instancem module
			std::map<GLuint, std::vector<Model*>>::iterator it = sceneModels.begin();

	while (it != sceneModels.end())
	{
		for (int i = 0; i < it->second.size(); i++)
		{
			for (int j = 0; j < it->second[i]->GetModelInstancesSize(); j++)
			{
				if (FrustumCullingHandle::GetInstance()->CheckModelCulling(it->second[i]->GetTransform(j), it->second[0]->GetBoundingBox())) //We do not need the bounding box for each instance as all we use are the min/max verts which do not change
				{
					glUseProgram(it->first);
					it->second[i]->Render(camera_);
				}
			}
		}
		it++;
	}
		*/

//	for (auto m : sceneModels)
	//{
	//	glUseProgram(m.first); //Make sure you're using the correct shader program
	//	for (auto n : m.second) //Render all the models that use that shader program
	//	{
			//n->Render(camera_);
	//	}
//	}
}

SceneGraph* SceneGraph::GetInstance()
{
	if (!sceneGraphInstance)
	{
		sceneGraphInstance.reset(new SceneGraph());
	}
	return sceneGraphInstance.get();
}