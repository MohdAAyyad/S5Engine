#ifndef MATERIALHANDLER_H
#define MATERIALHANDLER_H

#include <memory>
#include <vector>
#include <string>
#include <glew.h>
#include <glm/glm.hpp>

struct Material
{
	inline Material():diffuseMap(0),shininess(0.0f),transparency(0.0f), 
			          ambient(glm::vec3(1.0f)), diffuse(glm::vec3(1.0f)), specular(glm::vec3()), name("")
	{

	}
	GLuint diffuseMap;
	float shininess; //Ns
	float transparency; //d

	glm::vec3 ambient; //Ka
	glm::vec3 diffuse; //Kd 
	glm::vec3 specular; //Ks

	std::string name;

};


class MaterialHandler
{
public:
	MaterialHandler(const MaterialHandler&) = delete; //Disable the copy constructor
	MaterialHandler(MaterialHandler&&) = delete; //Disable the move constructor
	MaterialHandler& operator=(const MaterialHandler&) = delete; //Disable operator = for copy and move
	MaterialHandler& operator=(MaterialHandler&&) = delete;

	static MaterialHandler* GetInstance();

	void AddMaterial(Material mat_);
	Material GetMaterial(const std::string name_) const;
	void OnDestroy();

private:
	MaterialHandler();
	~MaterialHandler();

	static std::unique_ptr<MaterialHandler> instance;
	friend std::default_delete<MaterialHandler>;

	static std::vector<Material> materials;
};

#endif

