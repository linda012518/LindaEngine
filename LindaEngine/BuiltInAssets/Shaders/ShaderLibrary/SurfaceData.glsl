#ifndef LINDA_SURFACE_DATA_INCLUDED
#define LINDA_SURFACE_DATA_INCLUDED

struct SurfaceData
{
    vec3  albedo;
    vec3  specular;
    float metallic;
    float smoothness;
    vec3  normalTS;
    vec3  emission;
    float occlusion;
    float alpha;
    float clearCoatMask;
    float clearCoatSmoothness;
};

#endif
