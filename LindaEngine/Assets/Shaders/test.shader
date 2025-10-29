Shader
{
	RenderQueue "2000"
	RenderType "Opaque"
	FallbackPass

	Pass
	{
		LightMode "customLightMode"

		Properties
		{
			uniform int testInt = 1;
			uniform ivec2 testIVec2 = (2, 3);
			uniform ivec3 testIVec3 = (4, 5, 6);
			uniform ivec4 testIVec4 = (7, 8, 9, 0);

			uniform float testFloat = 0.1;
			uniform vec2 testVec2 = (0.2, 0.3);
			uniform vec3 testVec3 = (0.4, 0.5, 0.6);
			uniform vec4 testVec4 = (0.7, 0.8, 0.9, 0.0);

			uniform sampler2D maskTexture = "white";
			uniform samplerCube skybox = "white";

			uniform mat4 testMat4;
		}
		
		RenderState
		{
			ColorMask (RGB)
			DepthState (depthTest=true, depthWrite=false, depthFunc= LESS)
			CullFaceFunc(BACK)
			StencilState(stencilTest=true)
			BlendState(enableBlend=true,blendSrc=SRC_ALPHA,blendDis=ONE_MINUS_SRC_ALPHA)
			PolygonModeState(LINE)
		}

		Keywords
		{
			NORMAL_MAP, TESTKEYWORDS
		}

		Vertex
		{
			AttributeNameArray { aPosition, aColor, aUV0 }

			void main()
			{
				gl_Position = _linda_Matrix_VP * _localToWorld * vec4(aPosition, 1.0);
			}
		}
		
		Fragment
		{
			out vec4 FragColor;

			void main()
			{
				FragColor.rgb = vec3(0.3, 0.0, 0.0);
				FragColor.a = 1.0;
			}
		}
	}
}  
