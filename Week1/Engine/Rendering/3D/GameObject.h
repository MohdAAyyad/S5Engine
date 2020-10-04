#ifndef GAMEOBJECT_H
#define GAMEOBJECT_h

#include "Model.h"

class GameObject
{
public:
	GameObject(Model* model_, glm::vec3 position_ = glm::vec3());
	~GameObject();
	void Render(Camera* camera_);
	void RenderInstance(Camera* camera_);
	void Update(float delatTime_);

	glm::vec3 GetPosition() const;
	float GetAngle() const;
	glm::vec3 GetRotation() const;
	glm::vec3 GetScale() const;

	void SetPosition(glm::vec3 position_);
	void SetAngle(float angle_);
	void SetRotation(glm::vec3 rotation_);
	void SetScale(glm::vec3 scale_);

	std::string GetTag() const;
	void SetTag(std::string tag_);
	BoundingBox GetBoundingBox();

	bool GetHit();
	void SetHit(bool hit_, int buttonType_);
	glm::mat4 GetModelInstanceTransform();
	GLuint GetShaderProgram();

	void SetQuatRotation(glm::quat quat_);
	glm::quat GetCurrentRotQuat();

	//Components
	template<typename T>
	void AddComponent(T comp_)
	{
		if (dynamic_cast<Component*>(comp_)) //Check if we're pushing a component
		{
			if (!GetComponent<T>()) //Check if we don't already have that component
			{
				components.push_back(comp_);
				comp_->OnCreate(this);
				std::cout << "Added component succesffully" << std::endl;
			}
			else
			{
				Debugger::Warning("Attempted to add a component that we have already added before", "GameObject.cpp", __LINE__);

				delete comp_;
				comp_ = nullptr;
			}
		}
		else
		{
			Debugger::Warning("Attempted to add a component and failed", "GameObject.cpp", __LINE__);
			delete comp_;
			comp_ = nullptr;
		}
	}

	template<typename T>
	T GetComponent()
	{
		T compToGet = nullptr;
		for (auto g : components)
		{
			compToGet = dynamic_cast<T>(g);
			if (compToGet)
			{
				std::cout << "Got component succesffully" << std::endl;
				return compToGet;
			}
		}
		std::cout << "Could not get component" << std::endl;
		return nullptr;
	}

	template<typename T>
	bool RemoveComponent()
	{
		int compIndex = -1;
		for (int i = 0; i < components.size(); i++)
		{
			if (dynamic_cast<T>(components[i]))
			{
				//We've found the component
				delete components[i];
				components[i] = nullptr;
				compIndex = i;
				break;
			}
		}

		if (compIndex != -1)
		{
			components.erase(components.begin() + compIndex);
			std::cout << "Removed component succesfully" << std::endl;
			return true;
		}

		std::cout << "Could not remove component" << std::endl;
		return false;
	}

private:
	std::string tag; //Used to distinguish gameobjects from each other
	Model* model;
	glm::vec3 position;
	float angle;
	glm::vec3 rotation;
	glm::vec3 scale;
	BoundingBox box;
	glm::quat rotQuat;

	bool hit;

	int modelInstance; //Instead of loading multiple model objects for the multiple gameobjects that use the same model, we're gonna load the model once and have all those gameobjects use the same model, and to differentiate between them, we use an instance tracker

	std::vector<class Component*> components;
};

#endif // !GAMEOBJECT_H

