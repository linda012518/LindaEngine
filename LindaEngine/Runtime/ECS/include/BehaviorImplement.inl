#pragma once

#include "YamlSerializer.h"
#include "FunctionCheck.h"
#include "BehaviorSystem.h"

namespace LindaEngine
{
#define IMPLEMENT_BEHAVIOR(className) \
	void className::Initialize() \
	{ \
		BehaviorSystem::AddAwake(this, OVERRIDE(Behavior, className, Awake)); \
		BehaviorSystem::AddOnEnable(this, OVERRIDE(Behavior, className, OnEnable)); \
		BehaviorSystem::AddStart(this, OVERRIDE(Behavior, className, Start)); \
		BehaviorSystem::AddFixUpdate(this, OVERRIDE(Behavior, className, FixUpdate)); \
		BehaviorSystem::AddOnTriggerEvent(this, OVERRIDE(Behavior, className, OnTriggerEvent)); \
		BehaviorSystem::AddOnCollisionEvent(this, OVERRIDE(Behavior, className, OnCollisionEvent)); \
		BehaviorSystem::AddOnMouseEvent(this, OVERRIDE(Behavior, className, OnMouseEvent)); \
		BehaviorSystem::AddUpdate(this, OVERRIDE(Behavior, className, Update)); \
		BehaviorSystem::AddLateUpdate(this, OVERRIDE(Behavior, className, LateUpdate)); \
		BehaviorSystem::AddOnPreCull(this, OVERRIDE(Behavior, className, OnPreCull)); \
		BehaviorSystem::AddOnPreRender(this, OVERRIDE(Behavior, className, OnPreRender)); \
		BehaviorSystem::AddOnRenderObject(this, OVERRIDE(Behavior, className, OnRenderObject)); \
		BehaviorSystem::AddOnPostRender(this, OVERRIDE(Behavior, className, OnPostRender)); \
		BehaviorSystem::AddOnApplicationPause(this, OVERRIDE(Behavior, className, OnApplicationPause)); \
		BehaviorSystem::AddOnApplicationQuit(this, OVERRIDE(Behavior, className, OnApplicationQuit)); \
		BehaviorSystem::AddOnDisable(this, OVERRIDE(Behavior, className, OnDisable)); \
		BehaviorSystem::AddOnDestroy(this, OVERRIDE(Behavior, className, OnDestroy)); \
	} \
	bool className::Serialize() \
	{ \
		YAML::Emitter& out = *YamlSerializer::out; \
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

}
