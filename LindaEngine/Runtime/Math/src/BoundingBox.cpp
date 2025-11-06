#include "BoundingBox.h"

using namespace LindaEngine;

AABBBoundingBox::AABBBoundingBox()
{
	Reset();
}

void AABBBoundingBox::Reset()
{
	min.x = -FLT_MAX;
	min.y = -FLT_MAX;
	min.z = -FLT_MAX;

	max.x = FLT_MAX;
	max.y = FLT_MAX;
	max.z = FLT_MAX;

	size.x = center.x = 0.0f;
	size.y = center.y = 0.0f;
	size.z = center.z = 0.0f;
}

void AABBBoundingBox::AddVertex(glm::vec3& vertex)
{
	min.x = std::min(min.x, vertex.x);
	min.y = std::min(min.y, vertex.y);
	min.z = std::min(min.z, vertex.z);

	max.x = std::max(max.x, vertex.x);
	max.y = std::max(max.y, vertex.y);
	max.z = std::max(max.z, vertex.z);
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

AABBBoundingBox AABBBoundingBox::Merge(const AABBBoundingBox& a, const AABBBoundingBox& b)
{
	AABBBoundingBox aabb;

	aabb.min.x = std::min(a.min.x, b.min.x);
	aabb.min.y = std::min(a.min.y, b.min.y);
	aabb.min.z = std::min(a.min.z, b.min.z);

	aabb.max.x = std::max(a.max.x, b.max.x);
	aabb.max.y = std::max(a.max.y, b.max.y);
	aabb.max.z = std::max(a.max.z, b.max.z);

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

	min.x = -FLT_MAX;
	min.y = -FLT_MAX;
	min.z = -FLT_MAX;

	max.x = FLT_MAX;
	max.y = FLT_MAX;
	max.z = FLT_MAX;
}

void SphereBoundingBox::AddVertex(glm::vec3& vertex)
{
	min.x = std::min(min.x, vertex.x);
	min.y = std::min(min.y, vertex.y);
	min.z = std::min(min.z, vertex.z);

	max.x = std::max(max.x, vertex.x);
	max.y = std::max(max.y, vertex.y);
	max.z = std::max(max.z, vertex.z);
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

	sphere.min.x = std::min(a.min.x, b.min.x);
	sphere.min.y = std::min(a.min.y, b.min.y);
	sphere.min.z = std::min(a.min.z, b.min.z);

	sphere.max.x = std::max(a.max.x, b.max.x);
	sphere.max.y = std::max(a.max.y, b.max.y);
	sphere.max.z = std::max(a.max.z, b.max.z);

	sphere.CalculateCenterSize();

	return sphere;
}

