#pragma once

#include <glm/glm.hpp>

namespace LindaEngine
{
	struct Plane
	{
		glm::vec3 normal;
		float distance;//平面到原点的最近距离，该点与原点得到向量与法线相机平行

		float GetDistance(const glm::vec3& point) const {
			//点乘得到点到法线上的投影长度
			return glm::dot(normal, point) + distance;
		}
	};
}
