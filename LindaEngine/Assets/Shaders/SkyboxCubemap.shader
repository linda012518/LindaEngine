Shader
{
	RenderType "Skybox"

	Pass
	{
		LightMode "Skybox"

		Properties
		{
			uniform samplerCube skybox = white;
		}
		
		RenderState
		{
			DepthState (depthTest=true; depthWrite=true; depthFunc= LEQUAL)
		}

		#include "BuiltInAssets/Shaders/ShaderLibrary/SkyboxVertexFunction.glsl"
		
		Fragment
		{
			in vec3 worldNormal;

			void main()
			{
				FragColor = texture(skybox, normalize(worldNormal));
				//FragColor.rgb = FragColor.rgb / (FragColor.rgb + vec3(1.0));
				//FragColor.rgb = pow(FragColor.rgb, vec3(1.0 / 2.2));
			}
		}
	}
}  
