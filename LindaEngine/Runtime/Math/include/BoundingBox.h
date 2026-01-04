#pragma once

#include <glm/glm.hpp>

#include <vector>

namespace LindaEngine
{
	struct BoundingBox
	{
		virtual ~BoundingBox() = default;
	};

	struct AABBBoundingBox : public BoundingBox
	{
		glm::vec3 min;
		glm::vec3 max;
		glm::vec3 center;
		glm::vec3 size;

		AABBBoundingBox();
		void Reset();
		void AddVertex(glm::vec3& vertex);
		void CalculateCenterSize();
		AABBBoundingBox ConvertNewAABB();

		static AABBBoundingBox Merge(const AABBBoundingBox& a, const AABBBoundingBox& b);
	};

	struct SkinnedAABBBoundingBox : public AABBBoundingBox
	{
		//TODO 记录每个骨骼影响顶点的AABB，每帧用骨骼矩阵计算AABB的更新，不计算每个顶点
		std::vector<AABBBoundingBox> boneAABBs;
	};

	struct OBBBoundingBox : public BoundingBox
	{
		glm::vec3 center;
		glm::vec3 axies[3];    // 三个正交轴
		glm::vec3 halfExtents; // 三个方向半长

		OBBBoundingBox();
		void Reset();
		void AddVertex(glm::vec3& vertex);
	};

	struct SphereBoundingBox : public BoundingBox
	{
		glm::vec3 center;
		float radius;

		SphereBoundingBox();
		void Reset();
		void AddVertex(glm::vec3& vertex);
		void CalculateCenterSize();

		static SphereBoundingBox Merge(const SphereBoundingBox& a, const SphereBoundingBox& b);

	private:
		glm::vec3 min;
		glm::vec3 max;
	};
}
