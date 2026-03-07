Shader
{
	Pass
	{
		LightMode "Adjunct"
		
		RenderState
		{
			DepthState (depthWrite=false;depthFunc= LEQUAL)
		}

		Properties
		{
			uniform vec4 color = [0.85, 0.85, 0.4, 1.0];
		}

		Vertex
		{
			uniform mat4 linda_LightLocalToWorld;

			void main()
			{
				gl_Position = linda_Matrix_VP * linda_LightLocalToWorld * vec4(aPosition, 1.0);
			}
		}
		
		Fragment
		{
			void main()
			{
				FragColor = color;
			}
		}
	}
}  
