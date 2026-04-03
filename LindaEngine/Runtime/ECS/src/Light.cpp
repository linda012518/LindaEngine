#include "Light.h"
#include "Entity.h"
#include "YamlSerializer.h"
#include "YamlCustomType.h"
#include "ComponentImplement.inl"
#include "LightSystem.h"
#include "Transform.h"
#include "GUILayoutEditor.h"

#include "imgui/imgui.h"

using namespace LindaEngine;
using namespace LindaEditor;

DYNAMIC_CREATE(DirectionLight)
DYNAMIC_CREATE(SpotLight)
DYNAMIC_CREATE(PointLight)

Light* Light::mainLight = nullptr;
bool Light::mainLightDirty = false;
float Light::mainAngleX = 0.0f;

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

void Light::OnImguiRender()
{
	float firstWidth = GUILayoutEditor::ImGuiLabelColumnWidth({ "Intensity", "Shadow Type", "OuterAngle", "InnerAngle"});

	GUILayoutEditor::DragFloat("Intensity", &_intensity, [&]() {
		if (_intensity < 0.0f)
			_intensity = 0.0f;
		else if (_intensity > 10000.0f)
			_intensity = 10000.0f;
		}, 0.01f, 0.0f, 10000.0f, firstWidth);

	GUILayoutEditor::ColorEdit4("Color", (float*)&_color, nullptr, firstWidth);

	static std::vector<std::string> names = { "None", "HardShadows", "SoftShadows" };
	GUILayoutEditor::Dropdown("Shadow Type", (int)_shadowType, names, [&](int index) {
		_shadowType = GetShadowTypeByString(names[index]);
		});
}

void Light::TransformDirty()
{
	CalculateAABB();
	if (this != mainLight)
		return;

	float angle = _transform->GetLocalEulerAngles().x;
	if (glm::abs(angle - mainAngleX) > 30.0f)
	{
		mainLightDirty = true;
		mainAngleX = angle;
	}
}

glm::vec4 Light::GetFinalColor()
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

ShadowType Light::GetShadowTypeByString(std::string str)
{
	if (str == "None") return ShadowType::None;
	else if (str == "HardShadows") return ShadowType::HardShadows;
	else if (str == "SoftShadows") return ShadowType::SoftShadows;
	return ShadowType::None;
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
	_innerAngle = 12.5f;
	_outerAngle = 17.5f;

	CalculateAABB();
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
	out << YAML::Key << "innerAngle" << YAML::Value << _innerAngle;
	out << YAML::Key << "outerAngle" << YAML::Value << _outerAngle;

	out << YAML::EndMap;

	return true;
}

bool SpotLight::Deserialize(YAML::Node& node)
{
	Light::Deserialize(node);
	_range = node["range"].as<float>();
	_innerAngle = node["innerAngle"].as<float>();
	_outerAngle = node["outerAngle"].as<float>();
	return true;
}

void SpotLight::OnImguiRender()
{
	Light::OnImguiRender();

	float firstWidth = GUILayoutEditor::ImGuiLabelColumnWidth({ "Intensity", "Shadow Type", "OuterAngle", "InnerAngle" });

	GUILayoutEditor::DragFloat("Range", &_range, [&]() {
		if (_range < 0.1f)
			_range = 0.1f;
		else if (_range > 10000.0f)
			_range = 10000.0f;
		SetRange(_range);
		}, 0.01f, 0.1f, 10000.0f, firstWidth);

	GUILayoutEditor::DragFloat("InnerAngle", &_innerAngle, [&]() {
		if (_innerAngle < 0.1f)
			_innerAngle = 0.1f;
		else if (_innerAngle > _outerAngle - 0.1f)
			_innerAngle = _outerAngle - 0.1f;
		SetInnerAngle(_innerAngle);
		}, 0.01f, 0.1f, _outerAngle - 0.1f, firstWidth);

	GUILayoutEditor::DragFloat("OuterAngle", &_outerAngle, [&]() {
		if (_outerAngle < _innerAngle + 0.1f)
			_outerAngle = _innerAngle + 0.1f;
		else if (_outerAngle > 10000.0f)
			_outerAngle = 10000.0f;
		SetOuterAngle(_outerAngle);
		}, 0.01f, _innerAngle + 0.1f, 10000.0f, firstWidth);
}

void SpotLight::SetRange(float range)
{
	_range = range; 
	CalculateAABB();
}

void SpotLight::SetInnerAngle(float angle)
{
	if (angle > 168.0f)
		angle = 168.0f;
	if (angle > _outerAngle)
		_innerAngle = _outerAngle;
	else
		_innerAngle = angle;
	CalculateAABB();
}

void SpotLight::SetOuterAngle(float angle)
{
	if (angle > 168.0f)
		angle = 168.0f;
	if (angle < _innerAngle)
		_outerAngle = _innerAngle;
	else
		_outerAngle = angle;
	CalculateAABB(); 
}

void SpotLight::CalculateAABB()
{
	float radius = glm::tan(glm::radians(_outerAngle * 0.5f)) * _range;
	glm::vec3 min = glm::vec3(-radius, -radius, 0.0f);
	glm::vec3 max = glm::vec3(radius, radius, _range);
	glm::mat4 transform = glm::mat4_cast(_transform->GetWorldRotation());
	transform = glm::translate(transform, _transform->GetWorldPosition());
	_aabb = AABBBoundingBox::WorldSpaceAABB(transform, min, max);
}

//float SpotLight::GetAttenuation()
//{
//	return 1.0f / glm::max(_range * _range, 0.00001f);
//}
//
//glm::vec4 SpotLight::GetSpotAngles()
//{
//	float innerCos = glm::cos(glm::radians(0.5f * _innerAngle));
//	float outerCos = glm::cos(glm::radians(0.5f * _outerAngle));
//	float angleRangeInv = 1.0f / glm::max(innerCos - outerCos, 0.001f);
//	return glm::vec4(angleRangeInv, -outerCos * angleRangeInv, 0.0f, 0.0f);
//}

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

void PointLight::OnImguiRender()
{
	Light::OnImguiRender();

	float firstWidth = GUILayoutEditor::ImGuiLabelColumnWidth({ "Intensity", "Shadow Type", "OuterAngle", "InnerAngle" });

	GUILayoutEditor::DragFloat("Range", &_range, [&]() {
		if (_range < 0.1f)
			_range = 0.1f;
		else if (_range > 10000.0f)
			_range = 10000.0f;
		SetRange(_range);
		}, 0.01f, 0.1f, 10000.0f, firstWidth);
}

void PointLight::CalculateAABB()
{
	glm::vec3 min = glm::vec3(-_range);
	glm::vec3 max = glm::vec3(_range);
	glm::mat4 transform = glm::translate(glm::mat4(1.0f), _transform->GetWorldPosition());
	_aabb = AABBBoundingBox::WorldSpaceAABB(transform, min, max);
}


//float PointLight::GetAttenuation()
//{
//	return 1.0f / glm::max(_range * _range, 0.00001f);
//}

