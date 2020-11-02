#ifndef PARTICLE_H
#define PARTICLE_H

#include <glew.h>
#include <glm/gtc/type_ptr.hpp>
#include <glm/glm.hpp>

class Particle
{
public:
	Particle(class ParticleEmitter* pm_, GLuint shaderProgram_, GLuint textureID_ = -1, 
			 glm::vec3 pos_ = glm::vec3(), glm::vec3 vel_ = glm::vec3(), 
		    glm::vec3 color_ = glm::vec3(),float lifeTime_ = 2.0f, float size_ = 1.0f);
	~Particle();

	void Update(const float deltaTime_);
	void Render(class Camera* cam_);

private:

	glm::vec3 color;
	glm::vec3 pos;
	glm::vec3 vel;
	float lifeTime;
	float size;
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

	ParticleEmitter* parentEmitter;
	void GenerateBuffers();

	bool rendering;
};

#endif // !PARTICLE_H
