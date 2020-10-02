#include "MaterialLoader.h"

MaterialLoader::~MaterialLoader()
{

}
void MaterialLoader::LoadMaterial(std::string fliePath_)
{
	std::ifstream in(fliePath_.c_str(), std::ios::in); //ios::in means read only
	if (!in)
	{
		Debugger::Error("Could not open material file path " + fliePath_, "MaterialLoader", __LINE__);
		return;
	}
	Material mat = Material();
	std::string matName = "";
	std::string line;
	while (std::getline(in, line)) //Run line by line until the end of the file
	{
		if (line.substr(0, 7) == "newmtl ") //Look at the first 7 characters and see if it equals newmtl (start,length)
		{
			if (mat.diffuseMap != 0) //This means we've already loaded a material
			{
				MaterialHandler::GetInstance()->AddMaterial(mat);
				mat = Material(); //Reset the material so you can load the next one
			}
			matName = line.substr(7);
			mat.diffuseMap = LoadTexture(matName);
			mat.name = matName;
		}
		//Read the relevant remaining data 

		if (line.substr(0, 4) == "	Ns ") //Look for /tKs 
		{
			std::stringstream v(line.substr(4));
			v >> mat.shininess;
		}

		if (line.substr(0, 3) == "	d ") //Look for /tKs 
		{
			std::stringstream v(line.substr(3));
			v >> mat.transparency;
		}

		if (line.substr(0, 4) == "	Ks ") //Look for /tKs 
		{
			std::stringstream v(line.substr(4));
			v >> mat.specular.x >> mat.specular.y >> mat.specular.z;
		}

		if (line.substr(0, 4) == "	Kd ") //Look for \tKd 
		{
			std::stringstream v(line.substr(4));
			v >> mat.diffuse.x >> mat.diffuse.y >> mat.diffuse.z;
		}

		if (line.substr(0, 4) == "	Ka ") //Look for \tKa 
		{
			std::stringstream v(line.substr(4));
			v >> mat.ambient.x >> mat.ambient.y >> mat.ambient.z;
		}
	}

	if (mat.diffuseMap != 0) //Add the last material to the handler
	{
		MaterialHandler::GetInstance()->AddMaterial(mat);
	}

	in.close();
}
GLuint MaterialLoader::LoadTexture(std::string fileName_)
{
	GLuint currentTexture = TextureHandler::GetInstance()->GetTexture(fileName_);

	if (currentTexture == 0) //Texture does not exist
	{
		TextureHandler::GetInstance()->Create2DTexture(fileName_, "./Resources/Textures/" + fileName_ + ".JPG");
		currentTexture = TextureHandler::GetInstance()->GetTexture(fileName_);
	}

	return currentTexture;
}