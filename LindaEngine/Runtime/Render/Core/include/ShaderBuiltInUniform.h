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

		static std::string linda_BonesMatrices;
		static std::string linda_EntityID;

		static std::string linda_SpecCube;
		static std::string linda_PanoramicCube;
	};

}
