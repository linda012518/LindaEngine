
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

#define LINDA_SPECCUBE_LOD_STEPS 5.0

float PerceptualRoughnessToMipmapLevel(float perceptualRoughness)
{
    perceptualRoughness = perceptualRoughness * (1.7 - 0.7 * perceptualRoughness);

    return perceptualRoughness * LINDA_SPECCUBE_LOD_STEPS;
}

vec3 GlossyEnvironmentReflection(vec3 reflectVector, vec3 positionWS, float perceptualRoughness, float occlusion)
{
    float mip = PerceptualRoughnessToMipmapLevel(perceptualRoughness);
    vec3 irradiance = textureLod(linda_PrefilterSpecCube, -reflectVector, perceptualRoughness * LINDA_SPECCUBE_LOD_STEPS).rgb;
    return irradiance * occlusion;
}

vec3 EnvBRDFApprox( vec3 SpecularColor, float Roughness, float NoV )
{
    // 拟合式环境BRDF，不需要采样 IBL BRDF LUT 图，采样LUT方式 F * brdf.x + brdf.y

	// [ Lazarov 2013, "Getting More Physical in Call of Duty: Black Ops II" ]
	// Adaptation to fit our G term.
	const vec4 c0 = { -1, -0.0275, -0.572, 0.022 };
	const vec4 c1 = { 1, 0.0425, 1.04, -0.04 };
	vec4 r = Roughness * c0 + c1;
	float a004 = min( r.x * r.x, exp2( -9.28 * NoV ) ) * r.x + r.y;
	vec2 AB = vec2( -1.04, 1.04 ) * a004 + r.zw;

	// Anything less than 2% is physically impossible and is instead considered to be shadowing
	// Note: this is needed for the 'specular' show flag to work, since it uses a SpecularColor of 0
	AB.y *= clamp( 50.0 * SpecularColor.g, 0.0, 1.0 );

	return SpecularColor * AB.x + AB.y;
}

vec3 SpecularIBL(vec3 R,vec3 WorldPos,float Roughness,vec3 SpecularColor,float NoV)
{	
	vec3 SpeucularLD = GlossyEnvironmentReflection(R,WorldPos,Roughness,1.0f);
	vec3 SpecularDFG = EnvBRDFApprox(SpecularColor,Roughness,NoV);
	return SpeucularLD * SpecularDFG;
}

float GetSpecularOcclusion(float NoV, float RoughnessSq, float AO)
{
    // Specular Occlusion，基于环境光遮蔽（AO）对镜面反射的影响进行调整，提升暗部细节表现
    // [ Epic Games 2016, "Moving Frostbite to Physically Based Rendering, Part 3" ]
	return clamp( pow( NoV + AO, RoughnessSq ) - 1 + AO, 0.0, 1.0  );
}

vec3 AOMultiBounce( vec3 BaseColor, float AO )
{
    // 使AO溢色更明显，增加二次反弹AO的贡献，提升暗部细节表现
    // 二次反弹AO，基于 [ Epic Games 2016, "Moving Frostbite to Physically Based Rendering, Part 3" ]
	vec3 a =  2.0404 * BaseColor - 0.3324;
	vec3 b = -4.7951 * BaseColor + 0.6417;
	vec3 c =  2.7552 * BaseColor + 0.6903;
	return max( vec3(AO), vec3( ( AO * a + b ) * AO + c ) * AO );
}