#include "BoundingBox.h"
#include "Mathf.h"

using namespace LindaEngine;

AABBBoundingBox::AABBBoundingBox()
{
	Reset();
}

void AABBBoundingBox::Reset()
{
	min.x = Mathf::MaxValue;
	min.y = Mathf::MaxValue;
	min.z = Mathf::MaxValue;

	max.x = Mathf::MinValue;
	max.y = Mathf::MinValue;
	max.z = Mathf::MinValue;

	size.x = center.x = 0.0f;
	size.y = center.y = 0.0f;
	size.z = center.z = 0.0f;
}

void AABBBoundingBox::AddVertex(glm::vec3& vertex)
{
	min.x = glm::min(min.x, vertex.x);
	min.y = glm::min(min.y, vertex.y);
	min.z = glm::min(min.z, vertex.z);

	max.x = glm::max(max.x, vertex.x);
	max.y = glm::max(max.y, vertex.y);
	max.z = glm::max(max.z, vertex.z);
}

void AABBBoundingBox::CalculateCenterSize()
{
	center.x = (min.x + max.x) * 0.5f;
	center.y = (min.y + max.y) * 0.5f;
	center.z = (min.z + max.z) * 0.5f;

	size.x = max.x - min.x;
	size.y = max.y - min.y;
	size.z = max.z - min.z;
}

AABBBoundingBox AABBBoundingBox::WorldSpaceAABB(const glm::mat4& transform, glm::vec3& min, glm::vec3& max)
{
	AABBBoundingBox aabb;

	std::vector<glm::vec3> vertices = {
		glm::vec3(min.x, min.y, min.z),
		glm::vec3(max.x, min.y, min.z),
		glm::vec3(min.x, max.y, min.z),
		glm::vec3(max.x, max.y, min.z),
		glm::vec3(min.x, min.y, max.z),
		glm::vec3(max.x, min.y, max.z),
		glm::vec3(min.x, max.y, max.z),
		glm::vec3(max.x, max.y, max.z)
	};

	aabb.min = glm::vec3(Mathf::MaxValue);
	aabb.max = glm::vec3(Mathf::MinValue);

	for (const auto& vertex : vertices) {
		glm::vec4 transformed = transform * glm::vec4(vertex, 1.0f);
		glm::vec3 pos = glm::vec3(transformed) / transformed.w;

		aabb.min = glm::min(aabb.min, pos);
		aabb.max = glm::max(aabb.max, pos);
	}

	aabb.CalculateCenterSize();

	return aabb;
}

AABBBoundingBox AABBBoundingBox::Merge(const AABBBoundingBox& a, const AABBBoundingBox& b)
{
	AABBBoundingBox aabb;

	aabb.min.x = glm::min(a.min.x, b.min.x);
	aabb.min.y = glm::min(a.min.y, b.min.y);
	aabb.min.z = glm::min(a.min.z, b.min.z);

	aabb.max.x = glm::max(a.max.x, b.max.x);
	aabb.max.y = glm::max(a.max.y, b.max.y);
	aabb.max.z = glm::max(a.max.z, b.max.z);

	aabb.CalculateCenterSize();

	return aabb;
}

//////////////////////////////////////////////////////////////////////////////

OBBBoundingBox::OBBBoundingBox()
{
	Reset();
}

void OBBBoundingBox::Reset()
{
	halfExtents.x = center.x = 0.0f;
	halfExtents.y = center.y = 0.0f;
	halfExtents.z = center.z = 0.0f;

	axies[0] = glm::vec3(1.0f, 0.0f, 0.0f);
	axies[1] = glm::vec3(0.0f, 1.0f, 0.0f);
	axies[2] = glm::vec3(0.0f, 0.0f, 1.0f);
}

void OBBBoundingBox::AddVertex(glm::vec3& vertex)
{

}

/////////////////////////////////////////////////////////////////////////////////////////

SphereBoundingBox::SphereBoundingBox()
{
	Reset();
}

void SphereBoundingBox::Reset()
{
	center = glm::vec3(0.0f);
	radius = 0.0f;

	min.x = Mathf::MaxValue;
	min.y = Mathf::MaxValue;
	min.z = Mathf::MaxValue;

	max.x = Mathf::MinValue;
	max.y = Mathf::MinValue;
	max.z = Mathf::MinValue;
}

void SphereBoundingBox::AddVertex(glm::vec3& vertex)
{
	min.x = glm::min(min.x, vertex.x);
	min.y = glm::min(min.y, vertex.y);
	min.z = glm::min(min.z, vertex.z);

	max.x = glm::max(max.x, vertex.x);
	max.y = glm::max(max.y, vertex.y);
	max.z = glm::max(max.z, vertex.z);
}

void SphereBoundingBox::CalculateCenterSize()
{
	center.x = (min.x + max.x) * 0.5f;
	center.y = (min.y + max.y) * 0.5f;
	center.z = (min.z + max.z) * 0.5f;

	radius = glm::length(glm::vec3(max.x - min.x, max.y - min.y, max.z - min.z));
}

SphereBoundingBox SphereBoundingBox::Merge(const SphereBoundingBox& a, const SphereBoundingBox& b)
{
	SphereBoundingBox sphere;

	sphere.min.x = glm::min(a.min.x, b.min.x);
	sphere.min.y = glm::min(a.min.y, b.min.y);
	sphere.min.z = glm::min(a.min.z, b.min.z);

	sphere.max.x = glm::max(a.max.x, b.max.x);
	sphere.max.y = glm::max(a.max.y, b.max.y);
	sphere.max.z = glm::max(a.max.z, b.max.z);

	sphere.CalculateCenterSize();

	return sphere;
}

