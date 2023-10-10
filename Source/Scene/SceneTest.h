#pragma once

#include "Scene.h"

#include "GameObject/GameObject.h"

class CameraController;

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
	GameObject* gameObject = nullptr;
	CameraController* cameraController = nullptr;
};