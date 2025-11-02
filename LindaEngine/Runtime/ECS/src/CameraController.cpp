#include "CameraController.h"
#include "BehaviorImplement.inl"

using namespace LindaEngine;

DYNAMIC_CREATE(CameraController)
IMPLEMENT_BEHAVIOR(CameraController)

CameraController::CameraController(Entity& entity, bool enable) : Behavior(entity, enable)
{
}

CameraController::~CameraController()
{
}

void CameraController::Awake()
{

}

void CameraController::Update()
{

}

