#pragma once

#include <glm/glm.hpp>

namespace LindaEngine
{
	struct Frustum;
	struct BoundingBox;

	class CullTool
	{
	public:
		static bool DistanceCull(glm::vec3& origin, BoundingBox& bounding, float dis, bool isAABB = true);
		static bool FrustumCull(Frustum& frustum, BoundingBox& bounding, bool isAABB = true);
		static bool OcclusionCull();
	};
}

