#ifndef SCENEGRAPH_H
#define SCNEGRAPH_H

#include <memory>
#include <vector>
#include <map>
#include "3D/GameObject.h"
#include "GUI/GUIObject.h"

class SceneGraph
{

public:
	SceneGraph(const SceneGraph&) = delete; //Disable the copy constructor
	SceneGraph(SceneGraph&&) = delete; //Disable the move constructor
	SceneGraph& operator=(const SceneGraph&) = delete; //Disable operator = for copy and move
	SceneGraph& operator=(SceneGraph&&) = delete;

	static SceneGraph* GetInstance();
	void OnDestroy();
	void AddModel(Model* model_);
	void AddGameObject(GameObject* gameObject_, std::string tag_ = "");
	void AddGUIObject(class GUIObject* guiObject_, std::string tag_ = "");
	GameObject* GetGameObject(std::string tag_);

	void Update(const float deltaTime_);
	void Render(Camera* camera_);
	void Draw(Camera* camera_);
	void LateUpdate(const float deltaTime_);
private:

	SceneGraph();
	~SceneGraph();
	std::vector<GameObject*> lateObjects;
	static std::unique_ptr<SceneGraph> sceneGraphInstance;
	friend std::default_delete<SceneGraph>;

	static std::map<GLuint, std::vector<Model*>> sceneModels; //A better way to go about it is to create a graph rather than use maps
	static std::map<std::string, GameObject*> sceneGameObjects;
	static std::map<std::string, GUIObject*> sceneGuiObjects;

};

#endif // !SCENEGRAPH_H