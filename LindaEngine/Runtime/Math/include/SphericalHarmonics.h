#pragma once

#include <cmath>
#include <vector>
#include <array>
#include <cstdint>

#include "glm/glm.hpp"
#include "glm/ext/scalar_constants.hpp"

namespace LindaEngine
{
    // 球谐系数结构体 (L=2阶，9个系数，RGB三个通道)
    struct SHCoefficients 
    {
        glm::vec3 coeffs[9]; // 9个系数，每个是RGB vec3

        SHCoefficients() 
        {
            for (int i = 0; i < 9; i++) coeffs[i] = glm::vec3(0.0f);
        }
    };

    class SphericalHarmonics 
    {
    public:
        // 更高效的实现：使用Compute Shader或离线预处理
        // 这里提供一个基于CPU的精确实现，使用固定网格采样
        static SHCoefficients ComputeSHFromCubemap(std::vector<std::vector<glm::vec3>> faces, int cubeSize, bool srgb = false);

    private:
        // 将立方体面+UV转换为3D方向
        static glm::vec3 UVToDirection(int face, float u, float v);

        // 计算球谐基函数 Y(l,m) 在方向 (theta, phi) 处的值
        // 使用归一化的笛卡尔坐标方向 dir (假设 |dir| = 1)
        static float SHBasis(int index, const glm::vec3& dir);

        // 在C++端预乘漫反射余弦核，Shader端可直接使用
        static void ApplyCosineKernel(SHCoefficients& sh);

        static float LinearToSRGB(float linear);

        static float SRGBToLinear(float srgb);
    };

}
