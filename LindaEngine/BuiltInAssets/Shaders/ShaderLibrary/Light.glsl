#ifndef LINDA_LIGHT_INCLUDED
#define LINDA_LIGHT_INCLUDED

struct Light 
{
	vec3 color;
	vec3 direction;
	float attenuation;
};

Light GetDirectionalLight(int index, vec3 surfacePositionWS)
{
	Light light;
	light.color = linda_DirLightColors[index].rgb;
	light.direction = normalize(-linda_DirLightDirections[index].xyz);
	light.attenuation = 1.0;
	return light; 
}

Light GetPointLight(int index, vec3 surfacePositionWS)
{
	Light light;
	light.color = linda_PointLightColors[index].rgb;

	vec3 position = linda_PointLightPositionAttens[index].xyz;
	vec3 ray = position - surfacePositionWS;
	light.direction = normalize(ray);

	float distanceSqr = max(dot(ray, ray), 0.00001);
	float rangeAttenuation = Pow2(clamp(1.0 - Pow2(distanceSqr * linda_PointLightPositionAttens[index].w), 0.0, 1.0));
	light.attenuation = rangeAttenuation / distanceSqr;

	return light; 
}

Light GetSpotLight(int index, vec3 surfacePositionWS)
{
	Light light;
	light.color = linda_SpotLightColors[index].rgb;

	vec3 position = linda_SpotLightPositions[index].xyz;
	vec3 ray = position - surfacePositionWS;
	light.direction = normalize(ray);

	float distanceSqr = max(dot(ray, ray), 0.00001);
	float rangeAttenuation = Pow2(clamp(1.0 - Pow2(distanceSqr * linda_SpotLightPositions[index].w), 0.0, 1.0));
	
	vec4 spotAngles = linda_SpotLightAngles[index];
	vec3 spotDirection = normalize(-linda_SpotLightDirections[index].xyz);
	float spotAttenuation = Pow2(clamp(dot(spotDirection, light.direction) * spotAngles.x + spotAngles.y, 0.0, 1.0));

	light.attenuation = spotAttenuation * rangeAttenuation / distanceSqr;

	return light; 
}

#endif
