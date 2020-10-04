#ifndef GUIOBJECT_H
#define GUIOBJECT_H

#include <vector>
#include <glm/glm.hpp>
class GUIObject
{
public:
	inline GUIObject()
	{
		owner = nullptr;
	};
	inline ~GUIObject()
	{
		owner = nullptr;
	};
	virtual void Draw() = 0;
	virtual void FindContainingPoint(glm::vec2 mousePos_) = 0;
	
protected:
	class GameObject* owner;
};
#endif

