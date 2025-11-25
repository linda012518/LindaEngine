#include "Drawable.h"

using namespace LindaEngine;

void Drawable::Draw()
{
	std::vector<Ref<MaterialPass>> go = material->GetPassByLightMode(Material::overrideLightMode);
	if (go.size() <= 0)
		return;

	for (auto& pass : go)
	{
		material->Bind(pass, transform, meshData->attributes);
		meshData->Draw();
	}
}

