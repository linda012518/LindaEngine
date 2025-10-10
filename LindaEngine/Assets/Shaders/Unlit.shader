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

			out vec3 color;
			out vec2 uv;

			uniform mat4 model;
			uniform mat4 view;
			uniform mat4 projection;

			void main()
			{
				gl_Position = 	projection * view * model * vec4(aPos, 1.0);
				color 		= 	aColor;
				uv			=	aTexCoord;
			}
		}
		
		Fragment
		{
			#version 330 core

			out vec4 FragColor;

			in vec3 color;
			in vec2 uv;

			uniform sampler2D texture1;

			void main()
			{
				FragColor.rgb = texture(texture1, uv).rgb * color;
				FragColor.a = 1.0;
			}
		}
	}

	Pass
	{
		Name "ShadowCaster"

		#include "DirectionalShadowPass.glsl"
	}

	Pass
	{
		Name "Depth"

		#include "DepthPass.glsl"
	}

	Pass
	{
		Name "DepthNormal"

		#include "DepthNormalPass.glsl"
	}
}  
