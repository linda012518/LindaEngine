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

	// 从矩阵中提取 right 平面
	plane = glm::vec4(m[0][3] - m[0][0], m[1][3] - m[1][0], m[2][3] - m[2][0], m[3][3] - m[3][0]);
	length = glm::length(glm::vec3(plane));
	planes[0].normal = glm::vec3(plane) / length;
	planes[0].distance = plane.w / length;

	// 从矩阵中提取 left 平面
	plane = glm::vec4(m[0][3] + m[0][0], m[1][3] + m[1][0], m[2][3] + m[2][0], m[3][3] + m[3][0]);
	length = glm::length(glm::vec3(plane));
	planes[1].normal = glm::vec3(plane) / length;
	planes[1].distance = plane.w / length;

	// 从矩阵中提取 bottom 平面
	plane = glm::vec4(m[0][3] + m[0][1], m[1][3] + m[1][1], m[2][3] + m[2][1], m[3][3] + m[3][1]);
	length = glm::length(glm::vec3(plane));
	planes[2].normal = glm::vec3(plane) / length;
	planes[2].distance = plane.w / length;

	// 从矩阵中提取 top 平面
	plane = glm::vec4(m[0][3] - m[0][1], m[1][3] - m[1][1], m[2][3] - m[2][1], m[3][3] - m[3][1]);
	length = glm::length(glm::vec3(plane));
	planes[3].normal = glm::vec3(plane) / length;
	planes[3].distance = plane.w / length;

	// 从矩阵中提取 far 平面
	plane = glm::vec4(m[0][3] - m[0][2], m[1][3] - m[1][2], m[2][3] - m[2][2], m[3][3] - m[3][2]);
	length = glm::length(glm::vec3(plane));
	planes[4].normal = glm::vec3(plane) / length;
	planes[4].distance = plane.w / length;

	// 从矩阵中提取 near 平面
	plane = glm::vec4(m[0][3] + m[0][2], m[1][3] + m[1][2], m[2][3] + m[2][2], m[3][3] + m[3][2]);
	length = glm::length(glm::vec3(plane));
	planes[5].normal = glm::vec3(plane) / length;
	planes[5].distance = plane.w / length;
}

bool Frustum::AABBInside(AABBBoundingBox& aabb)
{
	//TODO 现在每帧更新，可移到渲染组件
	AABBBoundingBox temp = aabb.ConvertNewAABB();

	// 对每个视锥体平面进行测试
	for (int i = 0; i < 6; i++)
	{
		const Plane& plane = planes[i];
		const glm::vec3& N = plane.normal;

		// 找到 AABB 在平面法向量正方向上的最远点（N-vertex）
		// 如果这个点在平面负侧，说明整个AABB都在平面负侧（视锥体外）
		// 如果法向量指向正方向，选择max；如果指向负方向，选择min
		glm::vec3 nVertex;

		nVertex.x = (N.x >= 0.0f) ? temp.max.x : temp.min.x;
		nVertex.y = (N.y >= 0.0f) ? temp.max.y : temp.min.y;
		nVertex.z = (N.z >= 0.0f) ? temp.max.z : temp.min.z;

		// 计算N-vertex到平面的距离
		// 如果距离 < 0，说明 AABB 完全在平面负侧（视锥体外），可以剔除
		float distance = plane.GetDistance(nVertex);
		if (distance < 0.0f)
			return false;
	}

	// 所有平面测试都通过，AABB 在视锥体内或与视锥体相交
	return true;
}

bool Frustum::SphereInside(SphereBoundingBox& sphere)
{
	for (int i = 0; i < 6; i++)
	{
		if (planes[i].GetDistance(sphere.center) < -sphere.radius)
			return false;
	}

	return true;
}

bool Frustum::PointInside(const glm::vec3& point)
{
	for (int i = 0; i < 6; i++)
	{
		if (planes[i].GetDistance(point) < 0)
			return false;
	}

	return true;
}


