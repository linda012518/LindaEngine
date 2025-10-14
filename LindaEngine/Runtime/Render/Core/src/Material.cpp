#include "Material.h"
#include "ShaderManager.h"
#include "MaterialPass.h"

using namespace LindaEngine;

Ref<Material> Material::overrideMat = nullptr;

Material::Material()
{
}

Material::~Material()
{
}

void Material::CompileShader()
{
	std::vector<ShaderSource>& ssVector = ShaderManager::GetShaderSource(_shaderPath.c_str());
	for (auto& ss : ssVector) {
		if (ss.name == "ShadowCaster")
		{

		}
		else if (ss.name == "Depth")
		{

		}
		else if (ss.name == "DepthNormal")
		{

		}
		else
		{
			for (auto& pass : _colorPasses) {
				if (ss.name != pass->_lightMode)
					continue;
			}
		}
	}
}
