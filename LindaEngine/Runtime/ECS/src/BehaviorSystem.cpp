#include "BehaviorSystem.h"
#include "Behavior.h"
#include "Entity.h"

//#define ADDTOLIST(fun, array) if (behavior->fun == false) array.push_back(behavior);

#define REMOVETOLIST(array, behavior) \
auto itr##array = std::find(array.begin(), array.end(), behavior); \
if (itr##array != array.end()) \
	array.erase(itr##array);

#define ADDTOLIST(func, array) \
void BehaviorSystem::Add##func(Behavior* behavior, bool none) { if (none) return; array.push_back(behavior); }

#define IMPLEMENT_LIFECYCLEFUN(fun, array, condition, callFun) \
void BehaviorSystem::fun() \
{ \
	for (auto& b : array) \
	{ \
		if (false == b->condition()) \
			continue; \
		b->callFun(); \
	} \
}



using namespace LindaEngine;

std::vector<Behavior*> BehaviorSystem::_awakes;
std::vector<Behavior*> BehaviorSystem::_onEnables;
std::vector<Behavior*> BehaviorSystem::_starts;
std::vector<Behavior*> BehaviorSystem::_fixUpdates;
std::vector<Behavior*> BehaviorSystem::_onTriggerEvents;
std::vector<Behavior*> BehaviorSystem::_onCollisionEvents;
std::vector<Behavior*> BehaviorSystem::_onMouseEvents;
std::vector<Behavior*> BehaviorSystem::_updates;
std::vector<Behavior*> BehaviorSystem::_lateUpdates;
std::vector<Behavior*> BehaviorSystem::_onPreCulls;
std::vector<Behavior*> BehaviorSystem::_onPreRenders;
std::vector<Behavior*> BehaviorSystem::_onRenderObjects;
std::vector<Behavior*> BehaviorSystem::_onPostRenders;
std::vector<Behavior*> BehaviorSystem::_onApplicationPauses;
std::vector<Behavior*> BehaviorSystem::_onApplicationQuits;
std::vector<Behavior*> BehaviorSystem::_onDisables;
//std::vector<Behavior*> BehaviorSystem::_onDestroys;

void BehaviorSystem::Tick()
{
	for (auto& b : _onEnables)
	{
		b->ClearDirty();
	}
	for (auto& b : _onDisables)
	{
		b->ClearDirty();
	}
}

void BehaviorSystem::OnDeserializeFinish()
{

}

void BehaviorSystem::Add(Behavior* behavior)
{
	behavior->Initialize();
}

void BehaviorSystem::Remove(Behavior* behavior)
{
	REMOVETOLIST(_awakes, behavior);
	REMOVETOLIST(_onEnables, behavior);
	REMOVETOLIST(_starts, behavior);
	REMOVETOLIST(_fixUpdates, behavior);
	REMOVETOLIST(_onTriggerEvents, behavior);
	REMOVETOLIST(_onCollisionEvents, behavior);
	REMOVETOLIST(_onMouseEvents, behavior);
	REMOVETOLIST(_updates, behavior);
	REMOVETOLIST(_lateUpdates, behavior);
	REMOVETOLIST(_onPreCulls, behavior);
	REMOVETOLIST(_onPreRenders, behavior);
	REMOVETOLIST(_onRenderObjects, behavior);
	REMOVETOLIST(_onPostRenders, behavior);
	REMOVETOLIST(_onApplicationPauses, behavior);
	REMOVETOLIST(_onApplicationQuits, behavior);
	REMOVETOLIST(_onDisables, behavior);
	//REMOVETOLIST(_onDestroys, behavior);
}

void BehaviorSystem::Clear()
{
	ClearFunctionArray(_awakes);
	ClearFunctionArray(_onEnables);
	ClearFunctionArray(_starts);
	ClearFunctionArray(_fixUpdates);
	ClearFunctionArray(_onTriggerEvents);
	ClearFunctionArray(_onCollisionEvents);
	ClearFunctionArray(_onMouseEvents);
	ClearFunctionArray(_updates);
	ClearFunctionArray(_lateUpdates);
	ClearFunctionArray(_onPreCulls);
	ClearFunctionArray(_onPreRenders);
	ClearFunctionArray(_onRenderObjects);
	ClearFunctionArray(_onPostRenders);
	ClearFunctionArray(_onApplicationPauses);
	ClearFunctionArray(_onApplicationQuits);
	ClearFunctionArray(_onDisables);
	//ClearFunctionArray(_onDestroys);

	//_awakes.clear();
	//_onEnables.clear();
	//_starts.clear();
	//_fixUpdates.clear();
	//_onTriggerEvents.clear();
	//_onCollisionEvents.clear();
	//_onMouseEvents.clear();
	//_updates.clear();
	//_lateUpdates.clear();
	//_onPreCulls.clear();
	//_onPreRenders.clear();
	//_onRenderObjects.clear();
	//_onPostRenders.clear();
	//_onApplicationPauses.clear();
	//_onApplicationQuits.clear();
	//_onDisables.clear();
	//_onDestroys.clear();
}

void BehaviorSystem::Finalize()
{
	Clear();
}

void BehaviorSystem::DoAwake()
{
	auto itr = _awakes.begin();
	while (itr != _awakes.end()) 
	{
		if (false == (*itr)->IsActive())
			++itr;
		else
		{
			(*itr)->Awake();
			itr = _awakes.erase(itr);
		}
	}
}

void BehaviorSystem::DoOnEnable()
{
	for (auto& b : _onEnables)
	{
		if (false == b->CanEnable())
			continue;
		b->OnEnable();
	}
}

IMPLEMENT_LIFECYCLEFUN(DoFixUpdate, _fixUpdates, IsEnable, FixUpdate);
IMPLEMENT_LIFECYCLEFUN(DoOnTriggerEvent, _onTriggerEvents, IsEnable, OnTriggerEvent);
IMPLEMENT_LIFECYCLEFUN(DoOnCollisionEvent, _onCollisionEvents, IsEnable, OnCollisionEvent);
IMPLEMENT_LIFECYCLEFUN(DoOnMouseEvent, _onMouseEvents, IsEnable, OnMouseEvent);
IMPLEMENT_LIFECYCLEFUN(DoUpdate, _updates, IsEnable, Update);
IMPLEMENT_LIFECYCLEFUN(DoLateUpdate, _fixUpdates, IsEnable, LateUpdate);
IMPLEMENT_LIFECYCLEFUN(DoOnPreCull, _onPreCulls, IsEnable, OnPreCull);
IMPLEMENT_LIFECYCLEFUN(DoOnPreRender, _onPreRenders, IsEnable, OnPreRender);
IMPLEMENT_LIFECYCLEFUN(DoOnRenderObject, _onRenderObjects, IsEnable, OnRenderObject);
IMPLEMENT_LIFECYCLEFUN(DoOnPostRender, _onPostRenders, IsEnable, OnPostRender);
IMPLEMENT_LIFECYCLEFUN(DoOnApplicationPause, _onApplicationPauses, IsEnable, OnApplicationPause);
IMPLEMENT_LIFECYCLEFUN(DoOnApplicationQuit, _onApplicationQuits, IsEnable, OnApplicationQuit);
IMPLEMENT_LIFECYCLEFUN(DoOnDisable, _onDisables, CanDisable, OnDisable);
//IMPLEMENT_LIFECYCLEFUN(DoOnDestroy, _onDestroys, IsEnable, OnDestroy);

void BehaviorSystem::DoStart()
{
	auto itr = _starts.begin();
	while (itr != _starts.end())
	{
		if (false == (*itr)->IsEnable())
			++itr;
		else
		{
			(*itr)->Start();
			itr = _starts.erase(itr);
		}
	}
}

void BehaviorSystem::AddAwake(Behavior* behavior, bool none)
{
	if (none) return;
	_awakes.push_back(behavior);
}

ADDTOLIST(OnEnable, _onEnables);
ADDTOLIST(Start, _starts);
ADDTOLIST(FixUpdate, _fixUpdates);
ADDTOLIST(OnTriggerEvent, _onTriggerEvents);
ADDTOLIST(OnCollisionEvent, _onCollisionEvents);
ADDTOLIST(OnMouseEvent, _onMouseEvents);
ADDTOLIST(Update, _updates);
ADDTOLIST(LateUpdate, _lateUpdates);
ADDTOLIST(OnPreCull, _onPreCulls);
ADDTOLIST(OnPreRender, _onPreRenders);
ADDTOLIST(OnRenderObject, _onRenderObjects);
ADDTOLIST(OnPostRender, _onPostRenders);
ADDTOLIST(OnApplicationPause, _onApplicationPauses);
ADDTOLIST(OnApplicationQuit, _onApplicationQuits);
ADDTOLIST(OnDisable, _onDisables);
//ADDTOLIST(OnDestroy, _onDestroys);

void BehaviorSystem::ClearFunctionArray(std::vector<Behavior*> array)
{
	std::vector<Behavior*> temp;

	for (auto& com : array) {
		if (false == com->GetEntity().GetDontDestory())
			continue;
		temp.push_back(com);
	}

	array.clear();

	for (auto& com : temp) {
		array.push_back(com);
	}
}
