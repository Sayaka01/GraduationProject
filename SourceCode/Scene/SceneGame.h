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

	// 2D�摜�̏����ݒ�
	void SpriteInitialze();

	void SpriteLoad(GameObject* spr, std::string name, const wchar_t* filepath, 
		DirectX::SimpleMath::Vector2 pos, DirectX::SimpleMath::Vector2 scale, DirectX::SimpleMath::Vector4 color);

	// UI�̐���
	void PlayerUIUpdate();

	GameObject* objectManager = nullptr;
	GameObject* player = nullptr;
	GameObject* enemy = nullptr;
	GameObject* lightObject = nullptr;
	GameObject* stage = nullptr;
	CameraController* cameraController = nullptr;
	DirectionLight* directionLight = nullptr;

	// �v���C���[��UI�n
	GameObject* spriteManager = nullptr;
	GameObject* sprUiFrame{ nullptr };
	GameObject* sprBoxBar{ nullptr };
	GameObject* sprBoxBarBack{ nullptr };
	GameObject* sprCircleBar{ nullptr };
};