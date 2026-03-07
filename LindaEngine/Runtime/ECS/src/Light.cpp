#include "Light.h"
#include "Entity.h"
#include "YamlSerializer.h"
#include "YamlCustomType.h"
#include "ComponentImplement.inl"
#include "LightSystem.h"
#include "Transform.h"

#include "imgui/imgui.h"

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

void Light::OnImguiRender()
{
	float firstWidth = 100.0f;

	if (ImGui::BeginTable("IntensityTable", 2, ImGuiTableFlags_SizingStretchProp)) {
		ImGui::TableSetupColumn("Label", ImGuiTableColumnFlags_WidthFixed, firstWidth);
		ImGui::TableSetupColumn("Control", ImGuiTableColumnFlags_WidthStretch);
		ImGui::TableNextRow();
		ImGui::TableSetColumnIndex(0);
		ImGui::Text("Intensity");
		ImGui::TableSetColumnIndex(1);
		ImGui::SetNextItemWidth(-FLT_MIN);
		ImGui::DragFloat("##Intensity", &_intensity, 0.01f);
		ImGui::EndTable();
	}

	if (ImGui::BeginTable("ColorTable", 2, ImGuiTableFlags_SizingStretchProp)) {
		ImGui::TableSetupColumn("Label", ImGuiTableColumnFlags_WidthFixed, firstWidth);
		ImGui::TableSetupColumn("Control", ImGuiTableColumnFlags_WidthStretch);
		ImGui::TableNextRow();
		ImGui::TableSetColumnIndex(0);
		ImGui::Text("Color");
		ImGui::TableSetColumnIndex(1);
		ImGui::SetNextItemWidth(-FLT_MIN);
		ImGui::ColorEdit4("##Color", (float*)&_color);
		ImGui::EndTable();
	}

	static bool show_popup = false;
	if (ImGui::Button("Shadow Type", ImVec2(-1, 0))) {
		show_popup = true;
	}
	if (show_popup)
	{
		ImGui::SetNextWindowPos(ImVec2(ImGui::GetItemRectMin().x, ImGui::GetItemRectMax().y));

		ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.8f, 0.8f, 0.85f, 0.95f));    // 背景色：深蓝灰
		ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.3f, 0.4f, 0.8f, 1.0f));        // 边框色：蓝色
		ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.2f, 0.2f, 0.2f, 1.0f));          // 字体色：浅灰

		ImGui::Begin("##Dropdown", &show_popup,
			ImGuiWindowFlags_NoTitleBar |
			ImGuiWindowFlags_NoResize |
			ImGuiWindowFlags_NoMove |
			ImGuiWindowFlags_AlwaysAutoResize);

		// 检测点击外部区域
		bool popup_hovered = ImGui::IsWindowHovered(ImGuiHoveredFlags_AllowWhenBlockedByPopup |
			ImGuiHoveredFlags_AllowWhenBlockedByActiveItem);
		bool button_hovered = ImGui::IsItemHovered();

		// 如果没有悬停在弹出框或按钮上，且鼠标被点击，则关闭弹出框
		if (ImGui::IsMouseClicked(0) && !popup_hovered && !button_hovered) {
			show_popup = false;
		}

		if (ImGui::IsKeyPressed(ImGuiKey_Escape)) {
			show_popup = false;
		}

		ImGui::PushStyleColor(ImGuiCol_Header, ImVec4(0.2f, 0.3f, 0.6f, 0.8f));        // 选中项背景
		ImGui::PushStyleColor(ImGuiCol_HeaderHovered, ImVec4(0.3f, 0.4f, 0.7f, 0.8f)); // 悬停背景
		ImGui::PushStyleColor(ImGuiCol_HeaderActive, ImVec4(0.4f, 0.5f, 0.8f, 1.0f));  // 点击背景

		static std::vector<std::string> names = { "None", "HardShadows", "SoftShadows" };

		for (int i = 0; i < (int)names.size(); i++)
		{
			if (ImGui::Selectable(names[i].c_str()))
			{
				show_popup = false;
				_shadowType = GetShadowTypeByString(names[i]);
			}
		}

		ImGui::PopStyleColor(3);
		ImGui::End();
		ImGui::PopStyleColor(3);

	}

}

void Light::TransformDirty()
{
	CalculateAABB();
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

	float firstWidth = 100.0f;

	if (ImGui::BeginTable("RangeTable", 2, ImGuiTableFlags_SizingStretchProp)) {
		ImGui::TableSetupColumn("Label", ImGuiTableColumnFlags_WidthFixed, firstWidth);
		ImGui::TableSetupColumn("Control", ImGuiTableColumnFlags_WidthStretch);
		ImGui::TableNextRow();
		ImGui::TableSetColumnIndex(0);
		ImGui::Text("Range");
		ImGui::TableSetColumnIndex(1);
		ImGui::SetNextItemWidth(-FLT_MIN);
		ImGui::DragFloat("##Range", &_range, 0.01f);
		ImGui::EndTable();
	}

	if (ImGui::BeginTable("InnerAngleTable", 2, ImGuiTableFlags_SizingStretchProp)) {
		ImGui::TableSetupColumn("Label", ImGuiTableColumnFlags_WidthFixed, firstWidth);
		ImGui::TableSetupColumn("Control", ImGuiTableColumnFlags_WidthStretch);
		ImGui::TableNextRow();
		ImGui::TableSetColumnIndex(0);
		ImGui::Text("InnerAngle");
		ImGui::TableSetColumnIndex(1);
		ImGui::SetNextItemWidth(-FLT_MIN);
		ImGui::DragFloat("##InnerAngle", &_innerAngle, 0.01f);
		ImGui::EndTable();
	}

	if (ImGui::BeginTable("OuterAngleTable", 2, ImGuiTableFlags_SizingStretchProp)) {
		ImGui::TableSetupColumn("Label", ImGuiTableColumnFlags_WidthFixed, firstWidth);
		ImGui::TableSetupColumn("Control", ImGuiTableColumnFlags_WidthStretch);
		ImGui::TableNextRow();
		ImGui::TableSetColumnIndex(0);
		ImGui::Text("OuterAngle");
		ImGui::TableSetColumnIndex(1);
		ImGui::SetNextItemWidth(-FLT_MIN);
		ImGui::DragFloat("##OuterAngle", &_outerAngle, 0.01f);
		ImGui::EndTable();
	}

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

	float firstWidth = 100.0f;

	if (ImGui::BeginTable("RangeTable", 2, ImGuiTableFlags_SizingStretchProp)) {
		ImGui::TableSetupColumn("Label", ImGuiTableColumnFlags_WidthFixed, firstWidth);
		ImGui::TableSetupColumn("Control", ImGuiTableColumnFlags_WidthStretch);
		ImGui::TableNextRow();
		ImGui::TableSetColumnIndex(0);
		ImGui::Text("Range");
		ImGui::TableSetColumnIndex(1);
		ImGui::SetNextItemWidth(-FLT_MIN);
		ImGui::DragFloat("##Range", &_range, 0.01f);
		ImGui::EndTable();
	}

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

