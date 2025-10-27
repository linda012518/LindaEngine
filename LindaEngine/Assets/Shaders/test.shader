Shader
{
	RenderQueue "2000"
	RenderType "Opaque"
	//FallbackPass

	Pass
	{
		LightMode "customLightMode"

		Properties
		{
		
		}

		RenderState
		{
		
		}

		Keywords
		{
		
		}

		Vertex
		{
			AttributeNameArray { aPosition, aColor, aUV0 }

			uniform mat4 _localToWorld;
			uniform mat4 _worldToLocal;

			uniform mat4 _linda_Matrix_VP; //TODO ¡Ÿ ±≤‚ ‘”√

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
				FragColor.rgb = vec3(0.6, 0.0, 0.0);
				FragColor.a = 1.0;
			}
		}
	}
}  
