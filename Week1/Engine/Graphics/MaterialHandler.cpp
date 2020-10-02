#include "MaterialHandler.h"

std::unique_ptr<MaterialHandler> MaterialHandler::instance = nullptr;
std::vector<Material>  MaterialHandler::materials = std::vector<Material>();

MaterialHandler::MaterialHandler()
{
	materials.reserve(10);
}

MaterialHandler::~MaterialHandler()
{
	OnDestroy();
}

void MaterialHandler::OnDestroy()
{
	if (materials.size() > 0)
		materials.clear();
}

 MaterialHandler* MaterialHandler::GetInstance()
 {
	 if (!instance)
		 instance.reset(new MaterialHandler());
	 return instance.get();
 }

void MaterialHandler::AddMaterial( Material mat_)
{
	//We did not check if we already have it since if two materials have different functions then they must have different names
	materials.push_back(mat_);
}
Material MaterialHandler::GetMaterial(const std::string name_) const
{
	for (auto m : materials)
	{
		if (m.name == name_)
			return m;
	}
	return Material();
}
