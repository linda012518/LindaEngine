#include "Physics.h"
#include "Renderer.h"
#include "RendererSystem.h"
#include "glm/ext/scalar_constants.hpp"

#include <algorithm>

using namespace LindaEngine;

RaycastHit Physics::Raycast(Ray ray)
{
	RaycastHit hit;

    std::vector<Weak<Renderer>>& renderers = RendererSystem::GetRenderers();
    for (auto& go : renderers)
    {
        AABBBoundingBox& box = go->GetBoundingBox();
        SphereBoundingBox sphere;
        sphere.center = box.center;
        sphere.radius = glm::length(box.size) * 0.5f;
        HitResult result = RayIntersectSphere(ray, sphere);
        result = RayIntersectAABB(ray, box);
        if (result.hit)
        {
            result.entity = &go->GetEntity();
            hit.entities.push_back(result);
        }
    }

    std::sort(hit.entities.begin(), hit.entities.end(), [](const HitResult a, const HitResult b) { return a.t < b.t; });

	return hit;
}

HitResult Physics::RayIntersectAABB(const Ray& ray, const AABBBoundingBox& aabb)
{
    HitResult result;
    float tmin = 0.0f;
    float tmax = std::numeric_limits<float>::max();

    // 对三个轴分别测试
    for (int i = 0; i < 3; i++) 
    {
        if (std::abs(ray.direction[i]) < glm::epsilon<float>()) 
        {
            // 射线平行于该轴，检查原点是否在范围内
            if (ray.origin[i] < aabb.min[i] || ray.origin[i] > aabb.max[i]) 
            {
                return result;
            }
        }
        else 
        {
            // 计算与两个平面的交点参数
            float invD = 1.0f / ray.direction[i];
            float t1 = (aabb.min[i] - ray.origin[i]) * invD;
            float t2 = (aabb.max[i] - ray.origin[i]) * invD;

            if (t1 > t2) std::swap(t1, t2);

            tmin = std::max(tmin, t1);
            tmax = std::min(tmax, t2);

            if (tmin > tmax) 
            {
                return result; // 无交点
            }
        }
    }

    // 找到有效交点
    if (tmax < 0) 
    {
        return result; // AABB在射线后方
    }

    float t = (tmin < 0) ? tmax : tmin; // 如果在内部，取出口点
    result.t = t;
    result.point = ray.At(t);
    result.hit = true;

    return result;
}

HitResult Physics::RayIntersectSphere(const Ray& ray, const SphereBoundingBox& sphere)
{
    HitResult result;

    glm::vec3 oc = ray.origin - sphere.center;
    float a = glm::dot(ray.direction, ray.direction);
    float b = 2.0f * glm::dot(oc, ray.direction);
    float c = glm::dot(oc, oc) - sphere.radius * sphere.radius;

    float discriminant = b * b - 4 * a * c;

    if (discriminant < 0) 
    {
        return result; // 无实数解，不相交
    }

    float sqrtDisc = std::sqrt(discriminant);
    float t = (-b - sqrtDisc) / (2.0f * a); // 取较近的点

    if (t < 0) 
    {
        t = (-b + sqrtDisc) / (2.0f * a); // 尝试较远的点（射线起点在球内）
        if (t < 0) 
        {
            return result; // 两个交点都在射线后方
        }
    }

    result.t = t;
    result.hit = true;
    result.point = ray.At(t);

    return result;
}
