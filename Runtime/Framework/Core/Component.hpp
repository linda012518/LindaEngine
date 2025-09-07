#pragma once

#include <vector>

#include "LObject.hpp"
#include "Entity.hpp"

namespace LindaEngine
{
	class Component : public LObject
	{
	public:
		Component();
		virtual ~Component();

		void SetEntity(Entity* e);

		virtual void Tick();
		

	protected:
		Entity* _entity;
	};
}
