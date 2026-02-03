Shader
{
	Pass
	{
		LightMode "Adjunct"
		
		RenderState
		{
			DepthState (depthFunc= LEQUAL)
			PolygonModeState(LINE)
		}

		Vertex
		{
			void main()
			{
				gl_Position = _linda_Matrix_VP * _localToWorld * vec4(aPosition, 1.0);
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
