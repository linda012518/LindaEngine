#pragma once

#include "Plane.h"

namespace LindaEngine
{
	class Camera;
	struct AABBBoundingBox;
	struct SphereBoundingBox;

	struct Frustum
	{
		void UpdateFrustum(Camera* cam);
		bool AABBInside(AABBBoundingBox& aabb);
		bool SphereInside(SphereBoundingBox& sphere);

	private:
		bool IsInside(const glm::vec3& point);

	private:
		Plane near;
		Plane far;
		Plane top;
		Plane bottom;
		Plane left;
		Plane right;
	};
}

