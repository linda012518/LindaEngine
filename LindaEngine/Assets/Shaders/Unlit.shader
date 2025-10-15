Shader
{
	Pass
	{
		Vertex
		{
			AttributeNameArray { aPosition, aColor, aUV0 }
			//layout (location = 0) in vec3 aPos;
			//layout (location = 1) in vec3 aColor;
			//layout (location = 2) in vec2 aTexCoord;

// #define
// #undef
// #if
// #ifdef

// #ifndef
// #else
// #elif
// #endif

// #if USE_VERTEX_COLOR
//   in vec4 v_color;
// #endif

			out vec3 color;
			out vec2 uv;

			UniformArray
			{
				uniform mat4 model;
				uniform mat4 view;
				uniform mat4 projection;
			}

			void main()
			{
				gl_Position = 	projection * view * model * vec4(aPosition, 1.0);
				color 		= 	aColor;
				uv			=	aUV0;
			}
		}
		
		Fragment
		{
			out vec4 FragColor;

			in vec3 color;
			in vec2 uv;

			UniformArray
			{
				uniform sampler2D texture1;
			}

			void main()
			{
				FragColor.rgb = texture(texture1, uv).rgb * color;
				FragColor.a = 1.0;
			}
		}
	}

	FallbackPass Pass
	{
		LightMode "ShadowCaster"

		#include "DirectionalShadowPass.glsl"
	}

	Pass
	{
		LightMode "Depth"

		#include "DepthPass.glsl"
	}

	Pass
	{
		LightMode "DepthNormal"

		#include "DepthNormalPass.glsl"
	}
}  
