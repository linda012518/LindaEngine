Shader
{
	Pass
	{
		Properties
		{
			uniform samplerCube skybox = white;
		}

		Vertex
		{
			out vec3 worldNormal;
			uniform mat4 linda_CubemapVisibleMatrix_VP;

			void main()
			{
				worldNormal = aPosition;
				gl_Position = linda_CubemapVisibleMatrix_VP * vec4(aPosition, 1.0);
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
