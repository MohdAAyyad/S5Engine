#ifndef PARTICLE_H
#define PARTICLE_H

#include <glew.h>
#include "ParticleBase.h"

class Particle : public ParticleBase
{
public:
	Particle(ParticleEmitter* pm_, GLuint shaderProgram_, GLuint textureID_ = -1, 
			 glm::vec3 pos_ = glm::vec3(), glm::vec3 vel_ = glm::vec3(), 
		    glm::vec3 color_ = glm::vec3(),float lifeTime_ = 2.0f, float size_ = 1.0f);
	~Particle();

	void Update(const float deltaTime_) override;
	void Render(class Camera* cam_) override;

private:
	GLuint shaderProgram;
	GLuint textureID;
	GLuint VAO;
	GLuint modelLoc;
	GLuint projLoc;
	GLuint viewLoc;
	GLuint posLoc;
	GLuint sizeLoc;
	GLuint inputTextureLoc;
	GLuint colorLoc;
	void GenerateBuffers();

};

#endif // !PARTICLE_H
