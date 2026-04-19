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
			#include "BuiltInAssets/Shaders/ShaderLibrary/StandardLighting.glsl"

			struct Varyings 
			{
				vec2 uv;
				vec3 positionWS;
				vec3 normalWS;
				vec3 tangentWS;
			};
			
			in Varyings fragInput;

			void main()
			{
				//vec3 albedo = texture(albedoMap, fragInput.uv).rgb;
				vec3 albedo = pow(texture(albedoMap, fragInput.uv).rgb, vec3(2.2));
				float metallic = texture(metallicMap, fragInput.uv).r;
				float roughness = texture(roughnessMap, fragInput.uv).r;
				float ao = texture(aoMap, fragInput.uv).r;
				vec3 emission = texture(emissionMap, fragInput.uv).rgb;
				vec3 normalTS = texture(normalMap, fragInput.uv).rgb;
				normalTS = UnpackNormal(normalTS, normalIntensity);

				vec3 normal = normalize(fragInput.normalWS);
				vec3 tangent = normalize(fragInput.tangentWS);
				vec3 bitangent = normalize(cross(fragInput.normalWS, fragInput.tangentWS));
				mat3 TBN = mat3(tangent, bitangent, normal);
				vec3 normalWS = normalize(TBN * normalTS);

				vec3 diffuseColor = albedo * OneMinusReflectivityMetallic(metallic);
				vec3 specularColor = mix(vec3(MIN_REFLECTIVITY), albedo, metallic);
				// V: surface → camera; L: surface → directional light source (−forward, same convention as ProceduralSkybox sun)
				vec3 viewDir = normalize(linda_CameraPosition.xyz - fragInput.positionWS);

				vec3 lighting = vec3(0.0);
				for (int i = 0; i < linda_LightCount.x; i++)
				{
					Light light = GetDirectionalLight(i, fragInput.positionWS);
					lighting += StandardShading(diffuseColor, specularColor, roughness, light.direction, viewDir, normalWS, light.color) * light.attenuation;
				}
				for (int i = 0; i < linda_LightCount.y; i++)
				{
					Light light = GetPointLight(i, fragInput.positionWS);
					lighting += StandardShading(diffuseColor, specularColor, roughness, light.direction, viewDir, normalWS, light.color) * light.attenuation;
				}
				for (int i = 0; i < linda_LightCount.z; i++)
				{
					Light light = GetSpotLight(i, fragInput.positionWS);
					lighting += StandardShading(diffuseColor, specularColor, roughness, light.direction, viewDir, normalWS, light.color) * light.attenuation;
				}
				vec3 indirectLighting = StandardBRDF_Env(diffuseColor, specularColor, roughness, fragInput.positionWS, normalWS, viewDir, ao);

				FragColor = vec4(lighting + indirectLighting, 1.0);
			}
		}
	}
}  
