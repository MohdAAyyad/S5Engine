#pragma once
#include "GUIComponent.h"
class GUIImageComponent :public GUIComponent
{

public:
	GUIImageComponent();
	~GUIImageComponent();
	void OnCreate(std::string imageName_, glm::vec2 offset = glm::vec2(0.0f, 0.0f), glm::vec2 scale_ = glm::vec2(1.0f, 1.0f), float angle_ = 0.0f, glm::vec4 tint_ = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)) override;
	void Update(float deltaTime_) override;
	void Draw(class Camera* camera_, glm::vec2 pos_) override;
	bool FindContainingPoint() override;
	void SetOwner(class GUIObject* owner_) override;

private:
	class SpriteSurface* spriteSurface;
	glm::vec2 offset;
};

