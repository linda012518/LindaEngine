
// 重建辐照度（漫反射光照）
vec3 ShadeSH9(vec3 normal) 
{
    vec3 n = normalize(normal);
    float x = n.x, y = n.y, z = n.z;
    
    // 预计算常用项
    float xx = x * x;
    float yy = y * y;
    float zz = z * z;
    float xy = x * y;
    float yz = y * z;
    float xz = x * z;
    
    // 直接展开9个基函数的计算
    vec3 irradiance = vec3(0.0);
    
    // l=0, m=0 (常数项)
    irradiance += linda_SHCoeffs[0].rgb * 0.282095;
    
    // l=1 (线性项)
    irradiance += linda_SHCoeffs[1].rgb * (-0.488603 * y);
    irradiance += linda_SHCoeffs[2].rgb * (0.488603 * z);
    irradiance += linda_SHCoeffs[3].rgb * (-0.488603 * x);
    
    // l=2 (二次项)
    irradiance += linda_SHCoeffs[4].rgb * (1.092548 * xy);
    irradiance += linda_SHCoeffs[5].rgb * (-1.092548 * yz);
    irradiance += linda_SHCoeffs[6].rgb * (0.315392 * (3.0 * zz - 1.0));
    irradiance += linda_SHCoeffs[7].rgb * (-1.092548 * xz);
    irradiance += linda_SHCoeffs[8].rgb * (0.546274 * (xx - yy));
    
    // 线性空间
    // 最终还需要乘以PBR的漫反射系数（1/PI）和颜色
    // return max(irradiance, 0.0);

    // 直接在sh里面计算好
    irradiance = max(irradiance, 0.0);
    // 在外面转gamma
    // return pow(max(irradiance / linda_PI, 0.0), vec3(1.0 / 2.2));
    return irradiance / linda_PI;
}