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
			uniform sampler2D mainTexture = white;
			uniform vec4 mainTexture_ST = [1.0, 1.0, 0.0, 0.0];
			uniform Color color = [1.0, 1.0, 1.0, 1.0];
		}

		Vertex
		{
			out vec2 uv;

			void main()
			{
				uv = Transform_UV(aUV0, mainTexture);
				gl_Position = linda_Matrix_VP * linda_LocalToWorld * vec4(aPosition, 1.0);
			}
		}
		
		Fragment
		{
			in vec2 uv;

			void main()
			{
				FragColor = texture(mainTexture, uv) * color;
			}
		}
	}
}  
