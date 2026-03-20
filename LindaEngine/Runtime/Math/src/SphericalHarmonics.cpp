#include "SphericalHarmonics.h"

using namespace LindaEngine;

SHCoefficients SphericalHarmonics::ComputeSHFromCubemap(std::vector<std::vector<glm::vec3>> faces, int cubeSize, bool srgb)
{
    SHCoefficients sh;

    std::vector<float> shR(9, 0.0f), shG(9, 0.0f), shB(9, 0.0f);
    float totalWeight = 0.0f;

    // 遍历所有texel
    for (int face = 0; face < 6; face++)
    {
        for (int y = 0; y < cubeSize; y++)
        {
            for (int x = 0; x < cubeSize; x++)
            {
                // 将像素坐标转换到[-1,1]范围
                float u = ((x + 0.5f) / cubeSize) * 2.0f - 1.0f;
                float v = ((y + 0.5f) / cubeSize) * 2.0f - 1.0f;

                // 计算方向向量
                glm::vec3 dir = UVToDirection(face, (x + 0.5f) / cubeSize, (y + 0.5f) / cubeSize);
                float lenSq = glm::dot(dir, dir);
                dir = glm::normalize(dir);

                // 计算立体角权重（关键！）
                // 对于cubemap，权重是 1 / (x^2+y^2+1)^(3/2)
                float weight = 1.0f / (lenSq * sqrt(lenSq));

                glm::vec3 color = faces[face][y * cubeSize + x];
                if (true == srgb)
                {
                    color.r = SRGBToLinear(color.r);
                    color.g = SRGBToLinear(color.g);
                    color.b = SRGBToLinear(color.b);
                }
                // 累加
                for (int i = 0; i < 9; i++)
                {
                    float basis = SHBasis(i, dir);
                    shR[i] += color.r * basis * weight;
                    shG[i] += color.g * basis * weight;
                    shB[i] += color.b * basis * weight;
                }
                totalWeight += weight;
            }
        }
    }

    // 归一化：乘以 4*PI / totalWeight
    float norm = (4.0f * glm::pi<float>()) / totalWeight;
    for (int i = 0; i < 9; i++) {
        sh.coeffs[i] = glm::vec3(shR[i], shG[i], shB[i]) * norm;
    }

    ApplyCosineKernel(sh);

    return sh;
}

glm::vec3 SphericalHarmonics::UVToDirection(int face, float u, float v)
{
    // u,v 在 [0,1] 范围内，转换到 [-1,1]
    float x = u * 2.0f - 1.0f;
    float y = v * 2.0f - 1.0f;

    switch (face) {
    case 0: return glm::vec3(1.0f, -y, -x); // +X
    case 1: return glm::vec3(-1.0f, -y, x); // -X
    case 2: return glm::vec3(x, 1.0f, y); // +Y
    case 3: return glm::vec3(x, -1.0f, -y); // -Y
    case 4: return glm::vec3(x, -y, 1.0f); // +Z
    case 5: return glm::vec3(-x, -y, -1.0f); // -Z
    default: return glm::vec3(0.0f);
    }
}

float SphericalHarmonics::SHBasis(int index, const glm::vec3& dir)
{
    float x = dir.x, y = dir.y, z = dir.z;

    switch (index) {
    case 0: // l=0, m=0  (常数项)
        return 0.282095f; // sqrt(1/4pi)

    case 1: // l=1, m=-1
        return -0.488603f * y; // -sqrt(3/4pi) * y

    case 2: // l=1, m=0
        return 0.488603f * z;  // sqrt(3/4pi) * z

    case 3: // l=1, m=1
        return -0.488603f * x; // -sqrt(3/4pi) * x

    case 4: // l=2, m=-2
        return 1.092548f * x * y; // sqrt(15/4pi) * xy

    case 5: // l=2, m=-1
        return -1.092548f * y * z; // -sqrt(15/4pi) * yz

    case 6: // l=2, m=0
        return 0.315392f * (3.0f * z * z - 1.0f); // sqrt(5/16pi) * (3z^2-1)

    case 7: // l=2, m=1
        return -1.092548f * x * z; // -sqrt(15/4pi) * xz

    case 8: // l=2, m=2
        return 0.546274f * (x * x - y * y); // sqrt(15/16pi) * (x^2-y^2)

    default:
        return 0.0f;
    }
}

void SphericalHarmonics::ApplyCosineKernel(SHCoefficients& sh)
{
    // Ramamoorthi 2001 的解析系数
    float A[9] = {
        3.141593f,              // l=0
        2.094395f, 2.094395f, 2.094395f,  // l=1
        0.785398f, 0.785398f, 0.785398f, 0.785398f, 0.785398f // l=2
    };

    for (int i = 0; i < 9; i++) {
        sh.coeffs[i] *= A[i];
    }
}

float SphericalHarmonics::LinearToSRGB(float linear) {
    return linear <= 0.0031308f ? linear * 12.92f : 1.055f * powf(linear, 1.0f / 2.4f) - 0.055f;
}

float SphericalHarmonics::SRGBToLinear(float srgb)
{
    return (srgb <= 0.04045f) ? srgb / 12.92f : std::pow((srgb + 0.055f) / 1.055f, 2.4f);
}
