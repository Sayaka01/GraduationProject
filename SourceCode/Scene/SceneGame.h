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
		DirectX::SimpleMath::Vector2 pos, DirectX::SimpleMath::Vector2 scale = { 1.0f,1.0f }, DirectX::SimpleMath::Vector4 color= { 1.0f,1.0f,1.0f,1.0f });

	// UIの制御
	void PlayerUIUpdate();

	// ゲーム終了時の更新処理
	void ResultUpdate();
	
	// ゲームが終了した場合の処理
	void JudgeResult();

	GameObject* objectManager = nullptr;
	GameObject* player = nullptr;

	GameObject* enemyManager = nullptr;
	std::vector<GameObject*> enemies;

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

	// GameOver時のUI
	GameObject* sprOverBack{ nullptr };
	GameObject* sprOverText{ nullptr };

	// GameClear時のUI
	GameObject* sprClearBack{ nullptr };
	GameObject* sprClearText{ nullptr };

	// GameClear/Over時の操作用
	GameObject* sprTitleText{ nullptr };
	GameObject* sprRetryText{ nullptr };

	enum class GameState
	{
		Ready = -1,//プレイ準備中
		Playing,//プレイ中
		Clear,//ゲームクリア
		Over,//ゲームオーバー
	};

	char gameResult{ (char)GameState::Ready };

	//bool isQuestClear{ false };//クエストの条件をクリアしたかどうか
	bool isFinishGame{ false };//ゲームが終わったかどうか
};