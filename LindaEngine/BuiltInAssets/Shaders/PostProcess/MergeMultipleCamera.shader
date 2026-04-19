Shader
{
	Pass
	{
		Properties
		{
			uniform sampler2D mainTexture = white;
			uniform sampler2D currentTexture = white;
			uniform float factor = 1.0;
			uniform vec4 offsetUV = [0.0, 0.0, 1.0, 1.0];
		}

		#include "BuiltInAssets/Shaders/ShaderLibrary/PostProcessVertexFunction.glsl"
		
		Fragment
		{
			in vec2 uv;

			void main()
			{
				float inside = all(greaterThan(uv, offsetUV.xy)) && all(lessThan(uv, offsetUV.zw)) ? 1.0 : 0.0;

				vec3 srcCol = texture(mainTexture, uv).rgb;
				srcCol = pow(srcCol, vec3(1.0/2.2)); 
				vec3 currentCol = texture(currentTexture, uv).rgb;
				float finalFactor = mix(srcCol.r, factor, factor);
				finalFactor = step(0.01, finalFactor);
				srcCol = mix(currentCol, srcCol, finalFactor * inside);
				FragColor = vec4(srcCol, 1.0);
			}
		}
	}
}  
