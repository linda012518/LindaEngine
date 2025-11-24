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
			uniform int testInt = 1;
			uniform ivec4 testIVec4 = [7, 8, 9, 0];

			uniform float testFloat = 0.1;
			uniform vec4 testVec4 = [0.7, 0.8, 0.9, 0.0];

			uniform sampler2D maskTexture = white;
			uniform samplerCube skybox = white;

			uniform mat4 testMat4;
		}
		
		RenderState
		{
			ColorMask (RGB)
			DepthState (depthTest=true; depthWrite=false; depthFunc= LESS)
			CullFaceFunc(BACK)
			StencilState(stencilTest=true)
			BlendState(enableBlend=true;blendSrc=SRC_ALPHA;blendDis=ONE_MINUS_SRC_ALPHA;constantColor=[1,0.2, 0, 1.0])
			PolygonModeState(LINE)
		}

		Keywords
		{
			NORMAL_MAP, TESTKEYWORDS
		}

		Vertex
		{
			AttributeNameArray { aPosition, aColor, aUV0 }

			out vec2 uv;

			void main()
			{
				uv = aUV0;
				gl_Position = _linda_Matrix_VP * _localToWorld * vec4(aPosition, 1.0);
			}
		}
		
		Fragment
		{
			out vec4 FragColor;

			in vec2 uv;

			void main()
			{
				FragColor.rgb = vec3(0.3, 0.0, 0.0);
				FragColor.a = 1.0;
				FragColor = testVec4;
				FragColor = texture(maskTexture, uv);
			}
		}
	}
}  
