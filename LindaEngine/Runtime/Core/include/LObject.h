#pragma once

#include <iostream>

namespace LindaEngine
{
	class LObject
	{
	public:
		LObject();
		virtual ~LObject();

		int GetID() const;

	protected:
		static int _id;
		static int _tempID;
		int _selfID;
	};
}
