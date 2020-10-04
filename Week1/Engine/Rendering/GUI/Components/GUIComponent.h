#ifndef GUICOMPONENT_h
#define GUICOMPONENT_h

#include <string>
#include <glm/glm.hpp>

class GUIComponent
{
public:
	virtual void OnCreate(std::string imageName_,glm::vec2 offset = glm::vec2(0.0f,0.0f),glm::vec2 scale_ = glm::vec2(1.0f,1.0f),float angle_ = 0.0f, glm::vec4 tint_ = glm::vec4(1.0f,1.0f,1.0f,1.0f)) = 0;
	virtual void Update(float deltaTime_) = 0;
	virtual void Draw(class Camera* camera_, glm::vec2 pos_) = 0;
	virtual bool FindContainingPoint() = 0;
	virtual void SetOwner(class GUIObject* owner_) = 0;

protected:
	GUIObject* owner;
};

#endif // !GUICOMPONENT_h