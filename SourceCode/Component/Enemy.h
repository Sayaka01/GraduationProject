#pragma once

#include <DirectXMath.h>

#include "Component.h"

#include "PlayerStates.h"

class BehaviorTree;
class BehaviorData;
class NodeBase;
class ActionBase;

#define OpenConsole

class Enemy : public Component
{
public:
	Enemy() = default;
	~Enemy() override = default;

	//-----< �֐� >-----//

	//������
	void Initialize() override;
	//�X�V
	void Update() override;
	//�`��
	void Draw() override;
	//�I������
	void Finalize()override;
	//ImGui
	void DebugGui() override;

	bool UpdateState() {}

	//-----< Getter, Setter >-----//


private:

	//-----< �֐� >-----//


	//-----< �ϐ� >-----//
	//std::vector<PlayerState::Default*> states;//���
	//PlayerState::Default* currentState = nullptr;

	DirectX::XMFLOAT3 moveVelocity = {};//�ړ��x�N�g��

	BehaviorTree* aiTree{ nullptr };
	BehaviorData* behaviorData{ nullptr };
	NodeBase* activeNode{ nullptr };

};