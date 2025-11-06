#include "Frustum.h"
#include "Camera.h"
#include "Transform.h"
#include "Entity.h"
#include "BoundingBox.h"

using namespace LindaEngine;

void Frustum::UpdateFrustum(Camera* cam)
{
	const glm::mat4& m = cam->GetViewProjectMatrix();

	glm::vec4 plane;
	float length;

	plane = glm::vec4(m[0][3] + m[0][0], m[1][3] + m[1][0], m[2][3] + m[2][0], m[3][3] + m[3][0]);
	length = glm::length(glm::vec3(plane));
	left.normal = glm::vec3(plane) / length;
	left.distance = plane.w / length;

	plane = glm::vec4(m[0][3] - m[0][0], m[1][3] - m[1][0], m[2][3] - m[2][0], m[3][3] - m[3][0]);
	length = glm::length(glm::vec3(plane));
	right.normal = glm::vec3(plane) / length;
	right.distance = plane.w / length;

	plane = glm::vec4(m[0][3] + m[0][1], m[1][3] + m[1][1], m[2][3] + m[2][1], m[3][3] + m[3][1]);
	length = glm::length(glm::vec3(plane));
	bottom.normal = glm::vec3(plane) / length;
	bottom.distance = plane.w / length;

	plane = glm::vec4(m[0][3] - m[0][1], m[1][3] - m[1][1], m[2][3] - m[2][1], m[3][3] - m[3][1]);
	length = glm::length(glm::vec3(plane));
	top.normal = glm::vec3(plane) / length;
	top.distance = plane.w / length;

	plane = glm::vec4(m[0][3] + m[0][2], m[1][3] + m[1][2], m[2][3] + m[2][2], m[3][3] + m[3][2]);
	length = glm::length(glm::vec3(plane));
	near.normal = glm::vec3(plane) / length;
	near.distance = plane.w / length;

	plane = glm::vec4(m[0][3] - m[0][2], m[1][3] - m[1][2], m[2][3] - m[2][2], m[3][3] - m[3][2]);
	length = glm::length(glm::vec3(plane));
	far.normal = glm::vec3(plane) / length;
	far.distance = plane.w / length;
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

bool Frustum::IsInside(Plane& plane, AABBBoundingBox& aabb)
{
	// 找到AABB在平面法向量方向上的最远点（p_vertex）
	glm::vec3 p_vertex = aabb.min;
	if (plane.normal.x >= 0) p_vertex.x = aabb.max.x;
	if (plane.normal.y >= 0) p_vertex.y = aabb.max.y;
	if (plane.normal.z >= 0) p_vertex.z = aabb.max.z;
	return plane.GetDistance(p_vertex) < 0;
}


