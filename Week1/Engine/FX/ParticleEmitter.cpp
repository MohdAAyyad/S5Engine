#include "ParticleEmitter.h"
#include "Particle.h"
#include "../Graphics/ShaderHandler.h"
#include "../Graphics/TextureHandler.h"
#include "../Core/Debugger.h"
#include "../Core/Randomizer.h"
#include "../Core/CoreEngine.h"
#include <Windows.h>

using namespace MATH;


ParticleEmitter::ParticleEmitter(int num_, std::string tex_, std::string shader_)
{

	rendType = CoreEngine::GetInstance()->GetRendType();

	glm::vec3 pos_, vel_, color_;
	float life, size_;

	if (rendType == RendererType::OPENGL)
	{
		if (TextureHandler::GetInstance()->GetTexture(tex_) == 0) //Check if the texture was created first
		{
			TextureHandler::GetInstance()->Create2DTextureGL(tex_, "Resources/Textures/" + tex_ + ".png");
		}
		textureID = TextureHandler::GetInstance()->GetTexture(tex_);
		shaderProgram = ShaderHandler::GetInstance()->GetShader(shader_);

		if (textureID != 0 && shaderProgram != 0)
		{
			//particles.reserve(num_);
			numOfPartilces = num_;
			for (int i = 0; i < num_; i++)
			{
				GenerateRandomValues(pos_, vel_, color_, life, size_);
				Particle* part = new Particle(this, shaderProgram, textureID, pos_, vel_, color_, life, size_);
				particles.push_back(part);

				//Sleep(100.0f);

			}
		}
		else
		{
			Debugger::Error("Couldn't create particles", "ParticleEmitter.cpp", __LINE__);
		}

	}
}

ParticleEmitter::~ParticleEmitter()
{
	if (particles.size() > 0)
	{
		for (int i = 0; i < particles.size(); i++)
		{
			if (particles[i])
			{
				delete particles[i];
				particles[i] = nullptr;
				break;
			}
		}

		particles.clear();
	}
}


void ParticleEmitter::CreateMoreParticlesPos()
{
	glm::vec3 pos_, vel_, color_;
	float life, size_;

	if (textureID != 0 && shaderProgram != 0)
	{
		GenerateRandomValues(pos_, vel_, color_, life, size_);
		if (rendType == RendererType::OPENGL)
		{
			Particle* part = new Particle(this, shaderProgram, textureID, pos_, vel_, color_, life, size_);
			particles.push_back(part);
		}
	}
	else
	{
		Debugger::Error("Couldn't create particles", "ParticleEmitter.cpp", __LINE__);
	}

}

void ParticleEmitter::OnCreate(GameObject* obj_)
{
	ownerObj = obj_;
}
void ParticleEmitter::Update(float deltaTime_)
{
	class Camera* cam = CoreEngine::GetInstance()->GetCamera();
	glUseProgram(shaderProgram);
	for (auto p : particles)
	{
		p->Update(deltaTime_);
		p->Render(cam);
	}

}

void ParticleEmitter::GenerateRandomValues(glm::vec3& pos_, glm::vec3& vel_, glm::vec3& color_, float& lifeTime_, float& size_)
{
	Randomizer* random = new Randomizer();

	//pos_.x = random->rand(-1.0f, 1.0f);
	//pos_.y = random->rand(-1.0f, 1.0f);
	//pos_.z = 0.0f;
	pos_ = glm::vec3(0.0f, 0.0f, 0.0f);

	vel_.x = random->rand(-0.005f, 0.005f);
	vel_.y = random->rand(-0.005f, 0.005f);
	vel_.z = random->rand(-0.005f, 0.005f);

	color_.x = random->rand(0.0f, 1.0f);
	color_.y = random->rand(0.0f, 1.0f);
	color_.z = random->rand(0.0f, 1.0f);

	lifeTime_ = random->rand(100.0f, 200.0f);
	size_ = random->rand(0.02f, 10.0f);

}

void ParticleEmitter::RemoveParticle(ParticleBase* part_)
{
	int index_ = -1;
	for (int i = 0; i < particles.size(); i++)
	{
		if (particles[i] == part_)
		{
			delete particles[i];
			particles[i] = nullptr;
			index_ = i;
			break;
		}
	}

	if (index_ >= 0)
	{
		particles.erase(particles.begin() + index_);
	}
}