
#include "Stdafx.h"
#include "Player.h"

#include "System/UserFunction.h"

using namespace PlayerState;

void Player::Initialize()
{
	name = "Player";

	//�X�e�[�g�̒ǉ�
	states.emplace_back(new Idle());//�ҋ@
	states.emplace_back(new Run());//����

	//�ҋ@�X�e�[�g��
	ChangeState("Idle");
}

void Player::Update()
{
	currentState->Update();//�X�e�[�g�̍X�V
	std::string nextStateName = currentState->Judge();//�X�e�[�g�ύX������
	ChangeState(nextStateName);//nextStateName��""����Ȃ���΃X�e�[�g���ύX�����
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

		ImGui::TreePop();
	}
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
			OutputDebugLog(nextStateName + "�X�e�[�g�ɑJ�ڂ��܂����B");
			break;
		}
	}

	if (!changeState)
	{
		OutputDebugLog(nextStateName + "�Ƃ������O�̃X�e�[�g�͑��݂��܂���B");
	}
}