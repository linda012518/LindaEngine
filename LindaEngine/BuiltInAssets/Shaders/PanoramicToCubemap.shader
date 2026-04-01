Shader
{
	RenderType "Skybox"

	Pass
	{
		LightMode "Skybox"

		Properties
		{
			uniform sampler2D linda_PanoramicCube = white;
		}
		
		RenderState
		{
			DepthState (depthTest=true; depthWrite=true; depthFunc= LEQUAL)
		}

		#include "ShaderLibrary/SkyboxVertexFunction.glsl"

		Fragment
		{
			in vec3 worldNormal;

			vec2 SampleSphericalMap(vec3 v)
			{
				vec2 uv = vec2(atan(v.z, v.x), asin(v.y));
				float halfDivPI = 0.5 / linda_PI;
				vec2 invAtan = vec2(halfDivPI, halfDivPI * 2.0);
				uv *= invAtan;
				uv += 0.5;
				return uv;
			}

			void main()
			{
				vec2 uv = SampleSphericalMap(normalize(worldNormal));
				FragColor = texture(linda_PanoramicCube, uv);
			}
		}
	}
}  
