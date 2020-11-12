#include "TextureHandler.h"

std::unique_ptr<TextureHandler>TextureHandler::textureInstance = nullptr;

std::vector<Texture*> TextureHandler::textures = std::vector<Texture*>();

TextureHandler::TextureHandler()
{
	textures.reserve(10);
}

TextureHandler::~TextureHandler()
{
	OnDestroy();
}

void TextureHandler::OnDestroy()
{
	if (textures.size() > 0)
	{
		for (auto m : textures)
		{
			glDeleteTextures(sizeof(GLuint), &(m->textureID)); //glDeleteTexture needs the address of the GLuint
			delete m;
			m = nullptr;
		}

		textures.clear();
	}
}


void TextureHandler::Create2DTextureGL(const std::string& textureName_, const std::string textureFilePath_)
{
	Texture* t = new Texture();
	SDL_Surface* surface = nullptr;
	
	surface = IMG_Load(textureFilePath_.c_str());

	if (surface == nullptr)
	{
		Debugger::Error("Texture could not be loaded:" + textureName_, "TextureHandler.cpp", __LINE__);
		delete t;
		t = nullptr;
		return;
	}

	glGenTextures(1, &(t->textureID));
	glBindTexture(GL_TEXTURE_2D, t->textureID);

	int mode = surface->format->BytesPerPixel == 4 ? GL_RGBA : GL_RGB;

	glTexImage2D(GL_TEXTURE_2D, 0, mode, surface->w, surface->h, 0, mode, GL_UNSIGNED_BYTE, surface->pixels);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR); //Texture gets smaller
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); //Texture is magnified (gets bigger)

	glGenerateMipmap(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, 0);

	t->width = surface->w;
	t->height = surface->h;
	t->textureName = textureName_;

	textures.push_back(t);
	SDL_FreeSurface(surface);
	surface = nullptr;
}

TextureHandler* TextureHandler::GetInstance()
{
	if (textureInstance.get() == nullptr)
	{
		textureInstance.reset(new TextureHandler());
	}
		return textureInstance.get();
}


const GLuint TextureHandler::GetTexture(const std::string textureName_)
{
	for (auto m : textures)
	{
		if (m->textureName == textureName_)
		{
			return m->textureID;
		}
	}

	return 0;

}
const Texture* TextureHandler::GetTextureData(const std::string textureName_)
{
	for (auto m : textures)
	{
		if (m->textureName == textureName_)
		{
			return m;
		}
	}

	return nullptr; //return 0 works as well as 0 is intrepeted as nullptr;
}