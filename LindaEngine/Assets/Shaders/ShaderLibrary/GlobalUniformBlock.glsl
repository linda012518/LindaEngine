
layout (std140) uniform globalUniform
{
	mat4 _linda_Matrix_V;
	mat4 _linda_Matrix_P;
	mat4 _linda_Matrix_VP;

	mat4 _linda_Matrix_I_V;
	mat4 _linda_Matrix_I_P;
	mat4 _linda_Matrix_I_VP;

	vec4 _time; //以场景加载开始的时间 (t/20, t, t*2, t*3)
};
