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

		void Dispatch(LObject* sender, int eventCode, void* userData);

		virtual void OnEvent(LObject* sender, void* userData);

	protected:
		static int _id;
		int _selfID;
	};
}
