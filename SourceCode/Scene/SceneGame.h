#pragma once

#include "Scene.h"

#include "GameObject/GameObject.h"

class CameraController;
class DirectionLight;

class SceneGame :public Scene
{
public:
	SceneGame() {}
	~SceneGame()override {}

	void Initialize()override;
	void Finalize()override;
	void Update()override;
	void Draw()override;

private:
	void PlayerInitialize();
	void EnemyInitialize();

	// 2DâÊëúÇÃèâä˙ê›íË
	void SpriteInitialze();

	GameObject* objectManager = nullptr;
	GameObject* player = nullptr;
	GameObject* enemy = nullptr;
	GameObject* lightObject = nullptr;
	GameObject* stage = nullptr;
	CameraController* cameraController = nullptr;
	DirectionLight* directionLight = nullptr;

	// ÉvÉåÉCÉÑÅ[ÇÃUIån
	GameObject* sprUiFrame{ nullptr };
	GameObject* sprBoxBar{ nullptr };
	GameObject* sprCircleBar{ nullptr };
};