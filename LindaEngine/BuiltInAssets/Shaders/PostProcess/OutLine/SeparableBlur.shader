Shader
{
	Pass
	{
		Properties
		{
			uniform sampler2D mainTexture = white;
			uniform vec4 direction_texelSize = [1.0, 0.0, 1.0, 0.0];
			uniform float kernelRadius = 1.0;
			uniform float maxRadius = 4.5;
		}

		Vertex
		{
			out vec2 uv;

			void main()
			{
				float x = (gl_VertexID == 1) ? 3.0 : -1.0;
				float y = (gl_VertexID == 2) ? 3.0 : -1.0;
				
				gl_Position = vec4(x, y, 0.0, 1.0);
				
				uv.x = (gl_VertexID == 1) ? 2.0 : 0.0;
				uv.y = (gl_VertexID == 2) ? 2.0 : 0.0;
			}
		}
		
		Fragment
		{
			in vec2 uv;

			float GaussianPdf(float x, float sigma) {
				return 0.39894 * exp( -0.5 * x * x / ( sigma * sigma)) / sigma;
			}

			void main()
			{
				vec2 direction = direction_texelSize.xy;
				vec2 invSize = 1.0 / direction_texelSize.zw;

				float sigma = kernelRadius / 2.0;
				float weightSum = GaussianPdf(0.0, sigma);
				vec4 diffuseSum = texture( mainTexture, uv) * weightSum;
				vec2 delta = direction * invSize * kernelRadius / float(maxRadius);
				vec2 uvOffset = delta;
				for ( float i = 1.0; i <= maxRadius; i += 1.0 ) {
					float x = kernelRadius * float(i) / float(maxRadius);
					float w = GaussianPdf(x, sigma);
					vec4 sample1 = texture( mainTexture, uv + uvOffset);
					vec4 sample2 = texture( mainTexture, uv - uvOffset);
					diffuseSum += ((sample1 + sample2) * w);
					weightSum += (2.0 * w);
					uvOffset += delta;
				}
				FragColor = diffuseSum / weightSum;
			}
		}
	}
}  
