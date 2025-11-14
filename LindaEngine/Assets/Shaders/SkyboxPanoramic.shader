Shader
{
	RenderType "Skybox"

	Pass
	{
		LightMode "Skybox"

		Properties
		{
			uniform sampler2D skybox = white;
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
			out vec4 FragColor;

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

            vec2 DecodeDir2LatLongUV(vec3 dir)
            {
                //解码全景图，不用转立方体贴图，如果有接缝不好，关掉minmap
				vec3 normalizedCoords = normalize(dir);
				float latitude = acos(normalizedCoords.y);
				float longitude = atan(normalizedCoords.z, normalizedCoords.x);
				vec2 sphereCoords = vec2(longitude, latitude) * vec2(0.5 / PI, 1.0 / PI);
				return vec2(0.5, 1.0) - sphereCoords;
            }

			void main()
			{
				vec2 uv = DecodeDir2LatLongUV(normalize(worldNormal));
				uv.y = 1.0 - uv.y;
				FragColor = texture(skybox, uv);
			}
		}
	}
}  
