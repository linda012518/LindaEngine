#pragma once

#include "ScriptablePass.h"

namespace LindaEngine
{
	class Material;

	class FrustumVisiblePass : public ScriptablePass
	{
	public:
		FrustumVisiblePass();
		void Render(Camera* camera);

	private:
		Ref<Material> _material;
		Ref<Material> _defaultLineMaterial;
	};
}
