
#include "Stdafx.h"
#include "Player.h"

#include "GameObject/GameObject.h"

#include "Component/Transform.h"
#include "Component/CapsuleCollider.h"
#include "Component/SphereCollider.h"
#include "Component/ModelRenderer.h"

#include "System/UserFunction.h"

using namespace PlayerState;

void Player::Initialize()
{
	name = "Player";

	//�X�e�[�g�̒ǉ�
	states.emplace_back(new Idle(parent));//�ҋ@
	states.emplace_back(new Run(parent));//����
	states.emplace_back(new Jump(parent));//�W�����v
	states.emplace_back(new Falling(parent));//����
	states.emplace_back(new Landing(parent));//���n
	states.emplace_back(new PunchRight(parent));//�p���`�i�E�j
	states.emplace_back(new PunchLeft(parent));//�p���`�i���j

	//�ҋ@�X�e�[�g��
	ChangeState("Idle");

	//HitEvent�̐ݒ�
	CapsuleCollider* capsuleCollider = parent->GetComponent<CapsuleCollider>();
	capsuleCollider->radius = 2.0f;
	capsuleCollider->cylinderSize = 5.0f;
	capsuleCollider->SetHitProcessFunc(this, &Component::OnCollisionEnter);

	//���̈ʒu�Ɩ��O��o�^
	//�E��
	parent->GetComponent<ModelRenderer>()->GetModelResource()->AddBonePositionData("rightHand", "mixamorig:RightHandIndex4");
	parent->GetComponent<ModelRenderer>()->GetModelResource()->GetBoneData("rightHand")->isCalc = true;
	//����
	parent->GetComponent<ModelRenderer>()->GetModelResource()->AddBonePositionData("leftHand", "mixamorig:LeftHandIndex4");
	parent->GetComponent<ModelRenderer>()->GetModelResource()->GetBoneData("leftHand")->isCalc = true;
}

void Player::Update()
{
	//�X�e�[�g�̍X�V
	currentState->Update();
	
	//�X�e�[�g�ύX������
	std::string nextStateName = currentState->GetNext();
	//nextStateName��""����Ȃ���΃X�e�[�g���ύX�����
	ChangeState(nextStateName);

	//pos.y < 0.0f �Ȃ�␳
	if (parent->GetComponent<Transform>()->pos.y < 0.0f)parent->GetComponent<Transform>()->pos.y = 0.0f;

	//�E��̋�
	DirectX::SimpleMath::Vector3 rightHandPos = parent->GetComponent<ModelRenderer>()->GetModelResource()->GetBonePositionFromName("rightHand");
	parent->GetComponent<SphereCollider>("RightHand")->center = rightHandPos;

	//����̋�
	DirectX::SimpleMath::Vector3 leftHandPos = parent->GetComponent<ModelRenderer>()->GetModelResource()->GetBonePositionFromName("leftHand");
	parent->GetComponent<SphereCollider>("LeftHand")->center = leftHandPos;

	//�J�v�Z���̈ʒu�␳
	CapsuleCollider* capsuleCollider = parent->GetComponent<CapsuleCollider>();
	capsuleCollider->begin = parent->GetComponent<Transform>()->pos;
	capsuleCollider->begin.y += capsuleCollider->radius + capsuleCollider->cylinderSize;
	capsuleCollider->end = parent->GetComponent<Transform>()->pos;
	capsuleCollider->end.y += capsuleCollider->radius;

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
		ImGui::DragFloat("runSpeed", &runSpeed);
		ImGui::DragFloat("jumpSpeed", &jumpSpeed);
		ImGui::TreePop();
	}
}

void Player::OnCollisionEnter(Collider* collider)
{
	CapsuleCollider* capsuleCollider = parent->GetComponent<CapsuleCollider>();
	parent->GetComponent<Transform>()->pos = capsuleCollider->end;
	parent->GetComponent<Transform>()->pos.y -= capsuleCollider->radius;
}

void Player::ChangeState(std::string nextStateName)
{
	if (nextStateName == "")return;

	bool changeState = false;
	for (Default* state : states)
	{
		if (state->GetName() == nextStateName)
		{
			if (currentState)
				currentState->Exit();//�ύX�O�X�e�[�g�̏I������

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