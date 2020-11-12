#ifndef PARTICLEEMITTER_H
#define PARTICLEEMITTER_H

#include <string>
#include <vector>
#include <glm/glm.hpp>
#include <glew.h>
#include "../Rendering/3D/Components/Component.h"
#include "../Rendering/Renderer.h"

class ParticleEmitter :public Component
{
public:
	ParticleEmitter(int num_, std::string tex_, std::string shader_);
	~ParticleEmitter();
	
	void OnCreate(GameObject* gameObject_);
	void Update(float deltaTime_) override;
	void RemoveParticle(class ParticleBase* part_);

	void CreateMoreParticlesPos();


private:
	int numOfPartilces;
	std::vector<ParticleBase*> particles;
	GLuint shaderProgram;
	GLuint textureID;
	void GenerateRandomValues(glm::vec3& pos_, glm::vec3& vel_, glm::vec3& color_, float& lifeTime_, float& size_);
	RendererType rendType;
};
#endif // !PARTICLEEMITTER_H
