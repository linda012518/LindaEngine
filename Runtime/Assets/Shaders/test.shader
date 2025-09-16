Shader
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

			#include "test1/test.hlsl"
			#include "../test3.hlsl"
			#include "test2.hlsl"

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
		Name=Shadow
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
			222222222222222222
		}
	}

	Pass
	{
		Name = Depth
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
			111111111111111
		}
	}

	Pass
	{
		Name=DepthNormal
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
			3333333333333333333
		}
	}
}  
