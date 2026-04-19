#pragma once

#include <iostream>
#include "IEventHandler.h"

namespace LindaEngine
{
	class LObject : public std::enable_shared_from_this<LObject>, public IEventHandler
	{
	public:
		LObject();
		virtual ~LObject();

		int GetID() const;
		Ref<LObject> GetShared();
		Weak<LObject> GetWeak();
		void Bind(int eventCode);
		void Unbind(int eventCode);
		void Dispatch(Weak<IEventHandler> sender, int eventCode, Event& eventData);
		virtual void OnEvent(Weak<IEventHandler> sender, int eventCode, Event& eventData) { }
		virtual void OnImguiRender() {}

	protected:
		static int _id;
		static int _tempID;
		int _selfID;
	};
}
