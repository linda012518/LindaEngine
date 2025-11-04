#include "Light.h"
#include "Entity.h"
#include "YamlSerializer.h"
#include "ComponentImplement.inl"

using namespace LindaEngine;

DYNAMIC_CREATE(DirectionLight)
DYNAMIC_CREATE(SpotLight)
DYNAMIC_CREATE(PointLight)

Light::Light(Entity& entity, bool enable) : Component(entity, enable)
{
}

Light::~Light()
{
}

bool Light::Serialize()
{
	return true;
}

bool Light::Deserialize(YAML::Node& node)
{
	return true;
}

/////////////////////////////////////////////////////////////////////

DirectionLight::DirectionLight(Entity& entity, bool enable) : Light(entity, enable)
{
}

DirectionLight::~DirectionLight()
{
}

bool DirectionLight::Serialize()
{
	return true;
}

bool DirectionLight::Deserialize(YAML::Node& node)
{
	return true;
}

/////////////////////////////////////////////////////////////////////

SpotLight::SpotLight(Entity& entity, bool enable) : Light(entity, enable)
{
}

SpotLight::~SpotLight()
{
}

bool SpotLight::Serialize()
{
	return true;
}

bool SpotLight::Deserialize(YAML::Node& node)
{
	return true;
}

/////////////////////////////////////////////////////////////////////

PointLight::PointLight(Entity& entity, bool enable) : Light(entity, enable)
{
}

PointLight::~PointLight()
{
}

bool PointLight::Serialize()
{
	return true;
}

bool PointLight::Deserialize(YAML::Node& node)
{
	return true;
}
