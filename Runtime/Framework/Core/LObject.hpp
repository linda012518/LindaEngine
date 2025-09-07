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

		void Bind(int eventCode);

		void Dispatch(int eventCode, void* userData);

		virtual void OnEvent(void* userData);

	private:
		static int _id;
		int _selfID;
	};
}
