#pragma once

#include "Component.h"

#include <glm/glm.hpp>

namespace LindaEngine
{
	enum class LightType
	{
		None, DirectionLight, SpotLight, PointLight
	};

	enum class ShadowType
	{
		None, HardShadows, SoftShadows
	};

	class Light : public Component
	{
	public:
		Light(Entity& entity, bool enable = true);
		virtual ~Light();

		bool Serialize();
		bool Deserialize(YAML::Node& node);

		glm::vec4 GetColor();
		glm::vec3 GetPosition();
		glm::vec3 GetDirection();
		virtual float GetAttenuation() { return 1.0f; }

	protected:
		float _intensity;
		glm::vec4 _color;
		LightType _lightType;
		ShadowType _shadowType;
	};

	class DirectionLight : public Light
	{
	public:
		DECLARE_DYNAMIC_CREATE()
		DirectionLight(Entity& entity, bool enable = true);
		virtual ~DirectionLight();

		bool Serialize();
		bool Deserialize(YAML::Node& node);
	};

	class SpotLight : public Light
	{
	public:
		DECLARE_DYNAMIC_CREATE()
		SpotLight(Entity& entity, bool enable = true);
		virtual ~SpotLight();

		bool Serialize();
		bool Deserialize(YAML::Node& node);

		float GetAttenuation();
		glm::vec4 GetSpotAngles();

	private:
		float _range;
		float _spotAngle;
	};

	class PointLight : public Light
	{
	public:
		DECLARE_DYNAMIC_CREATE()
		PointLight(Entity& entity, bool enable = true);
		virtual ~PointLight();

		bool Serialize();
		bool Deserialize(YAML::Node& node);

		float GetAttenuation();

	private:
		float _range;
	};
}
