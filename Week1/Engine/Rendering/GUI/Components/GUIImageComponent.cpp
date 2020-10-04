#include "GUIImageComponent.h"
#include "../../../Camera/Camera.h"
#include "../GUIObject.h"
#include "SpriteSurface.h"

GUIImageComponent::GUIImageComponent():offset(glm::vec2(0.0f,0.0f))
{
	owner = nullptr;
	spriteSurface = nullptr;
}
GUIImageComponent::~GUIImageComponent()
{
	delete spriteSurface;
	spriteSurface = nullptr;
	owner = nullptr;
}
void GUIImageComponent::OnCreate(std::string imageName_, glm::vec2 offset_, glm::vec2 scale_, float angle_, glm::vec4 tint_)
{
	offset = offset_;
	spriteSurface = new SpriteSurface(imageName_, scale_, angle_, tint_);
}
void GUIImageComponent::Update(float deltaTime_)
{
}
void GUIImageComponent::Draw(Camera* camera_, glm::vec2 pos_)
{
	if (spriteSurface)
	{
		spriteSurface->Draw(camera_, pos_);
	}
}
bool GUIImageComponent::FindContainingPoint()
{
	if (spriteSurface)
	{
		//TODO

		return true;
	}
	return false;
}

void GUIImageComponent::SetOwner(class GUIObject* owner_)
{
	owner = owner_;
}