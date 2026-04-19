#pragma once

#include "AutoPtr.h"
#include <vector>

namespace LindaEngine
{
	class Behavior;

	class BehaviorSystem
	{
	public:
		static void Tick();
		static void OnDeserializeFinish();

		static void Add(Weak<Behavior> behavior);
		static void Remove(Weak<Behavior> behavior);
		static void Clear();
		static void Finalize();

		static void DoAwake();
		static void DoOnEnable();
		static void DoStart();

		static void DoFixUpdate();
		static void DoOnTriggerEvent();
		static void DoOnCollisionEvent();

		static void DoOnMouseEvent();

		static void DoUpdate();
		static void DoLateUpdate();

		static void DoOnPreCull();
		static void DoOnPreRender();
		static void DoOnRenderObject();
		static void DoOnPostRender();

		static void DoOnApplicationPause();
		static void DoOnApplicationQuit();

		static void DoOnDisable();
		static void DoOnDestroy();
		/////////////////////////////////////////////////////////////////////////////
		static void AddAwake(Weak<Behavior> behavior, bool none);
		static void AddOnEnable(Weak<Behavior> behavior, bool none);
		static void AddStart(Weak<Behavior> behavior, bool none);
		static void AddFixUpdate(Weak<Behavior> behavior, bool none);
		static void AddOnTriggerEvent(Weak<Behavior> behavior, bool none);
		static void AddOnCollisionEvent(Weak<Behavior> behavior, bool none);
		static void AddOnMouseEvent(Weak<Behavior> behavior, bool none);
		static void AddUpdate(Weak<Behavior> behavior, bool none);
		static void AddLateUpdate(Weak<Behavior> behavior, bool none);
		static void AddOnPreCull(Weak<Behavior> behavior, bool none);
		static void AddOnPreRender(Weak<Behavior> behavior, bool none);
		static void AddOnRenderObject(Weak<Behavior> behavior, bool none);
		static void AddOnPostRender(Weak<Behavior> behavior, bool none);
		static void AddOnApplicationPause(Weak<Behavior> behavior, bool none);
		static void AddOnApplicationQuit(Weak<Behavior> behavior, bool none);
		static void AddOnDisable(Weak<Behavior> behavior, bool none);
		static void AddOnDestroy(Weak<Behavior> behavior, bool none);

	private:
		static void ClearFunctionArray(std::vector<Weak<Behavior>> array);

	private:
		static std::vector<Weak<Behavior>> _awakes;
		static std::vector<Weak<Behavior>> _onEnables;
		static std::vector<Weak<Behavior>> _starts;
		static std::vector<Weak<Behavior>> _fixUpdates;
		static std::vector<Weak<Behavior>> _onTriggerEvents;
		static std::vector<Weak<Behavior>> _onCollisionEvents;
		static std::vector<Weak<Behavior>> _onMouseEvents;
		static std::vector<Weak<Behavior>> _updates;
		static std::vector<Weak<Behavior>> _lateUpdates;
		static std::vector<Weak<Behavior>> _onPreCulls;
		static std::vector<Weak<Behavior>> _onPreRenders;
		static std::vector<Weak<Behavior>> _onRenderObjects;
		static std::vector<Weak<Behavior>> _onPostRenders;
		static std::vector<Weak<Behavior>> _onApplicationPauses;
		static std::vector<Weak<Behavior>> _onApplicationQuits;
		static std::vector<Weak<Behavior>> _onDisables;
		//static std::vector<Behavior*> _onDestroys;

	};
}
