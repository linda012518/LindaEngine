Shader
{
	RenderQueue "2000"
	RenderType "Opaque"
	FallbackPass

	Pass
	{
		LightMode "Color"

		Properties
		{
			uniform sampler2D albedoMap = white;
			uniform vec4 albedoMap_ST = [1.0, 1.0, 0.0, 0.0];
			uniform vec4 albedoMap_TexelSize = [1.0, 1.0, 1.0, 1.0];
			uniform Color albedoColor = [1.0, 1.0, 1.0, 1.0];

			uniform sampler2D normalMap = bump;
			uniform float normalIntensity = 1.0;

			uniform sampler2D emissionMap = white;
			uniform HDRColor emissionColor = [1.0, 1.0, 1.0, 1.0];

			uniform sampler2D metallicMap = white;
			uniform float metallicIntensity = 1.0;
			
			uniform sampler2D roughnessMap = gray;
			uniform float roughnessIntensity = 0.5;

			uniform sampler2D aoMap = white;
			uniform float aoIntensity = 1.0;
		}

		Vertex
		{
			struct Varyings 
			{
				vec2 uv;
				vec3 positionWS;
				vec3 normalWS;
				vec3 tangentWS;
			};
			
			out Varyings fragInput;

			void main()
			{
				fragInput.uv = Transform_UV(aUV0, albedoMap);
				fragInput.positionWS = (linda_LocalToWorld * vec4(aPosition, 1.0)).xyz;
				fragInput.normalWS = normalize(aNormal * mat3(linda_WorldToLocal));
				fragInput.tangentWS = normalize(mat3(linda_LocalToWorld) * aTangent);

				gl_Position = linda_Matrix_VP * linda_LocalToWorld * vec4(aPosition, 1.0);
			}
		}
		
		Fragment
		{
			#include "BuiltInAssets/Shaders/ShaderLibrary/GlobalIllumination.glsl"
			#include "BuiltInAssets/Shaders/ShaderLibrary/SurfaceData.glsl"

			struct Varyings 
			{
				vec2 uv;
				vec3 positionWS;
				vec3 normalWS;
				vec3 tangentWS;
			};
			
			in Varyings fragInput;

			#define MIN_REFLECTIVITY 0.04  // 非金属的基础反射率（F0）

			float OneMinusReflectivityMetallic(float metallic)
			{
				// We'll need oneMinusReflectivity, so
				//   1-reflectivity = 1-lerp(dielectricSpec, 1, metallic) = lerp(1-dielectricSpec, 0, metallic)
				// store (1-dielectricSpec) alpha, then
				//   1-reflectivity = lerp(alpha, 0, metallic) = alpha + metallic*(0 - alpha) =
				//                  = alpha - metallic * alpha
				// 不考虑非金属固有反射率 retrun 1.0 - metallic;
				// 考虑非金属固有反射率 return lerp(1.0 - MIN_REFLECTIVITY, 0, metallic);
				float oneMinusDielectricSpec = 1.0 - MIN_REFLECTIVITY;
				return oneMinusDielectricSpec - metallic * oneMinusDielectricSpec;
			}

			vec3 UnpackNormalRGB(vec3 packedNormal, float scale)
			{
				vec3 normal;
				normal.xyz = packedNormal.rgb * vec3(2.0) - vec3(1.0);
				normal.xy *= vec2(scale);
				return normal;
			}

float Pow2 (float x)
{
    return x*x;
}
float Pow4 (float x)
{
    return x*x * x*x;
}
float Pow5(float x)
{
	return x*x * x*x * x;
}

vec3 Diffuse_Lambert_UE4( vec3 DiffuseColor )
{
	return DiffuseColor * vec3(1.0 / linda_PI);
}

float D_GGX_UE4( float a2, float NoH )
{
	float d = ( NoH * a2 - NoH ) * NoH + 1;	// 2 mad
	return a2 / ( linda_PI*d*d );			// 4 mul, 1 rcp
}

float Vis_SmithJointApprox_UE4( float a2, float NoV, float NoL )
{
	float a = sqrt(a2);
	float Vis_SmithV = NoL * ( NoV * ( 1.0 - a ) + a );
	float Vis_SmithL = NoV * ( NoL * ( 1.0 - a ) + a );
	return 0.5 * (1.0 / ( Vis_SmithV + Vis_SmithL ));
}

vec3 F_Schlick_UE4( vec3 SpecularColor, float VoH )
{
	float Fc = Pow5( 1.0 - VoH );				// 1 sub, 3 mul
	//return Fc + (1 - Fc) * SpecularColor;		// 1 add, 3 mad
	
	// Anything less than 2% is physically impossible and is instead considered to be shadowing
	return vec3(clamp( 50.0 * SpecularColor.g, 0.0, 1.0 ) * Fc) + vec3(1.0 - Fc) * SpecularColor;
	
}

vec3 SpecularGGX_UE4( float Roughness, vec3 SpecularColor, vec3 N, vec3 V, vec3 L )
{
	float a2 = Pow4( Roughness );
	vec3 H = normalize(L + V);
	float NoH = clamp(dot(N, H), 0.0, 1.0 );
	float NoV = clamp(abs(dot(N, V)) + 1e-5, 0.0, 1.0 );
	float NoL = clamp(dot(N, L), 0.0, 1.0 );
	float VoH = clamp(dot(V, H), 0.0, 1.0 );//VoH=LoH
	
	// Generalized microfacet specular
	float D = D_GGX_UE4( a2, NoH );
	float Vis = Vis_SmithJointApprox_UE4( a2, NoV, NoL );
	vec3 F = F_Schlick_UE4( SpecularColor, VoH );

	return (D * Vis) * F;
}

vec3 StandardBRDF_UE4( vec3 DiffuseColor, vec3 SpecularColor, float Roughness, vec3 N, vec3 V, vec3 L )
{
	vec3 diffuseTerm = Diffuse_Lambert_UE4( DiffuseColor );
	vec3 specularTerm = SpecularGGX_UE4(Roughness, SpecularColor, N, V, L);

	return diffuseTerm + specularTerm;
}

vec3 StandardShading_UE4( vec3 DiffuseColor, vec3 SpecularColor, float Roughness, vec3 L, vec3 V, vec3 N, vec3 lightColor )
{
	vec3 brdf = StandardBRDF_UE4( DiffuseColor, SpecularColor, Roughness, N, V, L );
	float ndotl = clamp(dot(N, L), 0.0, 1.0 );

	return brdf * vec3(ndotl) * lightColor * vec3(linda_PI);
}

vec3 AOMultiBounce( vec3 BaseColor, float AO )
{
	vec3 a =  2.0404 * BaseColor - 0.3324;
	vec3 b = -4.7951 * BaseColor + 0.6417;
	vec3 c =  2.7552 * BaseColor + 0.6903;
	return max( vec3(AO), vec3( ( AO * a + b ) * AO + c ) * AO );
}

float GetSpecularOcclusion(float NoV, float RoughnessSq, float AO)
{
	return clamp( pow( NoV + AO, RoughnessSq ) - 1 + AO, 0.0, 1.0  );
}

vec3 EnvBRDFApprox_UE4( vec3 SpecularColor, float Roughness, float NoV )
{
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

vec3 StandardBRDF_Env_UE4( vec3 DiffuseColor, vec3 SpecularColor, float Roughness, vec3 WorldPos, vec3 N, vec3 V, float ao )
{
	vec3 diffuseAO = AOMultiBounce(DiffuseColor, ao);
	vec3 radianceSH = ShadeSH9(N);
	vec3 indirectDiffuse = radianceSH * DiffuseColor * diffuseAO;//Õý³£Ó¦¸ÃÊÇ radianceSH * Diffuse_Lambert_UE4( DiffuseColor ); µ«SampleSHÒÑ¾­³ýPIÁË

	vec3 R = reflect(-V, N);
	float NoV = clamp(abs(dot(N, V)) + 1e-5, 0.0, 1.0 );
	vec3 specularLD = GlossyEnvironmentReflection(-R, WorldPos, Roughness, ao);
	vec3 specularDFG = EnvBRDFApprox_UE4(SpecularColor, Roughness, NoV);
	float specular = GetSpecularOcclusion(NoV, Roughness * Roughness, ao);
	vec3 specularAO = AOMultiBounce(SpecularColor, specular); 
	vec3 indirectSpecular = specularLD * specularDFG * specularAO;
	return indirectDiffuse + indirectSpecular;
}

			void main()
			{
				//vec3 albedo = texture(albedoMap, fragInput.uv).rgb;
				vec3 albedo = pow(texture(albedoMap, fragInput.uv).rgb, vec3(2.2));
				float metallic = texture(metallicMap, fragInput.uv).r;
				float roughness = texture(roughnessMap, fragInput.uv).r;
				float ao = texture(aoMap, fragInput.uv).r;
				vec3 emission = texture(emissionMap, fragInput.uv).rgb;
				vec3 normalTS = texture(normalMap, fragInput.uv).rgb;
				normalTS = UnpackNormalRGB(normalTS, normalIntensity);

				vec3 normal = normalize(fragInput.normalWS);
				vec3 tangent = normalize(fragInput.tangentWS);
				vec3 bitangent = normalize(cross(fragInput.normalWS, fragInput.tangentWS));
				mat3 TBN = mat3(tangent, bitangent, normal);
				vec3 normalWS = normalize(TBN * normalTS);

				vec3 diffuseColor = albedo * OneMinusReflectivityMetallic(metallic);
				vec3 specularColor = mix(vec3(MIN_REFLECTIVITY), albedo, metallic);
				// V: surface → camera; L: surface → directional light source (−forward, same convention as ProceduralSkybox sun)
				vec3 viewDir = normalize(linda_CameraPosition.xyz - fragInput.positionWS);
				vec3 lightDir = normalize(-linda_DirLightDirections[0].xyz);

				vec3 directLighting = StandardShading_UE4(diffuseColor, specularColor, roughness, lightDir, viewDir, normalWS, linda_DirLightColors[0].rgb);
				vec3 indirectLighting = StandardBRDF_Env_UE4(diffuseColor, specularColor, roughness, fragInput.positionWS, normalWS, viewDir, ao);
				
				vec3 finalLighting = directLighting + indirectLighting;

				FragColor = vec4(finalLighting, 1.0);
			}
		}
	}
}  
