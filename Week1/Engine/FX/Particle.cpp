#include "Particle.h"
#include "../Camera/Camera.h"
#include "ParticleEmitter.h"
#include <iostream>
Particle::Particle(ParticleEmitter* pm_,GLuint shaderProgram_, GLuint textureID_,glm::vec3 pos_, glm::vec3 vel_, glm::vec3 color_,
	 float lifeTime_ ,float size_): modelLoc(0),posLoc(0),projLoc(0), sizeLoc(0),viewLoc(0),shaderProgram(0), inputTextureLoc(0)
{
	pos = pos_;
	vel = vel_;
	lifeTime = lifeTime_;
	size = size_;
	textureID = textureID_;
	shaderProgram = shaderProgram_;
	color = color_;
	parentEmitter = pm_;
	rendering = true;
	GenerateBuffers();
}
Particle::~Particle()
{

}

void Particle::Update(const float deltaTime_)
{
	pos += vel * deltaTime_;
	lifeTime -= deltaTime_;
	if (lifeTime <= 0)
	{
		rendering = false;
		if (parentEmitter)
		{
			//parentEmitter->RemoveParticle(this);
		}
	}
}

void Particle::Render(class Camera* camera_)
{
	if (rendering)
	{
		glUseProgram(shaderProgram);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE);

		glEnable(GL_PROGRAM_POINT_SIZE);

		glBindVertexArray(VAO);

		glUniform1i(inputTextureLoc, 0); //This is unnecessary unless you have more than one texture //0 iss the index //If you have multiple textures, you need multiple uniforms and to increment the index by 1 for each one
		glActiveTexture(GL_TEXTURE0); //Activate GL_Texture0. There are 32 possible textures

		glBindTexture(GL_TEXTURE_2D, textureID);

		glm::mat4 modelMat;

		modelMat = glm::translate(modelMat, pos); //Move the model matrix by the amount of position (matrix * vec3 and returns matrix)
		modelMat = glm::rotate(modelMat, 0.0f, glm::vec3(0.0f, 0.0f, 1.0f));
		modelMat = glm::scale(modelMat, glm::vec3(size, size, 1.0f));

		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(camera_->GetPerspective()));
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(camera_->GetView()));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelMat));
		glUniform3fv(posLoc, 1, glm::value_ptr(pos));
		glUniform4fv(colorLoc, 1, glm::value_ptr(glm::vec4(color, 1.0f)));
		glUniform1f(sizeLoc, size);


		glDrawArrays(GL_POINTS, 0, 1);

		glBindTexture(GL_TEXTURE_2D, 0);
		glBindVertexArray(0);

		glDisable(GL_PROGRAM_POINT_SIZE);
		glDisable(GL_BLEND);
	}
}

void Particle::GenerateBuffers()
{
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO); //Bind the Vertex Array

	posLoc = glGetUniformLocation(shaderProgram, "position");
	modelLoc = glGetUniformLocation(shaderProgram, "model");
	projLoc = glGetUniformLocation(shaderProgram, "proj");
	viewLoc = glGetUniformLocation(shaderProgram, "view");
	sizeLoc = glGetUniformLocation(shaderProgram, "size");
	colorLoc = glGetUniformLocation(shaderProgram, "color");
	inputTextureLoc = glGetUniformLocation(shaderProgram, "inputTexture");

	glBindVertexArray(0); //Unbind the Vertex Array
}