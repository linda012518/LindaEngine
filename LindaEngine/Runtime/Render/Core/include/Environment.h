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
		static void ComputeProceduralMap();

		static Ref<Texture> GetCubemap();
		static Ref<Texture> GetIrradianceMap();
		static Ref<Texture> GetPrefilterMap();
		static Ref<Texture> GetBRDFLUTMap();
		static SHCoefficients& GetSH();

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
