#include "LightSource.h"

LightSource::LightSource(glm::vec3 pos_, glm::vec3 color_, GLfloat ambient_, GLfloat diffuse_):pos(glm::vec3()),color(glm::vec3()),ambient(0),diffuse(0)
{
	pos = pos_;
	color = color_;
	ambient = ambient_;
	diffuse = diffuse_;
}

LightSource::~LightSource()
{

}

glm::vec3 LightSource::GetPos()
{
	return pos;
}
glm::vec3 LightSource::GetColor()
{
	return color;
}
float LightSource::GetAmbient() const
{
	return ambient;
}
float LightSource::GetDiffuse() const
{
	return diffuse;
}

void LightSource::SetPos(glm::vec3 pos_)
{
	pos = pos_;
}
void LightSource::SetColor(glm::vec3 color_)
{
	color = color_;
}
void LightSource::SetAmbient(float ambient_)
{
	ambient = ambient_;
}
void LightSource::SetDiffuse(float diffuse_)
{
	diffuse = diffuse_;
}
