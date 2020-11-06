#ifndef TEXTUREHANDLER_H
#define TEXTUREHANDLER_H

#include <memory>
#include <glew.h>
#include <string>
#include <vector>
#include <SDL.h>
#include <SDL_image.h>
#include "../Core/Debugger.h"

struct Texture
{
	GLuint textureID = 0;
	float width = 0.0f;
	float height = 0.0f;
	std::string textureName = "";

};

class TextureHandler
{

public:
	static TextureHandler* GetInstance();

	TextureHandler(const TextureHandler&) = delete; //Disable the copy constructor
	TextureHandler(TextureHandler&&) = delete; //Disable the move constructor
	TextureHandler& operator=(const TextureHandler&) = delete; //Disable operator = for copy and move
	TextureHandler& operator=(TextureHandler&&) = delete;

	void Create2DTextureGL(const std::string& textureName_, const std::string textureFilePath_);

	static const GLuint GetTexture(const std::string textureName_);
	static const Texture* GetTextureData(const std::string textureName_);


	void OnDestroy();
private:
	TextureHandler();
	~TextureHandler();


	static std::unique_ptr<TextureHandler>textureInstance;
	friend std::default_delete<TextureHandler>;
	
	static std::vector<Texture*> textures;
};

#endif

