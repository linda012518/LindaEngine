#pragma once

#include "Camera.h"

namespace LindaEngine
{
	class PerspectiveCamera : public Camera
	{
	public:
		PerspectiveCamera(float fov, float aspectRatio, float nearClip, float farClip);
		virtual ~PerspectiveCamera();

	private:
		float _fov; 
		float _aspectRatio; 
		float _nearClip; 
		float _farClip;
	};
}
