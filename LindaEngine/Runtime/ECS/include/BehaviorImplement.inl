#pragma once

#include "YamlSerializer.h"
#include "FunctionCheck.h"
#include "BehaviorSystem.h"
#include "ComponentImplement.inl"

#define IMPLEMENT_BEHAVIOR(className) \
	DYNAMIC_CREATE(className) \
	className::className(LindaEngine::Entity& entity, bool enable) : LindaEngine::Behavior(entity, enable) \
	{ \
	} \
	className::~className() \
	{ \
	} \
	void className::Initialize() \
	{ \
		LindaEngine::BehaviorSystem::AddAwake(DynamicCastWeak(Behavior, GetWeak()), OVERRIDE(LindaEngine::Behavior, className, Awake)); \
		LindaEngine::BehaviorSystem::AddOnEnable(DynamicCastWeak(Behavior, GetWeak()), OVERRIDE(LindaEngine::Behavior, className, OnEnable)); \
		LindaEngine::BehaviorSystem::AddStart(DynamicCastWeak(Behavior, GetWeak()), OVERRIDE(LindaEngine::Behavior, className, Start)); \
		LindaEngine::BehaviorSystem::AddFixUpdate(DynamicCastWeak(Behavior, GetWeak()), OVERRIDE(LindaEngine::Behavior, className, FixUpdate)); \
		LindaEngine::BehaviorSystem::AddOnTriggerEvent(DynamicCastWeak(Behavior, GetWeak()), OVERRIDE(LindaEngine::Behavior, className, OnTriggerEvent)); \
		LindaEngine::BehaviorSystem::AddOnCollisionEvent(DynamicCastWeak(Behavior, GetWeak()), OVERRIDE(LindaEngine::Behavior, className, OnCollisionEvent)); \
		LindaEngine::BehaviorSystem::AddOnMouseEvent(DynamicCastWeak(Behavior, GetWeak()), OVERRIDE(LindaEngine::Behavior, className, OnMouseEvent)); \
		LindaEngine::BehaviorSystem::AddUpdate(DynamicCastWeak(Behavior, GetWeak()), OVERRIDE(LindaEngine::Behavior, className, Update)); \
		LindaEngine::BehaviorSystem::AddLateUpdate(DynamicCastWeak(Behavior, GetWeak()), OVERRIDE(LindaEngine::Behavior, className, LateUpdate)); \
		LindaEngine::BehaviorSystem::AddOnPreCull(DynamicCastWeak(Behavior, GetWeak()), OVERRIDE(LindaEngine::Behavior, className, OnPreCull)); \
		LindaEngine::BehaviorSystem::AddOnPreRender(DynamicCastWeak(Behavior, GetWeak()), OVERRIDE(LindaEngine::Behavior, className, OnPreRender)); \
		LindaEngine::BehaviorSystem::AddOnRenderObject(DynamicCastWeak(Behavior, GetWeak()), OVERRIDE(LindaEngine::Behavior, className, OnRenderObject)); \
		LindaEngine::BehaviorSystem::AddOnPostRender(DynamicCastWeak(Behavior, GetWeak()), OVERRIDE(LindaEngine::Behavior, className, OnPostRender)); \
		LindaEngine::BehaviorSystem::AddOnApplicationPause(DynamicCastWeak(Behavior, GetWeak()), OVERRIDE(LindaEngine::Behavior, className, OnApplicationPause)); \
		LindaEngine::BehaviorSystem::AddOnApplicationQuit(DynamicCastWeak(Behavior, GetWeak()), OVERRIDE(LindaEngine::Behavior, className, OnApplicationQuit)); \
		LindaEngine::BehaviorSystem::AddOnDisable(DynamicCastWeak(Behavior, GetWeak()), OVERRIDE(LindaEngine::Behavior, className, OnDisable)); \
	} \
	bool className::Serialize() \
	{ \
		YAML::Emitter& out = *LindaEngine::YamlSerializer::out; \
		out << YAML::Value << YAML::BeginMap; \
		out << YAML::Key << "enable" << YAML::Value << _enable; \
		out << YAML::Key << "Name" << YAML::Value << #className; \
		out << YAML::EndMap; \
		return true; \
	} \
	bool className::Deserialize(YAML::Node& node) \
	{ \
		_enable = node["enable"].as<bool>(); \
		return true; \
	} 

