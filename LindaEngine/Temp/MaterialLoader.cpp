#include "MaterialLoader.h"
#include "Material.h"

using namespace LindaEngine;

bool MaterialLoader::Serialize()
{
	return true;
}

Ref<Material> MaterialLoader::Deserialize(const char* path)
{
	return nullptr;
}
