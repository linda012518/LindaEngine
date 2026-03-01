#pragma once

#include "AutoPtr.h"
#include "glm/glm.hpp"

namespace LindaEngine
{
	class UniformBuffer;

	struct UniformDataGlobal
	{
		struct Data
		{
			glm::mat4 view;
			glm::mat4 project;
			glm::mat4 viewProject;

			glm::mat4 viewInverse;
			glm::mat4 projectInverse;
			glm::mat4 viewProjectInverse;

			glm::vec4 cameraPosition;

			glm::ivec4 lightCount;//x=dirLightCount y=pointLightCount z=spotLightCount

			glm::vec4 dirLightColors[8];
			glm::vec4 dirLightDirections[8];

			glm::vec4 pointLightColors[64];
			glm::vec4 pointLightPositionAttens[64];

			glm::vec4 spotLightColors[64];
			glm::vec4 spotLightPositions[64];
			glm::vec4 spotLightDirections[64];
			glm::vec4 spotLightAngles[64];

			glm::vec4 time;

			glm::vec4 linda_SHAr;
			glm::vec4 linda_SHAg;
			glm::vec4 linda_SHAb;
			glm::vec4 linda_SHBr;
			glm::vec4 linda_SHBg;
			glm::vec4 linda_SHBb;
			glm::vec4 linda_SHC;
		};

		Data data;

		UniformDataGlobal();
		void SetUniformBufferData();

	private:
		int size;
		Ref<UniformBuffer> buffer;

	};

}
