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

			void main()
			{
				vec3 N = normalize(worldNormal);

				vec3 irradiance = vec3(0.0);   
    
				// tangent space calculation from origin point
				vec3 up    = vec3(0.0, 1.0, 0.0);
				vec3 right = normalize(cross(up, N));
				up         = normalize(cross(N, right));
       
				float sampleDelta = 0.025;
				float nrSamples = 0.0;
				for(float phi = 0.0; phi < 2.0 * linda_PI; phi += sampleDelta)
				{
					for(float theta = 0.0; theta < 0.5 * linda_PI; theta += sampleDelta)
					{
						// spherical to cartesian (in tangent space)
						vec3 tangentSample = vec3(sin(theta) * cos(phi),  sin(theta) * sin(phi), cos(theta));
						// tangent space to world
						vec3 sampleVec = tangentSample.x * right + tangentSample.y * up + tangentSample.z * N; 

						irradiance += texture(linda_PanoramicCube, sampleVec).rgb * cos(theta) * sin(theta);
						nrSamples++;
					}
				}
				irradiance = linda_PI * irradiance * (1.0 / float(nrSamples));
    
				FragColor = vec4(irradiance, 1.0);
			}
		}
	}
}  
