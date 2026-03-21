#pragma once

#include "AutoPtr.h"
#include "SphericalHarmonics.h"

namespace LindaEngine
{
	struct Texture;
	class Material;

	class Environment
	{
	public:
		static void SetSkybox(Ref<Material> material);
		static void RenderSkyboxMap();
		static void ComputeBRDFLutMap();

	private:
		static SHCoefficients ComputeSHFromCubemap(Ref<Texture> cubemap);

	private:
		static Ref<Material> _material;
		static Ref<Texture> _cubemap;
		static Ref<Texture> _irradianceMap;
		static Ref<Texture> _prefilterMap;
		static Ref<Texture> _brdfLUTMap;
		static SHCoefficients _sh;
	};
}
