
#include "Stdafx.h"

#include "CameraController.h"

#include "Component/Camera.h"

CameraController::CameraController()
{
	name = "CameraController";
	Initialize();
}

void CameraController::Initialize()
{
	AddComponent(new Camera());
}
