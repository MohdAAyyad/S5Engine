#ifndef MATERIALLOADER_H
#define MATERIALLOADER_H

#include "../Graphics/MaterialHandler.h"
#include "../Graphics/TextureHandler.h"
#include <sstream>

class MaterialLoader
{
public:
	~MaterialLoader();

	MaterialLoader() = delete;
	MaterialLoader(const MaterialLoader&) = delete; //Disable the copy constructor
	MaterialLoader(MaterialLoader&&) = delete; //Disable the move constructor
	MaterialLoader& operator=(const MaterialLoader&) = delete; //Disable operator = for copy and move
	MaterialLoader& operator=(MaterialLoader&&) = delete;

	static void LoadMaterial(std::string fliePath_);
private:
	static GLuint LoadTexture(std::string fileName_);
};

#endif MATERIALLOADER_H

