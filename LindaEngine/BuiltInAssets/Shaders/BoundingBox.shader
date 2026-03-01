Shader
{
	Pass
	{
		LightMode "Adjunct"
		
		RenderState
		{
			DepthState (depthWrite=false;depthFunc= LEQUAL)
		}

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
				FragColor = vec4(0.0, 0.8, 0.0, 1.0);
			}
		}
	}
}  
