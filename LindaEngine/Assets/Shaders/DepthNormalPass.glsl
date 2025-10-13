Vertex
{
	#version 330 core

	AttributeNames { aPosition, aNormal }
	//layout (location = 0) in vec3 aPos;
	//layout (location = 0) in vec3 aNormal;

	uniform mat4 modelInverse;
	uniform mat4 model;
	uniform mat4 view;
	uniform mat4 projection;

	out vec3 worldNormal;

	void main()
	{
		gl_Position = projection * view * model * vec4(aPosition, 1.0);
		worldNormal = normalize(aNormal * modelInverse);
	}
}
		
Fragment
{
	#version 330 core

	out vec4 FragColor;
	in vec3 worldNormal;

	void main()
	{
		FragColor.a = gl_FragCoord.z;
		FragColor.rgb = normalize(worldNormal) * 0.5 + 0.5;
	}
}
