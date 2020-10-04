#ifndef GUIOBJECT_H
#define GUIOBJECT_H
#include <string>
#include <glm/glm.hpp>
#include <vector>
#include "Components/GUIComponent.h"
class GUIObject
{

private:
	std::string tag;

public:
	GUIObject(glm::vec2 pos_);
	~GUIObject();

	void SetTag(std::string tag_);
	glm::vec2 GetPos();
	void Draw(class Camera* camera_);
	bool FindContainingPoint();

private:

	glm::vec2 pos;
	std::vector<GUIComponent*> components;

public:

	//Components
	template<typename T>
	void AddComponent(T comp_)
	{
		if (dynamic_cast<GUIComponent*>(comp_)) //Check if we're pushing a component
		{
			if (!GetComponent<T>()) //Check if we don't already have that component
			{
				components.push_back(comp_);
				std::cout << "Added GUI component succesffully" << std::endl;
			}
			else
			{
				Debugger::Warning("Attempted to add a GUI component that we have already added before", "GameObject.cpp", __LINE__);

				delete comp_;
				comp_ = nullptr;
			}
		}
		else
		{
			Debugger::Warning("Attempted to add a GUI component and failed", "GameObject.cpp", __LINE__);
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
				std::cout << "Got GUI component succesffully" << std::endl;
				return compToGet;
			}
		}
		std::cout << "Could not get GUI component" << std::endl;
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


};

#endif // !GUIOBJECT_H

