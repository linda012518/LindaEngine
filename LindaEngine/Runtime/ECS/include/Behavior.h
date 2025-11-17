#pragma once

#include "Component.h"
#include "BehaviorDeclare.inl"

namespace LindaEngine
{
	class Behavior : public Component
	{
		friend class BehaviorSystem;
	public:
		Behavior(Entity& entity, bool enable = true);
		virtual ~Behavior();

		virtual void Awake();
		virtual void OnEnable();
		virtual void Start();
		virtual void FixUpdate();
		virtual void OnTriggerEvent();
		virtual void OnCollisionEvent();
		virtual void OnMouseEvent();
		virtual void Update();
		virtual void LateUpdate();
		virtual void OnPreCull();
		virtual void OnPreRender();
		virtual void OnRenderObject();
		virtual void OnPostRender();
		virtual void OnApplicationPause();
		virtual void OnApplicationQuit();
		virtual void OnDisable();
		virtual void OnDestroy();

		virtual void Initialize() = 0;

	private:
		bool CanEnable();
		bool CanDisable();
	};
}
