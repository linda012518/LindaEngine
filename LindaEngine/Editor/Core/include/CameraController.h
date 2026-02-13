#pragma once

#include "Behavior.h"
#include "glm/glm.hpp"

namespace LindaEngine
{
	class PerspectiveCamera;
	struct MouseEvent;

	class CameraController : public Behavior
	{
	public:
		DECLARE_BEHAVIOR(CameraController)

		void Awake();
		void Update();
		void OnDestroy();

		void OnEvent(IEventHandler* sender, int eventCode, Event& eventData);

	private:
		void ProcessMouseEvent(int eventCode, Event& eventData);
		void ProcessKeyEvent(int eventCode, Event& eventData);
		void ScaleEvent(MouseEvent& event);
		void PanningEvent(MouseEvent& event);
		void RotateEvent(MouseEvent& event);
		void LookRoundEvent(MouseEvent& event);
		void LookAtEntity();

	private:
		PerspectiveCamera* _camera = nullptr;
		float _mouseWheelSpeed = 0.001f;
		float _stanardWheelDelta = 120.0f;

		float _panSpeed = 0.002f;
		bool _isPanning = false;

		glm::vec2 _lastMousePos;
		glm::vec3 _lookRoundTarget;
		float _lookRoundDistance;

		bool _leftHeld = false;
		bool _rightHeld = false;
		bool _ctrlHeld = false;
		float _rotateSpeed = 0.1f;

		Entity* _selectedEntity = nullptr;

	public:
		static bool isLookRound;
	};

}
