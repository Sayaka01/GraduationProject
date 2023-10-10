#pragma once

#include "Scene.h"

#include "GameObject/GameObject.h"

class CameraController;
class DirectionLight;

class SceneTest :public Scene
{
public:
	SceneTest() {}
	~SceneTest()override {}

	void Initialize()override;
	void Finalize()override;
	void Update()override;
	void Draw()override;

private:
	void PlayerInitialize();

	GameObject* player = nullptr;
	GameObject* lightObject = nullptr;
	CameraController* cameraController = nullptr;
	DirectionLight* directionLight = nullptr;
};