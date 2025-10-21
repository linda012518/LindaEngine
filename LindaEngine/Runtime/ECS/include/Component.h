#pragma once

#include "LObject.h"
#include "ISerializable.h"
#include "Component.inl"

#include <typeinfo>
#include <unordered_map>

namespace LindaEngine
{
	class Entity;

	class Component : public LObject, public ISerializable
	{
	public:
		Component(Entity& entity, bool enable = true);
		virtual ~Component();

		Entity& GetEntity() const;
		bool IsEnable() const;
		void SetEnable(bool enable);

		virtual void Tick() {}
		virtual void Destroy() {}
		virtual void TransformDirty() {}

		virtual bool Serialize() = 0;
		virtual bool Deserialize(YAML::Node& node) = 0;
		virtual std::string GetClassName() const = 0;

	protected:
		bool _enable;
		Entity& _entity;
	};
}
