#pragma once

#include "Component.h"
#include "BoundingBox.h"

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

		void TransformDirty();

		glm::vec4 GetFinalColor();
		glm::vec3 GetPosition();
		glm::vec3 GetDirection();

		virtual void CalculateAABB() { }
		AABBBoundingBox& GetBoundingBox() { return _aabb; }
		//virtual float GetAttenuation() { return 1.0f; }

		float GetIntensity() { return _intensity; }
		glm::vec4& GetColor() { return _color; }
		ShadowType& GetShadowType() { return _shadowType; }
		LightType& GetLightType() { return _lightType; }

		void SetIntensity(float intensity) { _intensity = intensity; }
		void SetColor(glm::vec4 color) { _color = color; }
		void SetShadowType(ShadowType type) { _shadowType = type; }

	protected:
		float _intensity;
		glm::vec4 _color;
		LightType _lightType;
		ShadowType _shadowType;
		AABBBoundingBox _aabb;
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

		//float GetAttenuation();
		//glm::vec4 GetSpotAngles();

		void SetRange(float range) { _range = range; CalculateAABB(); }
		void SetInnerAngle(float angle) { _innerAngle = angle; CalculateAABB(); }
		void SetOuterAngle(float angle) { _outerAngle = angle; CalculateAABB(); }

		float GetRange() { return _range; }
		float GetInnerAngle() { return _innerAngle; }
		float GetOuterAngle() { return _outerAngle; }

		void CalculateAABB();

	private:
		float _range;
		float _innerAngle;
		float _outerAngle;
	};

	class PointLight : public Light
	{
	public:
		DECLARE_DYNAMIC_CREATE()
		PointLight(Entity& entity, bool enable = true);
		virtual ~PointLight();

		bool Serialize();
		bool Deserialize(YAML::Node& node);

		//float GetAttenuation();

		void SetRange(float range) { _range = range; CalculateAABB(); }
		float GetRange() { return _range; }

		void CalculateAABB();

	private:
		float _range;
	};
}
