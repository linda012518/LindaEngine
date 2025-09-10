#include "PerspectiveCamera.h"

using namespace LindaEngine;

PerspectiveCamera::PerspectiveCamera(float fov, float aspectRatio, float nearClip, float farClip) : 
	_fov(fov), _aspectRatio(aspectRatio), _nearClip(nearClip), _farClip(farClip), 
	Camera(glm::perspective(glm::radians(fov), aspectRatio, nearClip, farClip))
{

	std::cout << "			PerspectiveCamera" << _selfID << std::endl;
}

PerspectiveCamera::~PerspectiveCamera()
{
	std::cout << "			~PerspectiveCamera" << _selfID << std::endl;
}
