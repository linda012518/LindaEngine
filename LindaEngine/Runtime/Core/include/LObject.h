#pragma once

#include <iostream>

namespace LindaEngine
{
	struct Event;
	class LObject
	{
	public:
		LObject();
		virtual ~LObject();

		int GetID() const;

		void Bind(int eventCode);
		void Dispatch(LObject* sender, int eventCode, Event& eventData);
		virtual void OnEvent(LObject* sender, int eventCode, Event& eventData) { }

	protected:
		static int _id;
		static int _tempID;
		int _selfID;
	};
}
