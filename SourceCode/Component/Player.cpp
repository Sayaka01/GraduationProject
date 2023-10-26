
#include "Stdafx.h"
#include "Player.h"

#include "GameObject/GameObject.h"

#include "Component/Transform.h"
#include "Component/SphereCollider.h"

#include "System/UserFunction.h"

using namespace PlayerState;

void Player::Initialize()
{
	name = "Player";

	//�X�e�[�g�̒ǉ�
	states.emplace_back(new Idle(parent));//�ҋ@
	states.emplace_back(new Run(parent));//����

	//�ҋ@�X�e�[�g��
	ChangeState("Idle");

	//HitEvent�̐ݒ�
	parent->GetComponent<SphereCollider>()->radius = 2.5f;
	parent->GetComponent<SphereCollider>()->SetHitProcessFunc(this, &Component::OnCollisionEnter);
}

void Player::Update()
{
	//�X�e�[�g�̍X�V
	currentState->Update();
	
	//�ʒu�̍X�V
	parent->GetComponent<Transform>()->pos += moveVelocity;

	parent->GetComponent<SphereCollider>()->center = parent->GetComponent<Transform>()->pos;
	parent->GetComponent<SphereCollider>()->center.y += 6.0f;
	
	//�X�e�[�g�ύX������
	std::string nextStateName = currentState->Judge();
	//nextStateName��""����Ȃ���΃X�e�[�g���ύX�����
	ChangeState(nextStateName);
}

void Player::Draw()
{

}

void Player::Finalize()
{
	std::vector<PlayerState::Default*> removes;//���

	for (PlayerState::Default* state : states)
	{
		removes.emplace_back(state);
	}
	for (PlayerState::Default* state : removes)
	{
		std::vector<PlayerState::Default*>::iterator it = std::find(states.begin(), states.end(), state);

		if (it != states.end())
		{
			states.erase(it);
		}
		delete state;
	}

	removes.clear();
	states.clear();
}

void Player::DebugGui()
{
	if (ImGui::TreeNode(name.c_str()))
	{
		ImGui::DragFloat3("moveVelocity", &moveVelocity.x);
		ImGui::TreePop();
	}
}

void Player::OnCollisionEnter(Collider* collider)
{
	parent->GetComponent<Transform>()->pos = parent->GetComponent<SphereCollider>()->center;
	parent->GetComponent<Transform>()->pos.y -= 6.0f;
}

void Player::ChangeState(std::string nextStateName)
{
	if (nextStateName == "")return;

	bool changeState = false;
	for (Default* state : states)
	{
		if (state->GetName() == nextStateName)
		{
			currentState = state;//�X�e�[�g�̕ύX
			currentState->Enter();//�X�e�[�g�J�ڎ��̏���
			changeState = true;
			OutputDebugLog(nextStateName + "�X�e�[�g�ɑJ�ڂ��܂����B\n");
			break;
		}
	}

	if (!changeState)
	{
		OutputDebugLog(nextStateName + "�Ƃ������O�̃X�e�[�g�͑��݂��܂���B\n");
	}
}