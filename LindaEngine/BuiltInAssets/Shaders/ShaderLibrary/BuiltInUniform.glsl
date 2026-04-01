
#define linda_PI 3.14159265358979323

uniform mat4 linda_LocalToWorld;
uniform mat4 linda_WorldToLocal;

uniform samplerCube linda_PrefilterSpecCube;
uniform samplerCube linda_IrradianceCube;
uniform sampler2D linda_BRDFLUT;

#include "SkinBonesUniform.glsl"
