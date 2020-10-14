#include "GUIImageComponent.h"
#include "../../../Camera/Camera.h"
#include "../GUIObject.h"
#include "SpriteSurface.h"
#include "../../../Math/CollisionHandler.h"
#include "../../../Event/MouseEventListener.h"



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
		//FindContainingPoint();
	}
}
bool GUIImageComponent::FindContainingPoint()
{
	if(spriteSurface && owner)
	{
		//TODO
		// basically it's like AABB collision
		// generating a hit box turned out to be unnecessary 
		glm::vec2 mouse = MouseEventListener::GetMouseCurrentPos();
		// the position is set at the top right, so move the position over to the middle
		glm::vec2 box = glm::vec2(owner->GetPos().x - spriteSurface->GetWidth() / 2, owner->GetPos().y - spriteSurface->GetHeight() / 2);


		if (mouse.x >= box.x && mouse.x <= box.x + spriteSurface->GetWidth()
			&& mouse.y >= box.y && mouse.y <= box.y + spriteSurface->GetHeight())
		{
			printf("Mouse is over UI element! \n");
			return true;
		}
	}
	return false;
}

void GUIImageComponent::SetOwner(class GUIObject* owner_)
{
	owner = owner_;
}