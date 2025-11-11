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
			// ColorMask (RGB)
			DepthState (depthTest=true; depthWrite=true; depthFunc= LEQUAL)
			// CullFaceFunc(None)
			// StencilState(stencilTest=true)
			// BlendState(enableBlend=true;blendSrc=SRC_ALPHA;blendDis=ONE_MINUS_SRC_ALPHA;constantColor=[1,0.2, 0, 1.0])
			// PolygonModeState(LINE)
		}

		Vertex
		{
			AttributeNameArray { aPosition }

			out vec3 worldNormal;

			void main()
			{
				worldNormal = aPosition;
				vec4 pos = vec4(aPosition, 1.0);
				gl_Position = pos.xyww;
			}
		}
		
		Fragment
		{
			out vec4 FragColor;

			in vec3 worldNormal;

			void main()
			{
				FragColor = texture(skybox, normalize(worldNormal));
				// FragColor = vec4(0.8, 0.0, 0.6, 1.0);
				// FragColor.rgb = vec3(1.0, 1.0, 0.0);
				// FragColor.a = 1.0;
			}
		}
	}
}  
