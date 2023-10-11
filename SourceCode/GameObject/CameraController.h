#pragma once

#include "GameObject.h"

class CameraController : public GameObject
{
public:
	CameraController();
	~CameraController() override = default;

	void Initialize() override;
};
