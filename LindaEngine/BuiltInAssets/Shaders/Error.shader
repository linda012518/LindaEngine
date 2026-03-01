Shader
{
	Pass
	{
		Vertex
		{
			void main()
			{
				gl_Position = linda_Matrix_VP * linda_LocalToWorld * vec4(aPosition, 1.0);
			}
		}
		
		Fragment
		{
			void main()
			{
				FragColor = vec4(1.0, 0.0, 1.0, 1.0);
			}
		}
	}
}  
