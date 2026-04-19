#ifndef LINDA_COMMON_INCLUDED
#define LINDA_COMMON_INCLUDED

vec3 UnpackNormal(vec3 packedNormal, float scale)
{
	vec3 normal;
	normal.xyz = packedNormal.rgb * vec3(2.0) - vec3(1.0);
	normal.xy *= vec2(scale);
	return normal;
}

#endif
