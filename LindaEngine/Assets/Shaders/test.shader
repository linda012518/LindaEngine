Shader
{
	Pass
	{
		LightMode "customLightMode"
		Vertex
		{
			AttributeNameArray { aPosition, aColor, aUV0 }
			
			uniform mat4 localToWorld;
			uniform mat4 lindaMatrixVP;

			void main()
			{
				gl_Position = lindaMatrixVP * localToWorld * vec4(aPosition, 1.0);
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
