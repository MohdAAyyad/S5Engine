#ifndef PARTICLEBASE_H
#define PARTICLEBASE_H

#include <glm/gtc/type_ptr.hpp>
#include <glm/glm.hpp>

class ParticleBase
{
public:
	virtual void Update(const float deltaTime_) = 0;
	virtual void Render(class Camera* cam_) = 0;

protected:
	glm::vec3 color;
	glm::vec3 pos;
	glm::vec3 vel;
	float lifeTime;
	float size;
	class ParticleEmitter* parentEmitter;
	bool rendering;
};
#endif // !PARTICLEBASE_H

