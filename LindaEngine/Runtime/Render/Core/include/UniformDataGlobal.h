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

			glm::vec4 time;
		};

		Data data;

		UniformDataGlobal();
		void SetUniformBufferData();

	private:
		int size;
		Ref<UniformBuffer> buffer;

	};

}
