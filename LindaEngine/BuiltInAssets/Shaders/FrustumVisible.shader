Shader
{
	Pass
	{
		LightMode "Adjunct"
		
		RenderState
		{
			DepthState (depthWrite=false;depthFunc= LEQUAL)
		}

		Vertex
		{
			uniform mat4 linda_FrustumMatrix_I_VP; // 从裁剪空间到世界空间的逆矩阵

			void main()
			{
				// 变换到齐次裁剪空间（即逆变换后的坐标，仍为齐次）
				vec4 clipPos = linda_FrustumMatrix_I_VP * vec4(aPosition, 1.0);
    
				// 透视除法，得到世界空间坐标
				vec3 worldPos = clipPos.xyz / clipPos.w;

				gl_Position = linda_Matrix_VP * vec4(worldPos, 1.0);
			}
		}
		
		Fragment
		{
			void main()
			{
				FragColor = vec4(0.0, 0.8, 0.0, 1.0);
			}
		}
	}
}  
