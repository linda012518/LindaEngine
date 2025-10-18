#pragma once

#include "LObject.h"
#include "ISerializable.h"

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

		virtual void Serialize() = 0;
		virtual bool Deserialize() = 0;

	protected:
		bool _enable;
		bool _dirty;
		Entity& _entity;
	};
}
