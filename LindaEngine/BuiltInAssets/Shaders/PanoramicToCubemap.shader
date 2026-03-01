Shader
{
	Pass
	{
		Properties
		{
			uniform sampler2D linda_PanoramicCube = white;
		}
		
		RenderState
		{
			DepthState (depthTest=true; depthWrite=true; depthFunc= LEQUAL)
		}

		Vertex
		{
			AttributeNameArray { aPosition }

			out vec3 worldNormal;

			uniform mat4 linda_Matrix_VP_PanoramicCube;

			void main()
			{
				worldNormal = aPosition;
				gl_Position = linda_Matrix_VP_PanoramicCube * vec4(aPosition, 1.0);
			}
		}
		
		Fragment
		{
			in vec3 worldNormal;

			vec2 SampleSphericalMap(vec3 v)
			{
				vec2 uv = vec2(atan(v.z, v.x), asin(v.y));
				float halfDivPI = 0.5 / PI;
				vec2 invAtan = vec2(halfDivPI, halfDivPI * 2.0);
				uv *= invAtan;
				uv += 0.5;
				return uv;
			}

			void main()
			{
				vec2 uv = SampleSphericalMap(normalize(worldNormal));
				FragColor = texture(linda_PanoramicCube, uv);
			}
		}
	}
}  
