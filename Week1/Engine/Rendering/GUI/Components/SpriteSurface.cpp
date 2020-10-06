#include "SpriteSurface.h"
#include "../../../Graphics/TextureHandler.h"
#include "../../../Camera/Camera.h"
#include "../../../Graphics/ShaderHandler.h"

SpriteSurface::SpriteSurface(std::string imageName_, glm::vec2 imageScale_,float angle_, glm::vec4 tintColor_)
			: VAO(0), VBO(0), modelLoc(0), projLoc(0),tintLoc(0), inputTextureLoc(0), 
			  width(0.0f),height(0.0f), angle(0),imageScale(glm::vec2(1.0f,1.0f)),
			  tintColor(glm::vec4(1.0f,1.0f,1.0f,1.0f))
{
	vertexes.push_back(Vertex2D(glm::vec2(-0.5f, 0.5f), glm::vec2(0.0f, 0.0f)));
	vertexes.push_back(Vertex2D(glm::vec2(0.5f, 0.5f), glm::vec2(1.0f, 0.0f)));
	vertexes.push_back(Vertex2D(glm::vec2(-0.5f, -0.5f), glm::vec2(0.0f, 1.0f)));
	vertexes.push_back(Vertex2D(glm::vec2(0.5f, -0.5f), glm::vec2(1.0f, 1.0f)));





	imageName = imageName_;
	imageScale = imageScale_;
	tintColor = tintColor_;
	angle = angle_;
	textureID = 0;

	if (TextureHandler::GetInstance()->GetTexture(imageName_) == 0) //Check if the texture was created first
	{
		TextureHandler::GetInstance()->Create2DTexture(imageName_, "Resources/Textures/" + imageName_ + ".png");
	}
	const Texture* t = TextureHandler::GetInstance()->GetTextureData(imageName_);
	if (t)
	{
		width = t->width;
		height = t->height;
		textureID = t->textureID;
		std::cout << "Tex ID = " << textureID << std::endl;
		std::cout << "Width = " << width << "Heigh = " << height << std::endl;
	}

	GenerateBuffers();
}
SpriteSurface::~SpriteSurface()
{
	//Delete the vao and vbo
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
}

void SpriteSurface::GenerateBuffers()
{
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO); //Bind the Vertex Array
	//std::cout << "GL vertext array bind bind ERROR " << glGetError() << std::endl;
	glGenBuffers(1, &VBO);

	//std::cout << "GL array gen buffer bind ERROR " << glGetError() << std::endl;
	glBindBuffer(GL_ARRAY_BUFFER, VBO); //Bind the data buffer

	//std::cout << "GL array buffer bind ERROR " << glGetError() << std::endl;

	glBufferData(GL_ARRAY_BUFFER, vertexes.size() * sizeof(Vertex2D),
		&vertexes[0], GL_STATIC_DRAW); //We will pass a buffer, the size of the data is the size of the vector multiplied by the size of the Vertex, where in memory this data starts, GL_STATIC_DRAW: data will not change throughout the frame. The opposite of this is GL_DYNAMIC_DRAW i.e. data will change throguh the frame

	//std::cout << "GL ERROR " << glGetError() << std::endl;

	glEnableVertexAttribArray(0); //This is where the first attrbitue is gonna be (this is an index, doesn't necessarily mean position)
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex2D), (GLvoid*)0); //Start at zero, the size of the data, the type of the data point, is the data normalized? false. It's false most of the time, the space between the consecutive vertex attributes, the offset of the first attribute. Position is the first attribute and it's at zero.

	//std::cout << "GL first enable vertex attrib ERROR " << glGetError() << std::endl;

	glEnableVertexAttribArray(1); //This is where the first attrbitue is gonna be (this is an index, doesn't necessarily mean position)
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex2D), (GLvoid*)offsetof(Vertex2D, textCoords)); //Start at zero, the size of the data, the type of the data point, is the data normalized? false. It's false most of the time, the space between the consecutive vertex attributes, the offset of the first attribute. Position is the first attribute and it's at zero.

	//std::cout << "GL second  enable vertex attrib ERROR " << glGetError() << std::endl;

	glBindBuffer(GL_ARRAY_BUFFER, 0); //Unbind the buffer
	glBindVertexArray(0); //Unbind the Vertex Array

	GLuint shaderProgram = ShaderHandler::GetInstance()->GetShader("guiShader");

	//std::cout << "Shader program is " << shaderProgram << std::endl;

	modelLoc = glGetUniformLocation(shaderProgram, "model");
	projLoc = glGetUniformLocation(shaderProgram, "proj");
	tintLoc = glGetUniformLocation(shaderProgram, "tintColour");
	inputTextureLoc = glGetUniformLocation(shaderProgram, "inputTexture");

	//std::cout << "GL uniform ERROR " << glGetError() << std::endl;

	std::cout << "Locs " << modelLoc << "   "<< projLoc << "   "<< tintLoc << "   " << inputTextureLoc<< std::endl;

}

void SpriteSurface::Draw(Camera* camera_, glm::vec2 pos_)
{
	glBindVertexArray(VAO);
	//std::cout << "DUROW is being called inside the sprite shader " << pos_.x << "   "<< pos_.y << "  "<< std::endl;
	glUniform1i(inputTextureLoc, 0); //This is unnecessary unless you have more than one texture //0 iss the index //If you have multiple textures, you need multiple uniforms and to increment the index by 1 for each one
	glActiveTexture(GL_TEXTURE0); //Activate GL_Texture0. There are 32 possible textures
	
	glBindTexture(GL_TEXTURE_2D, textureID);

	glm::mat4 modelMat;

	modelMat = glm::translate(modelMat, glm::vec3(pos_,0.0f)); //Move the model matrix by the amount of position (matrix * vec3 and returns matrix)
	modelMat = glm::rotate(modelMat, angle, glm::vec3(0.0f,1.0f,0.0f));
	modelMat = glm::scale(modelMat, glm::vec3(width * imageScale.x, height* imageScale.y, 1.0f));


	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(camera_->GetOrthographic()));
	glUniform4fv(tintLoc, 1, glm::value_ptr(tintColor));
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelMat)); //I have this location of this uniform (modelloc) and I go its location and memory and make its value equal to transform_

	glDrawArrays(GL_TRIANGLE_STRIP, 0, vertexes.size());
	//The size is not for each element but the whole thing you want to draw
													 //GL_Triangles: Opengl will take every three points and create triangles out of them
													 //This draws triangles not traingle strips. Strips save memory since they combine vertrices, however, they're not supported by most formats. OBJ for exampel does not support it.
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
}

int SpriteSurface::GetWidth()
{
	return width;
}
int SpriteSurface::GetHeight()
{
	return height;
}