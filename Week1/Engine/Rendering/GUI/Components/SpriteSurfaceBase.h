#ifndef SPRITESURFACEBASE_H
#define SPRITESURFACEBASE_H

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include <string>
#include "../../../Math/BoundingBox.h"

struct Vertex2D
{
	Vertex2D(glm::vec2 pos_, glm::vec2 textCoords_)
	{
		position = pos_;
		textCoords = textCoords_;
	}
	glm::vec2 position;
	glm::vec2 textCoords; //Texture coordinates are always 2D cause you're essentially just looking at a picture
};

class SpriteSurfaceBase
{
public:
	virtual float GetWidth() = 0;
	virtual float GetHeight() = 0;
	virtual BoundingBox GetBoundingBox() = 0;
	virtual void Draw(class Camera* camera_, glm::vec2 pos_) = 0;

protected:
	float width;
	float height;

	glm::vec2 imageScale;
	float angle;
	glm::vec4 tintColor;

	std::string imageName;

	std::vector<Vertex2D> vertexes;
	BoundingBox box;


	virtual glm::vec2 FindMinVert() = 0;
	virtual glm::vec2 FindMaxVert() = 0;
	virtual void GenerateBoundingBox() = 0;
};
#endif
