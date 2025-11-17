#include "Behavior.h"
#include "Entity.h"
#include "BehaviorSystem.h"

using namespace LindaEngine;

Behavior::Behavior(Entity& entity, bool enable) : Component(entity, enable) 
{
	//BehaviorSystem::Add(this);
}

Behavior::~Behavior()
{
	//BehaviorSystem::Remove(this);
}

void Behavior::Awake(){}
void Behavior::OnEnable(){}
void Behavior::Start(){}
void Behavior::FixUpdate(){}
void Behavior::OnTriggerEvent(){}
void Behavior::OnCollisionEvent(){}
void Behavior::OnMouseEvent(){}
void Behavior::Update(){}
void Behavior::LateUpdate(){}
void Behavior::OnPreCull(){}
void Behavior::OnPreRender(){}
void Behavior::OnRenderObject(){}
void Behavior::OnPostRender(){}
void Behavior::OnApplicationPause(){}
void Behavior::OnApplicationQuit(){}
void Behavior::OnDisable(){}
void Behavior::OnDestroy(){}

bool Behavior::CanEnable()
{
	return IsDirty() && IsEnable();
}

bool Behavior::CanDisable()
{
	return IsDirty() && IsEnable() == false && LastEnbale() == true;
}


