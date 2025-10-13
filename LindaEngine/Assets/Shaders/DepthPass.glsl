Vertex
{
	#version 330 core

	AttributeNames { aPosition }
	//layout (location = 0) in vec3 aPos;

	uniform mat4 model;
	uniform mat4 view;
	uniform mat4 projection;

	void main()
	{
		gl_Position = projection * view * model * vec4(aPosition, 1.0);
	}
}
		
Fragment
{
	#version 330 core

	void main()
	{
		gl_FragDepth = gl_FragCoord.z;
	}
}
