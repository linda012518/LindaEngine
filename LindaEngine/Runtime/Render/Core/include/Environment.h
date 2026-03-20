#pragma once

#include "AutoPtr.h"
#include "SphericalHarmonics.h"

namespace LindaEngine
{
	struct Texture;

	class Environment
	{
	public:
		static void SetSkybox(Ref<Texture> cubemap);

	private:
		static SHCoefficients ComputeSHFromCubemap(Ref<Texture> cubemap);

	private:
		static Ref<Texture> _cubemap;
		static Ref<Texture> _irradianceMap;
		static Ref<Texture> _prefilterMap;
		static Ref<Texture> _brdfLUTMap;
		static SHCoefficients _sh;
	};
}
