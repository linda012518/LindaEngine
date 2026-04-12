Shader
{
	Pass
	{
		Properties
		{
			uniform sampler2D mainTexture = white;
			uniform vec4 mainTexture_TexelSize = [1.0, 1.0, 1.0, 1.0];
			uniform vec4 visibleEdgeColor = [1.0, 0.0, 0.0, 1.0];
			uniform vec4 hiddenEdgeColor = [0.0, 1.0, 0.0, 1.0];
		}

		#include "BuiltInAssets/Shaders/ShaderLibrary/PostProcessVertexFunction.glsl"
		
		Fragment
		{
			in vec2 uv;

			void main()
			{
				vec2 invSize = mainTexture_TexelSize.zw;
				vec4 uvOffset = vec4(1.0, 0.0, 0.0, 1.0) * vec4(invSize, invSize);
				vec4 c1 = texture( mainTexture, uv + uvOffset.xy);
				vec4 c2 = texture( mainTexture, uv - uvOffset.xy);
				vec4 c3 = texture( mainTexture, uv + uvOffset.yw);
				vec4 c4 = texture( mainTexture, uv - uvOffset.yw);
				float diff1 = (c1.r - c2.r) * 0.5;
				float diff2 = (c3.r - c4.r) * 0.5;
				float d = length( vec2(diff1, diff2) );
				float a1 = min(c1.g, c2.g);
				float a2 = min(c3.g, c4.g);
				float visibilityFactor = min(a1, a2);
				vec3 edgeColor = 1.0 - visibilityFactor > 0.001 ? visibleEdgeColor.rgb : hiddenEdgeColor.rgb;
				FragColor = vec4(edgeColor, 1.0) * vec4(d);
			}
		}
	}
}  
