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

	void SpriteLoad(GameObject** spr, std::string name, const wchar_t* filepath, 
		DirectX::SimpleMath::Vector2 pos, DirectX::SimpleMath::Vector2 scale = { 1.0f,1.0f }, DirectX::SimpleMath::Vector4 color= { 1.0f,1.0f,1.0f,1.0f });

	// UI�̐���
	void PlayerUIUpdate();

	// �Q�[���I�����̍X�V����
	void ResultUpdate();
	
	// �Q�[�����I�������ꍇ�̏���
	void JudgeResult();

	GameObject* objectManager = nullptr;
	GameObject* player = nullptr;

	GameObject* enemyManager = nullptr;
	std::vector<GameObject*> enemies;

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

	// GameOver����UI
	GameObject* sprOverBack{ nullptr };
	GameObject* sprOverText{ nullptr };

	// GameClear����UI
	GameObject* sprClearBack{ nullptr };
	GameObject* sprClearText{ nullptr };

	// GameClear/Over���̑���p
	GameObject* sprTitleText{ nullptr };
	GameObject* sprRetryText{ nullptr };

	enum class GameState
	{
		Ready = -1,//�v���C������
		Playing,//�v���C��
		Clear,//�Q�[���N���A
		Over,//�Q�[���I�[�o�[
	};

	char gameResult{ (char)GameState::Ready };

	//bool isQuestClear{ false };//�N�G�X�g�̏������N���A�������ǂ���
	bool isFinishGame{ false };//�Q�[�����I��������ǂ���
};