
layout (std140) uniform globalUniform
{
	mat4 _linda_Matrix_V;
	mat4 _linda_Matrix_P;
	mat4 _linda_Matrix_VP;

	mat4 _linda_Matrix_I_V;
	mat4 _linda_Matrix_I_P;
	mat4 _linda_Matrix_I_VP;

	vec4 _cameraPosition;

	ivec4 _lightCount;//x=dirLightCount y=pointLightCount z=spotLightCount

	vec4 _dirLightColors[8];
	vec4 _dirLightDirections[8];

	vec4 _pointLightColors[64];
	vec4 _pointLightPositionAttens[64];

	vec4 _spotLightColors[64];
	vec4 _spotLightPositions[64];
	vec4 _spotLightDirections[64];
	vec4 _spotLightAngles[64];

	vec4 _time; //以场景加载开始的时间 (t/20, t, t*2, t*3)
};
