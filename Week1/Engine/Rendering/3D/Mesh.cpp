#include "Mesh.h"
#include <iostream>
Mesh::Mesh(SubMesh submesh_,GLuint shaderProgram_) : VAO(0), VBO(0), modelLoc(0), viewLoc(0), projLoc(0), 
													textureLoc(0), cameraPosLoc(0), lightPosLoc(0),lightAmbientLoc(0),
													lightDiffuseLoc(0),lightColorLoc(0), matAmbientLoc(0), matDiffuseLoc(0), 
													matSpecularLoc(0), matShininessLoc(0), matTransparencyLoc(0)
{
subMesh = submesh_; //Assumed that it's the vectors are already reserved
shaderProgram = shaderProgram_;
GenerateBuffers();
}

Mesh::~Mesh()
{
	glDeleteVertexArrays(1, &VAO); //glDeleteVertexArrays takes a reference to the VAO
	glDeleteBuffers(1, &VBO);

	if (subMesh.vertexList.size() > 0)
	{
		subMesh.vertexList.clear();
	}
	if (subMesh.meshIndices.size() > 0)
	{
		subMesh.meshIndices.clear();
	}
}
void Mesh::Render(Camera* camera_, std::vector<glm::mat4> instances_) //Render whole vector of instances
{
	glUniform1i(textureLoc, 0); //This is unnecessary unless you have more than one texture //0 is the index //If you have multiple textures, you need multiple uniforms and to increment the index by 1 for each one
	glActiveTexture(GL_TEXTURE0); //Activate GL_Texture0. There are 32 possible textures
	glBindTexture(GL_TEXTURE_2D, subMesh.material.diffuseMap);

	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(camera_->GetView()));
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(camera_->GetPerspective()));
	glUniform3fv(cameraPosLoc, 1, glm::value_ptr(camera_->GetPosition()));
	glUniform3fv(lightPosLoc, 1, glm::value_ptr(camera_->GetLights()[0]->GetPos()));
	glUniform1f(lightAmbientLoc, camera_->GetLights()[0]->GetAmbient());
	glUniform1f(lightDiffuseLoc, camera_->GetLights()[0]->GetDiffuse());
	glUniform3fv(lightColorLoc, 1, glm::value_ptr(camera_->GetLights()[0]->GetColor()));
	glUniform3fv(matAmbientLoc, 1, glm::value_ptr(subMesh.material.ambient));
	glUniform3fv(matDiffuseLoc, 1, glm::value_ptr(subMesh.material.diffuse));
	glUniform3fv(matSpecularLoc, 1, glm::value_ptr(subMesh.material.specular));
	glUniform1f(matShininessLoc, subMesh.material.shininess);
	glUniform1f(matTransparencyLoc, subMesh.material.transparency);
	   
	glBindVertexArray(VAO);

	for (int i = 0; i < instances_.size(); i++)
	{
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(instances_[i])); //I have this location of this uniform (modelloc) and I go its location and memory and make its value equal to transform_
		glDrawArrays(GL_TRIANGLES, 0, subMesh.vertexList.size());
	}
	 //The size is not for each element but the whole thing you want to draw
												      //GL_Triangles: Opengl will take every three points and create triangles out of them
													  //This draws triangles not traingle strips. Strips save memory since they combine vertrices, however, they're not supported by most formats. OBJ for exampel does not support it.
													  //Check slides

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}


void Mesh::Render(Camera* camera_, glm::mat4 instance_) //Render a single instance 
{
	glUniform1i(textureLoc, 0); //This is unnecessary unless you have more than one texture //0 is the index //If you have multiple textures, you need multiple uniforms and to increment the index by 1 for each one
	glActiveTexture(GL_TEXTURE0); //Activate GL_Texture0. There are 32 possible textures
	glBindTexture(GL_TEXTURE_2D, subMesh.material.diffuseMap);

	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(camera_->GetView()));
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(camera_->GetPerspective()));
	glUniform3fv(cameraPosLoc, 1, glm::value_ptr(camera_->GetPosition()));
	glUniform3fv(lightPosLoc, 1, glm::value_ptr(camera_->GetLights()[0]->GetPos()));
	glUniform1f(lightAmbientLoc, camera_->GetLights()[0]->GetAmbient());
	glUniform1f(lightDiffuseLoc, camera_->GetLights()[0]->GetDiffuse());
	glUniform3fv(lightColorLoc, 1, glm::value_ptr(camera_->GetLights()[0]->GetColor()));
	glUniform3fv(matAmbientLoc, 1, glm::value_ptr(subMesh.material.ambient));
	glUniform3fv(matDiffuseLoc, 1, glm::value_ptr(subMesh.material.diffuse));
	glUniform3fv(matSpecularLoc, 1, glm::value_ptr(subMesh.material.specular));
	glUniform1f(matShininessLoc, subMesh.material.shininess);
	glUniform1f(matTransparencyLoc, subMesh.material.transparency);

	glBindVertexArray(VAO);

	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(instance_)); //I have this location of this uniform (modelloc) and I go its location and memory and make its value equal to transform_
	glDrawArrays(GL_TRIANGLES, 0, subMesh.vertexList.size());
	//The size is not for each element but the whole thing you want to draw
													 //GL_Triangles: Opengl will take every three points and create triangles out of them
													 //This draws triangles not traingle strips. Strips save memory since they combine vertrices, however, they're not supported by most formats. OBJ for exampel does not support it.
													 //Check slides

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Mesh::GenerateBuffers()
{

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO); //Bind the Vertex Array
	glBindBuffer(GL_ARRAY_BUFFER, VBO); //Bind the data buffer

	glBufferData(GL_ARRAY_BUFFER, subMesh.vertexList.size() * sizeof(Vertex),
			     &subMesh.vertexList[0], GL_STATIC_DRAW); //We will pass a buffer, the size of the data is the size of the vector multiplied by the size of the Vertex, where in memory this data starts, GL_STATIC_DRAW: data will not change throughout the frame. The opposite of this is GL_DYNAMIC_DRAW i.e. data will change throguh the frame

	//Position
	glEnableVertexAttribArray(0); //This is where the first attrbitue is gonna be (this is an index, doesn't necessarily mean position)
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)0); //Start at zero, the size of the data, the type of the data point, is the data normalized? false. It's false most of the time, the space between the consecutive vertex attributes, the offset of the first attribute. Position is the first attribute and it's at zero.

	//Normal
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex,normal)); //offsetof(Structure, name of structure attribute). Basically, you're telling it that the offset is the offset of normal in relation to the Vertex struct 

	//Text Coords
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, textCoords));

	//Color
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, color));

	glBindVertexArray(0); //Unbind the Vertex Array
	glBindBuffer(GL_ARRAY_BUFFER, 0); //Unbind the buffer

	modelLoc = glGetUniformLocation(shaderProgram, "model"); //The name is very important. In the shader, there MUST be a uniform with same name. Case sensitive and all
															 // modelLoc acts as an index to uniforms. The uniform value can be of any data type, modelLoc is only an index so opengl knows where to look
															 //opengl has a list of all the uniforms created and needs indexes to go through them

	viewLoc = glGetUniformLocation(shaderProgram, "view");
	projLoc = glGetUniformLocation(shaderProgram, "proj");
	textureLoc = glGetUniformLocation(shaderProgram, "inputTexture");
	cameraPosLoc = glGetUniformLocation(shaderProgram, "cameraPos");
	lightPosLoc = glGetUniformLocation(shaderProgram, "light.pos");
	lightAmbientLoc = glGetUniformLocation(shaderProgram, "light.ambient");
	lightDiffuseLoc = glGetUniformLocation(shaderProgram, "light.diffuse");
	lightColorLoc = glGetUniformLocation(shaderProgram, "light.color");
	matAmbientLoc = glGetUniformLocation(shaderProgram, "mat.ambient");
	matDiffuseLoc = glGetUniformLocation(shaderProgram, "mat.diffuse");
	matSpecularLoc = glGetUniformLocation(shaderProgram, "mat.specular");
	matShininessLoc = glGetUniformLocation(shaderProgram, "mat.shininess");
	matTransparencyLoc = glGetUniformLocation(shaderProgram, "mat.transparency");


	//We're basically telling the GPU the data we have. Opengl at this point doesn't really care what the data represents.
	//We'll give meaningful names and values to the data in shaders.
}