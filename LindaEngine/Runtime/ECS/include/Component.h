#pragma once

#include "LObject.h"

namespace LindaEngine
{
	class Entity;

	class Component : public LObject
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

	protected:
		bool _enable;
		bool _dirty;
		Entity& _entity;
	};
}
