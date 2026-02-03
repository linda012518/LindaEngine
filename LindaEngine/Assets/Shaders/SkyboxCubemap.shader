Shader
{
	RenderType "Skybox"

	Pass
	{
		LightMode "Skybox"

		Properties
		{
			uniform samplerCube skybox = white;
		}
		
		RenderState
		{
			DepthState (depthTest=true; depthWrite=true; depthFunc= LEQUAL)
		}

		Vertex
		{
			AttributeNameArray { aPosition }

			out vec3 worldNormal;

			void main()
			{
				worldNormal = aPosition;
				mat4 view = _linda_Matrix_V;
				view[3].xyz = vec3(0.0);
				view[3].w = 1.0;
				gl_Position = _linda_Matrix_P * view * vec4(aPosition, 1.0);
				gl_Position = gl_Position.xyww;
			}
		}
		
		Fragment
		{
			in vec3 worldNormal;

			void main()
			{
				FragColor = texture(skybox, normalize(worldNormal));
			}
		}
	}
}  
