#pragma once
#include <memory>
#include <string>
#include <map>
#include "Component/ModelData.h"

class ResourceManager
{
private:
	ResourceManager(){}
	~ResourceManager(){}
public:
	static ResourceManager& Instance()
	{
		static ResourceManager instance;
		return instance;
	}

	std::shared_ptr<ModelData> LoadModelResource(const char* filename, bool pbr = false);

private:
	using ModelMap = std::map<std::string, std::weak_ptr<ModelData>>;
	ModelMap models;
};