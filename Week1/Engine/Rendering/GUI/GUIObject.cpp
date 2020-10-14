#include "GUIObject.h"
#include <iostream>

GUIObject::GUIObject(glm::vec2 pos_)
{
	tag = ""; 
	pos = pos_;
}
GUIObject::~GUIObject() 
{
	if (components.size() > 0)
	{
		for (int i = 0; i < components.size(); i++)
		{
			delete components[i];
			components[i] = nullptr;
		}

		components.clear();
	}
}

void GUIObject::SetTag(std::string tag_)
{
	tag = tag_;
}

glm::vec2 GUIObject::GetPos()
{
	return pos;
}

void GUIObject::Draw(class Camera* camera_)
{
	for (auto c : components)
	{
		c->Draw(camera_,pos);
		FindContainingPoint();
	}
}
bool GUIObject::FindContainingPoint()
{
	for (auto c : components)
	{
		if (c->FindContainingPoint())
		{
			return true;
		}
	}
	return false;
}