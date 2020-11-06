#ifndef SPRITESURFACE_H
#define SPRITESURFACE_H

#include <glew.h>
#include "SpriteSurfaceBase.h"


class SpriteSurface : public SpriteSurfaceBase
{
public:

	SpriteSurface(std::string imageName_, glm::vec2 imageScale = glm::vec2(1.0f,1.0f), float angle=0.0f, glm::vec4 tintColor_ = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
	~SpriteSurface();

	void Draw(class Camera* camera_, glm::vec2 pos_) override;

	float GetWidth() override;
	float GetHeight() override;
	BoundingBox GetBoundingBox() override;
private:
	GLuint VAO, VBO;
	GLuint modelLoc;
	GLuint projLoc;
	GLuint tintLoc;
	GLuint inputTextureLoc;
	GLuint textureID;

	void GenerateBuffers();

	glm::vec2 FindMinVert() override;
	glm::vec2 FindMaxVert() override;
	void GenerateBoundingBox() override;
};
#endif // !SPRITESURFACE_H
