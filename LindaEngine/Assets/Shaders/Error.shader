Shader
{
	Pass
	{
		Vertex
		{
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
				FragColor = vec4(1.0, 0.0, 1.0, 1.0);
			}
		}
	}
}  
