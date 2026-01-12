// FrustumCuller.cpp
#include "FrustumCuller.h"

using namespace Graphics;

void FrustumCuller::updateFromMatrix(const glm::mat4& viewProjMatrix) {
    // 从组合的视图投影矩阵提取平面
    const glm::mat4 matrix = glm::transpose(viewProjMatrix);
    
    // 左平面
    m_planes[0].normal = glm::vec3(matrix[0][3] + matrix[0][0],
                                   matrix[1][3] + matrix[1][0],
                                   matrix[2][3] + matrix[2][0]);
    m_planes[0].distance = matrix[3][3] + matrix[3][0];
    
    // 右平面
    m_planes[1].normal = glm::vec3(matrix[0][3] - matrix[0][0],
                                   matrix[1][3] - matrix[1][0],
                                   matrix[2][3] - matrix[2][0]);
    m_planes[1].distance = matrix[3][3] - matrix[3][0];
    
    // 下平面
    m_planes[2].normal = glm::vec3(matrix[0][3] + matrix[0][1],
                                   matrix[1][3] + matrix[1][1],
                                   matrix[2][3] + matrix[2][1]);
    m_planes[2].distance = matrix[3][3] + matrix[3][1];
    
    // 上平面
    m_planes[3].normal = glm::vec3(matrix[0][3] - matrix[0][1],
                                   matrix[1][3] - matrix[1][1],
                                   matrix[2][3] - matrix[2][1]);
    m_planes[3].distance = matrix[3][3] - matrix[3][1];
    
    // 近平面
    m_planes[4].normal = glm::vec3(matrix[0][3] + matrix[0][2],
                                   matrix[1][3] + matrix[1][2],
                                   matrix[2][3] + matrix[2][2]);
    m_planes[4].distance = matrix[3][3] + matrix[3][2];
    
    // 远平面
    m_planes[5].normal = glm::vec3(matrix[0][3] - matrix[0][2],
                                   matrix[1][3] - matrix[1][2],
                                   matrix[2][3] - matrix[2][2]);
    m_planes[5].distance = matrix[3][3] - matrix[3][2];
    
    // 归一化所有平面
    for (auto& plane : m_planes) {
        normalizePlane(plane);
    }
}

void FrustumCuller::updateFromCamera(const glm::vec3& position,
                                    const glm::vec3& direction,
                                    const glm::vec3& up,
                                    float fovY,
                                    float aspectRatio,
                                    float zNear,
                                    float zFar) {
    // 计算视图投影矩阵
    glm::mat4 viewMatrix = glm::lookAt(position, position + direction, up);
    glm::mat4 projectionMatrix = glm::perspective(fovY, aspectRatio, zNear, zFar);
    glm::mat4 viewProjMatrix = projectionMatrix * viewMatrix;
    
    updateFromMatrix(viewProjMatrix);
}

void FrustumCuller::normalizePlane(FrustumCuller::Plane& plane) {
    float length = glm::length(plane.normal);
    if (length > 0.0f) {
        float invLength = 1.0f / length;
        plane.normal *= invLength;
        plane.distance *= invLength;
    }
}

float FrustumCuller::distanceToPlane(const FrustumCuller::Plane& plane, const glm::vec3& point) const {
    return glm::dot(plane.normal, point) + plane.distance;
}

bool FrustumCuller::isPointVisible(const glm::vec3& point) const {
    for (const auto& plane : m_planes) {
        if (distanceToPlane(plane, point) < 0.0f) {
            return false;
        }
    }
    return true;
}

FrustumCuller::Visibility FrustumCuller::testSphere(const FrustumCuller::Sphere& sphere) const {
    bool fullyInside = true;
    
    for (const auto& plane : m_planes) {
        float distance = distanceToPlane(plane, sphere.center);
        
        if (distance < -sphere.radius) {
            return FrustumCuller::Visibility::INVISIBLE;
        }
        
        if (distance < sphere.radius) {
            fullyInside = false;
        }
    }
    
    return fullyInside ? FrustumCuller::Visibility::FULLY_VISIBLE : FrustumCuller::Visibility::PARTIALLY_VISIBLE;
}

FrustumCuller::Visibility FrustumCuller::testAABB(const FrustumCuller::AABB& aabb) const {
    glm::vec3 center = aabb.getCenter();
    glm::vec3 extents = aabb.getExtents();
    
    bool fullyInside = true;
    
    for (const auto& plane : m_planes) {
        // 计算AABB在平面法线方向上的投影半径
        float radius = glm::dot(extents, glm::abs(plane.normal));
        
        // 计算中心到平面的距离
        float distance = distanceToPlane(plane, center);
        
        if (distance < -radius) {
            return FrustumCuller::Visibility::INVISIBLE;
        }
        
        if (distance < radius) {
            fullyInside = false;
        }
    }
    
    return fullyInside ? FrustumCuller::Visibility::FULLY_VISIBLE : FrustumCuller::Visibility::PARTIALLY_VISIBLE;
}

FrustumCuller::Visibility FrustumCuller::testOBB(const FrustumCuller::OBB& obb) const {
    bool fullyInside = true;
    
    for (const auto& plane : m_planes) {
        // 将平面法线转换到OBB的局部空间
        glm::vec3 localNormal = glm::transpose(obb.orientation) * plane.normal;
        
        // 计算OBB在平面法线方向上的投影半径
        float radius = glm::dot(obb.extents, glm::abs(localNormal));
        
        // 计算中心到平面的距离
        float distance = distanceToPlane(plane, obb.center);
        
        if (distance < -radius) {
            return FrustumCuller::Visibility::INVISIBLE;
        }
        
        if (distance < radius) {
            fullyInside = false;
        }
    }
    
    return fullyInside ? FrustumCuller::Visibility::FULLY_VISIBLE : FrustumCuller::Visibility::PARTIALLY_VISIBLE;
}

std::vector<FrustumCuller::Visibility> FrustumCuller::testSpheres(const std::vector<FrustumCuller::Sphere>& spheres) const {
    std::vector<FrustumCuller::Visibility> results;
    results.reserve(spheres.size());
    
    for (const auto& sphere : spheres) {
        results.push_back(testSphere(sphere));
    }
    
    return results;
}

std::vector<FrustumCuller::Visibility> FrustumCuller::testAABBs(const std::vector<FrustumCuller::AABB>& aabbs) const {
    std::vector<FrustumCuller::Visibility> results;
    results.reserve(aabbs.size());
    
    for (const auto& aabb : aabbs) {
        results.push_back(testAABB(aabb));
    }
    
    return results;
}

std::array<glm::vec3, 8> FrustumCuller::getCorners() const {
    // 通过平面相交计算视锥体角点
    // 这是一个简化的实现，实际使用中可能需要更精确的计算
    
    std::array<glm::vec3, 8> corners;
    
    // 计算近平面和远平面的中心点
    glm::vec3 nearCenter = -m_planes[4].normal * m_planes[4].distance;
    glm::vec3 farCenter = -m_planes[5].normal * m_planes[5].distance;
    
    // 计算近平面和远平面的上、右向量
    glm::vec3 up = m_planes[3].normal;
    glm::vec3 right = m_planes[1].normal;
    
    // 计算半尺寸（这是一个近似）
    float nearHeight = 2.0f * m_planes[4].distance;
    float nearWidth = nearHeight; // 假设为正方形
    
    // 计算8个角点
    corners[0] = nearCenter - up * nearHeight * 0.5f - right * nearWidth * 0.5f;
    corners[1] = nearCenter - up * nearHeight * 0.5f + right * nearWidth * 0.5f;
    corners[2] = nearCenter + up * nearHeight * 0.5f + right * nearWidth * 0.5f;
    corners[3] = nearCenter + up * nearHeight * 0.5f - right * nearWidth * 0.5f;
    
    corners[4] = farCenter - up * nearHeight - right * nearWidth;
    corners[5] = farCenter - up * nearHeight + right * nearWidth;
    corners[6] = farCenter + up * nearHeight + right * nearWidth;
    corners[7] = farCenter + up * nearHeight - right * nearWidth;
    
    return corners;
}

FrustumCuller::AABB FrustumCuller::transformAABB(const FrustumCuller::AABB& aabb, const glm::mat4& transform) {
    // 使用变换后的8个顶点计算新的AABB
    std::array<glm::vec3, 8> vertices = {
        glm::vec3(aabb.min.x, aabb.min.y, aabb.min.z),
        glm::vec3(aabb.max.x, aabb.min.y, aabb.min.z),
        glm::vec3(aabb.min.x, aabb.max.y, aabb.min.z),
        glm::vec3(aabb.max.x, aabb.max.y, aabb.min.z),
        glm::vec3(aabb.min.x, aabb.min.y, aabb.max.z),
        glm::vec3(aabb.max.x, aabb.min.y, aabb.max.z),
        glm::vec3(aabb.min.x, aabb.max.y, aabb.max.z),
        glm::vec3(aabb.max.x, aabb.max.y, aabb.max.z)
    };
    
    FrustumCuller::AABB result;
    
    for (const auto& vertex : vertices) {
        glm::vec4 transformed = transform * glm::vec4(vertex, 1.0f);
        glm::vec3 pos = glm::vec3(transformed) / transformed.w;
        
        result.min = glm::min(result.min, pos);
        result.max = glm::max(result.max, pos);
    }
    
    return result;
}

FrustumCuller::Sphere FrustumCuller::transformSphere(const FrustumCuller::Sphere& sphere, const glm::mat4& transform) {
    // 变换球体中心
    glm::vec4 centerTransformed = transform * glm::vec4(sphere.center, 1.0f);
    glm::vec3 newCenter = glm::vec3(centerTransformed) / centerTransformed.w;
    
    // 计算变换后的半径（使用均匀缩放的最大值）
    glm::vec3 scale = glm::vec3(glm::length(transform[0]),
                                glm::length(transform[1]),
                                glm::length(transform[2]));
    float maxScale = glm::max(glm::max(scale.x, scale.y), scale.z);
    float newRadius = sphere.radius * maxScale;
    
    return {newCenter, newRadius};
}

