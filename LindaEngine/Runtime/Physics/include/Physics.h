#pragma once

#include "AutoPtr.h"
#include "Ray.h"
#include "BoundingBox.h"
#include "glm/glm.hpp"

#include <vector>

namespace LindaEngine
{
	class Entity;

	struct HitResult
	{
		float t = 0.0f;
		bool hit = false;
		glm::vec3 point = glm::vec3(0.0f);
		Entity* entity = nullptr;
	};

	struct RaycastHit
	{
		std::vector<HitResult> entities; // 击中的所有物体，按照距离从近到远排序
	};

	class Physics
	{
	public:
		static RaycastHit Raycast(Ray ray);

	private:
		// 注意这里必需要AABB
		static HitResult RayIntersectAABB(const Ray& ray, const AABBBoundingBox& aabb);
		static HitResult RayIntersectSphere(const Ray& ray, const SphereBoundingBox& sphere);
	};
}
