#pragma once

#include <string>

namespace LindaEngine
{
	struct ShaderBuiltInUniform
	{
	public:
		static std::string linda_LocalToWorld;
		static std::string linda_WorldToLocal;
		static std::string linda_Matrix_VP_PanoramicCube;
		static std::string linda_FrustumMatrix_I_VP;
		static std::string linda_LightLocalToWorld;
		static std::string linda_SkyboxMatrix_P;

		static std::string linda_BonesMatrices;
		static std::string linda_EntityID;

		static std::string linda_SpecCube;
		static std::string linda_PanoramicCube;
	};

}
