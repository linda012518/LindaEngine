#include "Light.h"
#include "Entity.h"
#include "YamlSerializer.h"
#include "YamlCustomType.h"
#include "ComponentImplement.inl"
#include "LightSystem.h"
#include "Transform.h"

using namespace LindaEngine;

DYNAMIC_CREATE(DirectionLight)
DYNAMIC_CREATE(SpotLight)
DYNAMIC_CREATE(PointLight)

Light::Light(Entity& entity, bool enable) : Component(entity, enable)
{
	LightSystem::Add(this);

	_lightType = LightType::None;
	_intensity = 1.0f;
	_color = glm::vec4(1.0f);
	_shadowType = ShadowType::None;
}

Light::~Light()
{
	LightSystem::Remove(this);
}

bool Light::Serialize()
{
	YAML::Emitter& out = *YamlSerializer::out;

	out << YAML::Value << YAML::BeginMap;

	out << YAML::Key << "enable" << YAML::Value << _enable;
	out << YAML::Key << "intensity" << YAML::Value << _intensity;
	out << YAML::Key << "color" << YAML::Value << _color;
	out << YAML::Key << "shadowType" << YAML::Value << static_cast<int>(_shadowType);

	return true;
}

bool Light::Deserialize(YAML::Node& node)
{
	_enable = node["enable"].as<bool>();
	_intensity = node["intensity"].as<float>();
	_color = node["color"].as<glm::vec4>();
	_shadowType = static_cast<ShadowType>(node["shadowType"].as<int>());
	return true;
}

glm::vec4 Light::GetColor()
{
	return _color * _intensity;
}

glm::vec3 Light::GetPosition()
{
	return _transform->GetWorldPosition();
}

glm::vec3 Light::GetDirection()
{
	glm::vec3 forward, up, right;
	_transform->GetWorldDir(forward, up, right);
	return forward;
}

/////////////////////////////////////////////////////////////////////

DirectionLight::DirectionLight(Entity& entity, bool enable) : Light(entity, enable)
{
	_lightType = LightType::DirectionLight;
}

DirectionLight::~DirectionLight()
{
}

bool DirectionLight::Serialize()
{
	Light::Serialize();
	YAML::Emitter& out = *YamlSerializer::out;

	out << YAML::Key << "Name" << YAML::Value << "DirectionLight";

	out << YAML::EndMap;

	return true;
}

bool DirectionLight::Deserialize(YAML::Node& node)
{
	Light::Deserialize(node);
	return true;
}

/////////////////////////////////////////////////////////////////////

SpotLight::SpotLight(Entity& entity, bool enable) : Light(entity, enable)
{
	_lightType = LightType::SpotLight;
	_range = 10.0f;
	_spotAngle = 30.0f;
}

SpotLight::~SpotLight()
{
}

bool SpotLight::Serialize()
{
	Light::Serialize();
	YAML::Emitter& out = *YamlSerializer::out;

	out << YAML::Key << "Name" << YAML::Value << "DirectionLight";
	out << YAML::Key << "range" << YAML::Value << _range;
	out << YAML::Key << "spotAngle" << YAML::Value << _spotAngle;

	out << YAML::EndMap;

	return true;
}

bool SpotLight::Deserialize(YAML::Node& node)
{
	Light::Deserialize(node);
	_range = node["range"].as<float>();
	_spotAngle = node["spotAngle"].as<float>();
	return true;
}

float SpotLight::GetAttenuation()
{
	//return 1.0f / std::max(_range * _range, 0.00001f);
	return 0.0f;
}

glm::vec4 SpotLight::GetSpotAngles()
{
	return glm::vec4();
}

/////////////////////////////////////////////////////////////////////

PointLight::PointLight(Entity& entity, bool enable) : Light(entity, enable)
{
	_lightType = LightType::PointLight;
	_range = 10.0f;
}

PointLight::~PointLight()
{
}

bool PointLight::Serialize()
{
	Light::Serialize();
	YAML::Emitter& out = *YamlSerializer::out;

	out << YAML::Key << "Name" << YAML::Value << "DirectionLight";
	out << YAML::Key << "range" << YAML::Value << _range;

	out << YAML::EndMap;

	return true;
}

bool PointLight::Deserialize(YAML::Node& node)
{
	Light::Deserialize(node);
	_range = node["range"].as<float>();
	return true;
}

float PointLight::GetAttenuation()
{
	//return 1.0f / std::max(_range * _range, 0.00001f);
	return 0.0f;
}

