#pragma once

#include <vector>
#include "LObject.hpp"

namespace LindaEngine
{
	class Entity;

	class Component : public LObject
	{
	public:
		Component();
		virtual ~Component();

		void SetEntity(Ref<Entity>& e);

	private:
		Ref<Entity> _entity;
	};
}
