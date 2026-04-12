#ifndef LINDA_POSTPROCESS_VERTEX_FUNCTION_INCLUDED
#define LINDA_POSTPROCESS_VERTEX_FUNCTION_INCLUDED

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

#endif
