#ifndef SPRITESURFACE_H
#define SPRITESURFACE_H

#include <glew.h>
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

class SpriteSurface
{
public:

	SpriteSurface(std::string imageName_, glm::vec2 imageScale = glm::vec2(1.0f,1.0f), float angle=0.0f, glm::vec4 tintColor_ = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
	~SpriteSurface();

	void Draw(class Camera* camera_, glm::vec2 pos_);

	int GetWidth();
	int GetHeight();
	BoundingBox GetBoundingBox();
private:
	GLuint VAO, VBO;
	GLuint modelLoc;
	GLuint projLoc;
	GLuint tintLoc;
	GLuint inputTextureLoc;
	GLuint textureID;

	void GenerateBuffers();
	float width;
	float height;
	glm::vec2 imageScale;
	float angle;
	glm::vec4 tintColor;
	std::string imageName;

	glm::vec2 FindMinVert();
	glm::vec2 FindMaxVert();
	void GenerateBoundingBox();

	std::vector<Vertex2D> vertexes;
	
	BoundingBox box;
};
#endif // !SPRITESURFACE_H
