Shader
{
	Pass
	{
		Properties
		{
			uniform sampler2D mainTexture = white;
			uniform sampler2D maskTexture = white;
			uniform sampler2D edgeTexture1 = white;
			uniform sampler2D edgeTexture2 = white;
			uniform float edgeStrength = 2.0;
			uniform float edgeGlow = 1.0;
		}

		Vertex
		{
			out vec2 uv;

			void main()
			{
				float x = (gl_VertexID == 1) ? 3.0 : -1.0;
				float y = (gl_VertexID == 2) ? 3.0 : -1.0;
				
				gl_Position = vec4(x, y, 0.0, 1.0);
				
				uv.x = (gl_VertexID == 1) ? 2.0 : 0.0;
				uv.y = (gl_VertexID == 2) ? 2.0 : 0.0;
			}
		}
		
		Fragment
		{
			in vec2 uv;

			void main()
			{
				vec4 srcCol = texture(mainTexture, uv);

				vec4 edgeValue1 = texture(edgeTexture1, uv);
				vec4 edgeValue2 = texture(edgeTexture2, uv);
				vec4 maskColor = texture(maskTexture, uv);
				float visibilityFactor = 1.0 - maskColor.r > 0.0 ? 1.0 : 0.5;
				vec4 edgeValue = edgeValue1 + edgeValue2 * edgeGlow;
				edgeValue = edgeValue1;

				vec4 ccc = edgeValue * edgeStrength * (1.0 - maskColor.r);
				FragColor = mix(srcCol, vec4(1.0, 1.0, 1.0, 1.0) * (sin(linda_Time.y * 3.0) / 6.0 + 0.5) * 2.0, ccc.r);
			}
		}
	}
}  
