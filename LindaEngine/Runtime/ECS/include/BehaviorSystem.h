#pragma once

#include <vector>

namespace LindaEngine
{
	class Behavior;

	class BehaviorSystem
	{
	public:
		static void Tick();
		static void OnDeserializeFinish();

		static void Add(Behavior* behavior);
		static void Remove(Behavior* behavior);
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
		static void AddAwake(Behavior* behavior, bool none);
		static void AddOnEnable(Behavior* behavior, bool none);
		static void AddStart(Behavior* behavior, bool none);
		static void AddFixUpdate(Behavior* behavior, bool none);
		static void AddOnTriggerEvent(Behavior* behavior, bool none);
		static void AddOnCollisionEvent(Behavior* behavior, bool none);
		static void AddOnMouseEvent(Behavior* behavior, bool none);
		static void AddUpdate(Behavior* behavior, bool none);
		static void AddLateUpdate(Behavior* behavior, bool none);
		static void AddOnPreCull(Behavior* behavior, bool none);
		static void AddOnPreRender(Behavior* behavior, bool none);
		static void AddOnRenderObject(Behavior* behavior, bool none);
		static void AddOnPostRender(Behavior* behavior, bool none);
		static void AddOnApplicationPause(Behavior* behavior, bool none);
		static void AddOnApplicationQuit(Behavior* behavior, bool none);
		static void AddOnDisable(Behavior* behavior, bool none);
		static void AddOnDestroy(Behavior* behavior, bool none);

	private:
		static void ClearFunctionArray(std::vector<Behavior*> array);

	private:
		static std::vector<Behavior*> _awakes;
		static std::vector<Behavior*> _onEnables;
		static std::vector<Behavior*> _starts;
		static std::vector<Behavior*> _fixUpdates;
		static std::vector<Behavior*> _onTriggerEvents;
		static std::vector<Behavior*> _onCollisionEvents;
		static std::vector<Behavior*> _onMouseEvents;
		static std::vector<Behavior*> _updates;
		static std::vector<Behavior*> _lateUpdates;
		static std::vector<Behavior*> _onPreCulls;
		static std::vector<Behavior*> _onPreRenders;
		static std::vector<Behavior*> _onRenderObjects;
		static std::vector<Behavior*> _onPostRenders;
		static std::vector<Behavior*> _onApplicationPauses;
		static std::vector<Behavior*> _onApplicationQuits;
		static std::vector<Behavior*> _onDisables;
		//static std::vector<Behavior*> _onDestroys;

	};
}
