#pragma once

#include "ScriptablePass.h"
#include "glm/glm.hpp"

namespace LindaEngine
{
	class Light;
	class Material;
	enum class LightType;

	class FrustumVisiblePass : public ScriptablePass
	{
	public:
		FrustumVisiblePass();
		void Render(Weak<Camera> camera);

	private:
		void DrawLightWireframe(LightType type, glm::mat4& model);
		void CalculateSpotLightMat(glm::mat4& model, Weak<Light> light, bool isInner);

	private:
		Ref<Material> _cameraMaterial;
		Ref<Material> _lightMaterial;
	};
}
