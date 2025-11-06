#include "CullTool.h"
#include "Frustum.h"
#include "BoundingBox.h"

using namespace LindaEngine;

bool CullTool::DistanceCull(glm::vec3& origin, BoundingBox& bounding, float dis, bool isAABB)
{
	if (isAABB)
	{
		AABBBoundingBox& aabb = dynamic_cast<AABBBoundingBox&>(bounding);

		if (glm::length(origin - aabb.min) < dis)
			return true;

		if (glm::length(origin - aabb.max) < dis)
			return true;

		if (glm::length(origin - glm::vec3(aabb.min.x, aabb.min.y, aabb.max.z)) < dis)
			return true;

		if (glm::length(origin - glm::vec3(aabb.min.x, aabb.max.y, aabb.min.z)) < dis)
			return true;

		if (glm::length(origin - glm::vec3(aabb.min.x, aabb.max.y, aabb.max.z)) < dis)
			return true;

		if (glm::length(origin - glm::vec3(aabb.max.x, aabb.min.y, aabb.min.z)) < dis)
			return true;

		if (glm::length(origin - glm::vec3(aabb.max.x, aabb.min.y, aabb.max.z)) < dis)
			return true;

		if (glm::length(origin - glm::vec3(aabb.max.x, aabb.max.y, aabb.min.z)) < dis)
			return true;

		return false;
	}
	else
	{
		SphereBoundingBox& sbb = dynamic_cast<SphereBoundingBox&>(bounding);
		return glm::length(origin - sbb.center) + sbb.radius < dis;
	}
}

bool CullTool::FrustumCull(Frustum& frustum, BoundingBox& bounding, bool isAABB)
{
	if (isAABB)
	{
		return frustum.AABBInside(dynamic_cast<AABBBoundingBox&>(bounding));
	}
	else
	{
		return frustum.SphereInside(dynamic_cast<SphereBoundingBox&>(bounding));
	}
}

bool CullTool::OcclusionCull()
{
	//TODO ´ýÊµÏÖ
	return true;
}
