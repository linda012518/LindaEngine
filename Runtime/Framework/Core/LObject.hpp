#pragma once

#include <iostream>
#include "Base.h"
#include "Rtti.hpp"

namespace LindaEngine
{
	class LObject
	{
	public:
		LObject();
		virtual ~LObject();

		int GetID() const;

	private:
		static int _id;
		int _selfID;
	};
}
