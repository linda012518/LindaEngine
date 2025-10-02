#pragma once

#include "LObject.h"

namespace LindaEngine
{
	class Entity;

	class Component : public LObject
	{
	public:
		Component(Entity& entity, bool enable = true);
		virtual ~Component();

		Entity& GetEntity() const;
		bool GetEnable() const;
		void SetEnable(bool enable);
		void SetDirty();

		virtual void Tick() {} //检查组件状态是否有改变
		virtual bool OnAwake(); //第一次添加并且entity源活时调用，返回true已经调用过
		virtual void OnEnable() {} //启动时调用
		virtual bool OnStart(); //渲染第一帧前调用，返回true已经调用过
		virtual void Update() {}
		virtual void LateUpdate() {}
		virtual void OnDisable() {}
		virtual void OnDestroy() {}

		virtual void TransformDirty() {}

	protected:
		bool _enable;
		bool _dirty;
		bool _awaked;
		bool _started;
		Entity& _entity;
	};
}
