#pragma once

#include "AutoPtr.h"

#include "glm/glm.hpp"
#include "glm/gtx/matrix_decompose.hpp"

#include <string>
#include <vector>

namespace LindaEngine
{
	class Mesh;

	struct FBXResources
	{
		glm::vec3 localPosition;
		glm::quat localRotation;
		glm::vec3 localScale;
		std::string name;
		std::vector<Ref<FBXResources>> children;
		Ref<Mesh> mesh = nullptr;
		std::string hashCode;
	};

}
