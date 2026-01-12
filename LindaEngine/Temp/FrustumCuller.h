#pragma once

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include <array>
#include <vector>
#include <limits>

namespace Graphics 
{
    class FrustumCuller 
    {
    public:
        enum class Visibility {
            FULLY_VISIBLE,   // 完全可见
            PARTIALLY_VISIBLE, // 部分可见（相交）
            INVISIBLE        // 完全不可见
        };

        struct Plane {
            glm::vec3 normal;    // 平面法线（已归一化）
            float distance;      // 原点到平面的距离
        };

        struct AABB {
            glm::vec3 min;
            glm::vec3 max;

            AABB() : min(std::numeric_limits<float>::max()),
                max(std::numeric_limits<float>::lowest()) {
            }

            AABB(const glm::vec3& min, const glm::vec3& max) : min(min), max(max) {}

            glm::vec3 getCenter() const { return (min + max) * 0.5f; }
            glm::vec3 getExtents() const { return (max - min) * 0.5f; }
        };

        struct OBB {
            glm::vec3 center;
            glm::vec3 extents;
            glm::mat3 orientation; // 旋转矩阵
        };

        struct Sphere {
            glm::vec3 center;
            float radius;
        };

    public:
        FrustumCuller() = default;

        /**
         * @brief 从视图投影矩阵更新视锥体平面
         * @param viewProjMatrix 视图投影矩阵（通常为 projection * view）
         */
        void updateFromMatrix(const glm::mat4& viewProjMatrix);

        /**
         * @brief 从相机参数构建视锥体
         * @param position 相机位置
         * @param direction 相机方向（已归一化）
         * @param up 相机上向量
         * @param fovY 垂直视野（弧度）
         * @param aspectRatio 宽高比
         * @param zNear 近平面距离
         * @param zFar 远平面距离
         */
        void updateFromCamera(const glm::vec3& position,
            const glm::vec3& direction,
            const glm::vec3& up,
            float fovY,
            float aspectRatio,
            float zNear,
            float zFar);

        // 基本测试方法
        bool isPointVisible(const glm::vec3& point) const;
        FrustumCuller::Visibility testSphere(const FrustumCuller::Sphere& sphere) const;
        FrustumCuller::Visibility testAABB(const FrustumCuller::AABB& aabb) const;
        FrustumCuller::Visibility testOBB(const FrustumCuller::OBB& obb) const;

        // 批量测试方法（更高效）
        std::vector<FrustumCuller::Visibility> testSpheres(const std::vector<FrustumCuller::Sphere>& spheres) const;
        std::vector<FrustumCuller::Visibility> testAABBs(const std::vector<FrustumCuller::AABB>& aabbs) const;

        // 获取视锥体平面（用于调试或高级用途）
        const std::array<FrustumCuller::Plane, 6>& getPlanes() const { return m_planes; }

        // 获取视锥体角点（用于调试或可视化）
        std::array<glm::vec3, 8> getCorners() const;

        // 辅助方法
        static FrustumCuller::AABB transformAABB(const FrustumCuller::AABB& aabb, const glm::mat4& transform);
        static FrustumCuller::Sphere transformSphere(const FrustumCuller::Sphere& sphere, const glm::mat4& transform);

    private:
        void normalizePlane(FrustumCuller::Plane& plane);
        float distanceToPlane(const FrustumCuller::Plane& plane, const glm::vec3& point) const;

    private:
        std::array<FrustumCuller::Plane, 6> m_planes; // 左, 右, 下, 上, 近, 远
    };

} 