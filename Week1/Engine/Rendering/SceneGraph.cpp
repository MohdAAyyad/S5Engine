#include "../Math/CollisionHandler.h"
#include "SceneGraph.h"
#include "FrustumCullingHandle.h"
#include <glew.h>
#include "../Graphics/ShaderHandler.h"

std::unique_ptr<SceneGraph> SceneGraph::sceneGraphInstance = nullptr;
std::map<GLuint, std::vector<Model*>> SceneGraph::sceneModels = std::map<GLuint, std::vector<Model*>>();
std::map<std::string, GameObject*> SceneGraph::sceneGameObjects = std::map<std::string, GameObject*>();
std::map<std::string, GUIObject*> SceneGraph::sceneGuiObjects = std::map<std::string, GUIObject*>();

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

void SceneGraph::AddGUIObject(class GUIObject* guiObject_, std::string tag_)
{
	if (tag_ == "") //If a tag is not passed, give it a default value
	{
		std::string newTag = "GUIObject " + std::to_string(sceneGameObjects.size() + 1);
		guiObject_->SetTag(newTag);
		sceneGuiObjects[newTag] = guiObject_;
	}
	else if (sceneGameObjects.find(tag_) == sceneGameObjects.end()) //Tag doesn't exist
	{
		guiObject_->SetTag(tag_);
		sceneGuiObjects[tag_] = guiObject_;
	}
	else
	{
		Debugger::Error("Trying to add new object with a tag (" + tag_ + ") that already exists ", "SceneGraph.cpp", __LINE__);

		std::string newTag = "Gameobject " + std::to_string(sceneGameObjects.size() + 1);
		guiObject_->SetTag(newTag);
		sceneGuiObjects[newTag] = guiObject_;
	}

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
}

void SceneGraph::Draw(Camera* camera_)
{
	std::map<std::string, GUIObject*>::iterator it = sceneGuiObjects.begin();

	//Disable the depth test
	glDisable(GL_DEPTH_TEST);
	//Enable blending
	glEnable(GL_BLEND);
	//Make blending function take into account the alpha of the image
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	GLuint shaderProgram = ShaderHandler::GetInstance()->GetShader("guiShader");
	
	while (it != sceneGuiObjects.end())
	{
		glUseProgram(shaderProgram);
		it->second->Draw(camera_); //Draws this gui's instance only
		it++;

	}

	//Reset the depth test and the blending to get ready to render 3D objects again
	glDisable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);

}

SceneGraph* SceneGraph::GetInstance()
{
	if (!sceneGraphInstance)
	{
		sceneGraphInstance.reset(new SceneGraph());
	}
	return sceneGraphInstance.get();
}