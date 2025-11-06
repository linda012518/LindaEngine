#include "Frustum.h"
#include "Camera.h"
#include "Transform.h"
#include "Entity.h"
#include "BoundingBox.h"

using namespace LindaEngine;

void Frustum::UpdateFrustum(Camera* cam)
{
	const glm::mat4& m = cam->GetViewProjectMatrix();

	left.normal = glm::normalize(glm::vec3(m[0][3] + m[0][0], m[1][3] + m[1][0], m[2][3] + m[2][0]));
	left.distance = m[3][3] + m[3][0];

	right.normal = glm::normalize(glm::vec3(m[0][3] - m[0][0], m[1][3] - m[1][0], m[2][3] - m[2][0]));
	right.distance = m[3][3] - m[3][0];

	bottom.normal = glm::normalize(glm::vec3(m[0][3] + m[0][1], m[1][3] + m[1][1], m[2][3] + m[2][1]));
	bottom.distance = m[3][3] + m[3][1];

	top.normal = glm::normalize(glm::vec3(m[0][3] - m[0][1], m[1][3] - m[1][1], m[2][3] - m[2][1]));
	top.distance = m[3][3] - m[3][1];

	near.normal = glm::normalize(glm::vec3(m[0][3] + m[0][2], m[1][3] + m[1][2], m[2][3] + m[2][2]));
	near.distance = m[3][3] + m[3][2];

	far.normal = glm::normalize(glm::vec3(m[0][3] - m[0][2], m[1][3] - m[1][2], m[2][3] - m[2][2]));
	far.distance = m[3][3] - m[3][2];
}

bool Frustum::AABBInside(AABBBoundingBox& aabb)
{
	if (IsInside(aabb.min))
		return true;
	if (IsInside(glm::vec3(aabb.min.x, aabb.min.y, aabb.max.z)))
		return true;
	if (IsInside(glm::vec3(aabb.min.x, aabb.max.y, aabb.min.z)))
		return true;
	if (IsInside(glm::vec3(aabb.min.x, aabb.max.y, aabb.max.z)))
		return true;
	if (IsInside(glm::vec3(aabb.max.x, aabb.min.y, aabb.min.z)))
		return true;
	if (IsInside(glm::vec3(aabb.max.x, aabb.min.y, aabb.max.z)))
		return true;
	if (IsInside(glm::vec3(aabb.max.x, aabb.max.y, aabb.min.z)))
		return true;
	if (IsInside(aabb.max))
		return true;

	return false;
}

bool Frustum::SphereInside(SphereBoundingBox& sphere)
{
	if (left.GetDistance(sphere.center) < -sphere.radius)
		return false;
	if (right.GetDistance(sphere.center) < -sphere.radius)
		return false;
	if (bottom.GetDistance(sphere.center) < -sphere.radius)
		return false;
	if (top.GetDistance(sphere.center) < -sphere.radius)
		return false;
	if (near.GetDistance(sphere.center) < -sphere.radius)
		return false;
	if (far.GetDistance(sphere.center) < -sphere.radius)
		return false;
	return true;
}

bool Frustum::IsInside(const glm::vec3& point)
{
	if (left.GetDistance(point) < 0)
		return false;
	if (right.GetDistance(point) < 0)
		return false;
	if (bottom.GetDistance(point) < 0)
		return false;
	if (top.GetDistance(point) < 0)
		return false;
	if (near.GetDistance(point) < 0)
		return false;
	if (far.GetDistance(point) < 0)
		return false;
	return true;
}

