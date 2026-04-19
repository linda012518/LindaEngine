#pragma once

#include "glm/glm.hpp"

namespace LindaEngine
{
	struct Ray
	{
		glm::vec3 origin;
		glm::vec3 direction;

		glm::vec3 At(float t) const 
		{
			return origin + direction * t;
		}
	};
}
