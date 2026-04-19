#ifndef LINDA_LIGHTING_INCLUDED
#define LINDA_LIGHTING_INCLUDED

#include "BuiltInAssets/Shaders/ShaderLibrary/Light.glsl"
#include "BuiltInAssets/Shaders/ShaderLibrary/BxDF.glsl"

vec3 SpecularGGX( float Roughness, vec3 SpecularColor, vec3 N, vec3 V, vec3 L )
{
	float a2 = Pow4( Roughness );
	vec3 H = normalize(L + V);
	float NoH = clamp(dot(N, H), 0.0, 1.0 );
	float NoV = clamp(abs(dot(N, V)) + 1e-5, 0.0, 1.0 );
	float NoL = clamp(dot(N, L), 0.0, 1.0 );
	float VoH = clamp(dot(V, H), 0.0, 1.0 );//VoH=LoH
	
	// Generalized microfacet specular
	float D = D_GGX( a2, NoH );
	float Vis = Vis_SmithJointApprox( a2, NoV, NoL );
	vec3 F = F_Schlick( SpecularColor, VoH );

	return (D * Vis) * F;
}

vec3 StandardBRDF( vec3 DiffuseColor, vec3 SpecularColor, float Roughness, vec3 N, vec3 V, vec3 L )
{
	vec3 diffuseTerm = Diffuse_Lambert( DiffuseColor );
	vec3 specularTerm = SpecularGGX(Roughness, SpecularColor, N, V, L);

	return diffuseTerm + specularTerm;
}

vec3 StandardShading( vec3 DiffuseColor, vec3 SpecularColor, float Roughness, vec3 L, vec3 V, vec3 N, vec3 lightColor )
{
	vec3 brdf = StandardBRDF( DiffuseColor, SpecularColor, Roughness, N, V, L );
	float ndotl = clamp(dot(N, L), 0.0, 1.0 );

	return brdf * vec3(ndotl) * lightColor * vec3(linda_PI);
}

vec3 StandardBRDF_Env( vec3 DiffuseColor, vec3 SpecularColor, float Roughness, vec3 WorldPos, vec3 N, vec3 V, float ao )
{
	vec3 diffuseAO = AOMultiBounce(DiffuseColor, ao);
	vec3 radianceSH = ShadeSH9(N);
	vec3 indirectDiffuse = radianceSH * DiffuseColor * diffuseAO;

	vec3 R = reflect(-V, N);
	float NoV = clamp(abs(dot(N, V)) + 1e-5, 0.0, 1.0 );
	vec3 specularIBL = SpecularIBL(-R, WorldPos, Roughness, SpecularColor, NoV);
	float specular = GetSpecularOcclusion(NoV, Roughness * Roughness, ao);
	vec3 specularAO = AOMultiBounce(SpecularColor, specular); 
	vec3 indirectSpecular = specularIBL * specularAO;
	return indirectDiffuse + indirectSpecular;
}

#endif
