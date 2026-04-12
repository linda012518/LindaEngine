Shader
{
	Pass
	{
		Properties
		{
			uniform sampler2D mainTexture = white;
		}

		#include "BuiltInAssets/Shaders/ShaderLibrary/PostProcessVertexFunction.glsl"
		
		Fragment
		{
			in vec2 uv;

			void main()
			{
				vec3 srcCol = texture(mainTexture, uv).rgb;
				srcCol = srcCol / (srcCol + vec3(1.0));
				FragColor = vec4(srcCol, 1.0);
			}
		}
	}
}  
