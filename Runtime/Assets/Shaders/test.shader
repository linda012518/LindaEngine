Name
{
	Pass
	{
		Vertex
		{
			#version 330 core

			layout (location = 0) in vec3 aPos;
			layout (location = 1) in vec3 aColor;
			layout (location = 2) in vec2 aTexCoord;

			out vec3 ourColor;
			out vec2 TexCoord;

			void main()
			{
				gl_Position = 	vec4(aPos, 1.0);
				ourColor 	= 	aColor;
				TexCoord	=	aTexCoord;
			}
		}
		
		Fragment
		{
		#if defined( USE_COLOR_ALPHA )

			vColor = vec4( 1.0 );

		#elif defined( USE_COLOR ) || defined( USE_INSTANCING_COLOR ) || defined( USE_BATCHING_COLOR )

			vColor = vec3( 1.0 );

		#endif

		#ifdef USE_INSTANCING_COLOR

			vColor.xyz *= instanceColor.xyz;

		#endif
		}
	}

	Pass
	{
		Vertex
		{
			#version 330 core

			layout (location = 0) in vec3 aPos;
			layout (location = 1) in vec3 aColor;
			layout (location = 2) in vec2 aTexCoord;

			out vec3 ourColor;
			out vec2 TexCoord;

			void main()
			{
				gl_Position = 	vec4(aPos, 1.0);
				ourColor 	= 	aColor;
				TexCoord	=	aTexCoord;
			}
		}
		
		Fragment
		{
			
		}
	}
}
