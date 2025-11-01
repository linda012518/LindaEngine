#pragma once

#include "Component.h"

namespace LindaEngine
{
	class Light : public Component
	{
	public:
		Light(Entity& entity, bool enable = true);
		virtual ~Light();

		bool Serialize();
		bool Deserialize(YAML::Node& node);
	};

	class DirectionLight : public Light
	{
	public:
		DECLARE_DYNAMIC_CREATE(DirectionLight)
		DirectionLight(Entity& entity, bool enable = true);
		virtual ~DirectionLight();

		bool Serialize();
		bool Deserialize(YAML::Node& node);
	};

	class SpotLight : public Light
	{
	public:
		DECLARE_DYNAMIC_CREATE(SpotLight)
		SpotLight(Entity& entity, bool enable = true);
		virtual ~SpotLight();

		bool Serialize();
		bool Deserialize(YAML::Node& node);
	};

	class PointLight : public Light
	{
	public:
		DECLARE_DYNAMIC_CREATE(PointLight)
		PointLight(Entity& entity, bool enable = true);
		virtual ~PointLight();

		bool Serialize();
		bool Deserialize(YAML::Node& node);
	};
}
