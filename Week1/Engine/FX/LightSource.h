#ifndef LIGHTSOURCE_H
#define LIGHTSOURCE_H

#include <glm/glm.hpp>
#include <glew.h>
class LightSource
{
public:
	LightSource(glm::vec3 pos_, glm::vec3 color_, GLfloat ambient_, GLfloat diffuse_);
	~LightSource();

	glm::vec3 GetPos();
	glm::vec3 GetColor();
	GLfloat GetAmbient() const;
	GLfloat GetDiffuse() const;

	void SetPos(glm::vec3 pos_);
	void SetColor(glm::vec3 color_);
	void SetAmbient(float ambient_);
	void SetDiffuse(float diffuse_);

private:
	glm::vec3 pos;
	glm::vec3 color;
	GLfloat ambient;
	GLfloat diffuse;
};
#endif
