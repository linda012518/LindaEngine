#pragma once

#include <iostream>
#include "IEventHandler.h"

namespace LindaEngine
{
	class LObject : public IEventHandler
	{
	public:
		LObject();
		virtual ~LObject();

		int GetID() const;

		void Bind(int eventCode);
		void Unbind(int eventCode);
		void Dispatch(IEventHandler* sender, int eventCode, Event& eventData);
		virtual void OnEvent(IEventHandler* sender, int eventCode, Event& eventData) { }

	protected:
		static int _id;
		static int _tempID;
		int _selfID;
	};
}
