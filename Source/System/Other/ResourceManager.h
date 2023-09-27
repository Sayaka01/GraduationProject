#pragma once
#include <memory>
#include <string>
#include <map>
#include "SkinnedMesh.h"

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

	std::shared_ptr<SkinnedMesh> LoadModelResource(ID3D11Device* device, const char* filename, bool pbr);

private:
	using ModelMap = std::map<std::string, std::weak_ptr<SkinnedMesh>>;
	ModelMap models;
};