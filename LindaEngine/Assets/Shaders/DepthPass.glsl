Pass
{
	LightMode "Depth"

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
		AttributeNames { aPosition }

		uniform mat4 model;
		uniform mat4 view;
		uniform mat4 projection;

		void main()
		{
			gl_Position = projection * view * model * vec4(aPosition, 1.0);
		}
	}
		
	Fragment
	{
		void main()
		{
			gl_FragDepth = gl_FragCoord.z;
		}
	}

}
