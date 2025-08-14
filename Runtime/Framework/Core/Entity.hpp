#pragma once

#include <iostream>

namespace LindaEngine
{
	class Entity
	{
	protected:
		char	_name[64];
		int		_id;

	public:
		Entity();
		virtual ~Entity();

		virtual void setName(const char* name);

		char* getName();

		friend std::ostream& operator<<(std::ostream& out, const Entity& entity)
		{
			out << entity._name << std::endl;
			return out;
		}

	private:
		static int _entityCount;

	};
}
