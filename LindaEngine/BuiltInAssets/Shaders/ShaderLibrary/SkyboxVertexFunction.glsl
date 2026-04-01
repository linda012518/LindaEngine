
Vertex
{
	out vec3 worldNormal;

	uniform mat4 linda_SkyboxMatrix_V;
	uniform mat4 linda_SkyboxMatrix_P;

	void main()
	{
		worldNormal = aPosition;
		mat4 view = linda_SkyboxMatrix_V;
		view[3].xyz = vec3(0.0);
		view[3].w = 1.0;
		gl_Position = linda_SkyboxMatrix_P * view * vec4(aPosition, 1.0);
		gl_Position = gl_Position.xyww;
	}
}
