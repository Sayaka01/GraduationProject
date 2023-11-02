#pragma once

#include "Scene.h"

#include "GameObject/GameObject.h"
#include <SimpleMath.h>

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

	// 2D画像の初期設定
	void SpriteInitialze();

	void SpriteLoad(GameObject** spr, std::string name, const wchar_t* filepath, 
		DirectX::SimpleMath::Vector2 pos, DirectX::SimpleMath::Vector2 scale, DirectX::SimpleMath::Vector4 color);

	// UIの制御
	void PlayerUIUpdate();

	GameObject* objectManager = nullptr;
	GameObject* player = nullptr;
	GameObject* enemy = nullptr;
	GameObject* lightObject = nullptr;
	GameObject* stage = nullptr;
	CameraController* cameraController = nullptr;
	DirectionLight* directionLight = nullptr;

	// プレイヤーのUI系
	GameObject* spriteManager = nullptr;
	GameObject* sprUiFrame{ nullptr };
	GameObject* sprBoxBar{ nullptr };
	GameObject* sprBoxBarBack{ nullptr };
	GameObject* sprCircleBar{ nullptr };
};