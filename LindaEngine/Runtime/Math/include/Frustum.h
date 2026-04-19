#pragma once

#include "AutoPtr.h"
#include "Plane.h"

namespace LindaEngine
{
	class Camera;
	struct AABBBoundingBox;
	struct SphereBoundingBox;

	struct Frustum
	{
		void UpdateFrustum(Weak<Camera> cam);
		bool AABBInside(AABBBoundingBox& aabb);
		bool SphereInside(SphereBoundingBox& sphere);
		bool PointInside(const glm::vec3& point);

	private:
		Plane planes[6];
	};
}

