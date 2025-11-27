Shader
{
	Pass
	{
		Properties
		{
			uniform sampler2D mainTexture = white;
		}

		Vertex
		{
			out vec2 uv;

			void main()
			{
				// 顺时针顺序：左下 -> 右下 -> 左上
				// VertexID 0: (-1.0, -1.0) 左下
				// VertexID 1: (3.0, -1.0)  右下
				// VertexID 2: (-1.0, 3.0)  左上
				
				// x坐标：VertexID 0和2为-1.0，VertexID 1为3.0
				float x = (gl_VertexID == 1) ? 3.0 : -1.0;
				// y坐标：VertexID 0和1为-1.0，VertexID 2为3.0
				float y = (gl_VertexID == 2) ? 3.0 : -1.0;
				
				gl_Position = vec4(x, y, 0.0, 1.0);
				
				// UV坐标也需要相应调整
				uv.x = (gl_VertexID == 1) ? 2.0 : 0.0;
				uv.y = (gl_VertexID == 2) ? 2.0 : 0.0;

				// uv.x = gl_VertexID <= 1 ? 0.0 : 2.0;
				// uv.y = gl_VertexID == 1 ? 2.0 : 0.0;
				// gl_Position = vec4(gl_VertexID <= 1 ? -1.0 : 3.0, gl_VertexID == 1 ? 3.0 : -1.0, 0.0, 1.0);
			}
		}
		
		Fragment
		{
			out vec4 FragColor;

			in vec2 uv;

			void main()
			{
				FragColor = texture(mainTexture, uv);
			}
		}
	}
}  
