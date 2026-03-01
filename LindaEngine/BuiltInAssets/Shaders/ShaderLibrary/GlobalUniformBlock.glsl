
layout (std140) uniform globalUniform
{
	mat4 linda_Matrix_V;
	mat4 linda_Matrix_P;
	mat4 linda_Matrix_VP;

	mat4 linda_Matrix_I_V;
	mat4 linda_Matrix_I_P;
	mat4 linda_Matrix_I_VP;

	vec4 linda_CameraPosition;

	ivec4 linda_LightCount;//x=dirLightCount y=pointLightCount z=spotLightCount

	vec4 linda_DirLightColors[8];
	vec4 linda_DirLightDirections[8];

	vec4 linda_PointLightColors[64];
	vec4 linda_PointLightPositionAttens[64];

	vec4 linda_SpotLightColors[64];
	vec4 linda_SpotLightPositions[64];
	vec4 linda_SpotLightDirections[64];
	vec4 linda_SpotLightAngles[64];

	vec4 linda_Time; //以场景加载开始的时间 (t/20, t, t*2, t*3)

	vec4 linda_SHAr;
	vec4 linda_SHAg;
	vec4 linda_SHAb;
	vec4 linda_SHBr;
	vec4 linda_SHBg;
	vec4 linda_SHBb;
	vec4 linda_SHC;
};
