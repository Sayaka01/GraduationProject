
#include "Stdafx.h"
#include "Player.h"

#include "GameObject/GameObject.h"
#include "Component/Enemy.h"

#include "Component/Transform.h"
#include "Component/CapsuleCollider.h"
#include "Component/SphereCollider.h"
#include "Component/ModelRenderer.h"
#include "Component/RigidBody.h"
#include "Component/Health.h"

#include "System/UserFunction.h"

using namespace PlayerState;

void Player::Initialize()
{
	name = "Player";

	//�X�e�[�g�̒ǉ�
	states.clear();
	states.emplace_back(new Idle(parent));//�ҋ@
	states.emplace_back(new Run(parent));//����
	states.emplace_back(new Jump(parent));//�W�����v
	states.emplace_back(new Falling(parent));//����
	states.emplace_back(new Landing(parent));//���n
	states.emplace_back(new PunchRight(parent));//�p���`�i�E�j
	states.emplace_back(new PunchLeft(parent));//�p���`�i���j
	states.emplace_back(new Damage(parent));//�_���[�W
	states.emplace_back(new Death(parent));//���S
	states.emplace_back(new AimWire(parent));//���C���[�ł̒����ړ�
	states.emplace_back(new SwingWire(parent));//���C���[�ł̌ʂ��������ړ�
	states.emplace_back(new WireJump(parent));//���C���[�ł̌ʂ��������ړ���̃W�����v

	//�ҋ@�X�e�[�g��
	ChangeState("Idle");

	//HitEvent�̐ݒ�
	CapsuleCollider* capsuleCollider = parent->GetComponent<CapsuleCollider>("BodyCapsule");
	capsuleCollider->radius = 1.5f;
	capsuleCollider->cylinderSize = 5.0f;
	capsuleCollider->SetHitProcessFunc(this, &Component::OnCollisionEnter);

	//���̈ʒu�Ɩ��O��o�^
	//�E��
	parent->GetComponent<ModelRenderer>()->AddBonePositionData("rightHand", "mixamorig:RightHandIndex4");
	parent->GetComponent<ModelRenderer>()->GetBoneData("rightHand")->isCalc = true;
	//����
	parent->GetComponent<ModelRenderer>()->AddBonePositionData("leftHand", "mixamorig:LeftHandIndex4");
	parent->GetComponent<ModelRenderer>()->GetBoneData("leftHand")->isCalc = true;


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
	DirectX::SimpleMath::Vector3 rightHandPos = parent->GetComponent<ModelRenderer>()->GetBonePositionFromName("rightHand");
	parent->GetComponent<SphereCollider>("RightHandSphere")->center = rightHandPos;

	//����̋�
	DirectX::SimpleMath::Vector3 leftHandPos = parent->GetComponent<ModelRenderer>()->GetBonePositionFromName("leftHand");
	parent->GetComponent<SphereCollider>("LeftHandSphere")->center = leftHandPos;

	//�J�v�Z���̈ʒu�␳
	CapsuleCollider* capsuleCollider = parent->GetComponent<CapsuleCollider>("BodyCapsule");
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
		ImGui::DragFloat("knockBackSpeed", &knockBackSpeed);
		ImGui::DragFloat("wireSpeed", &wireSpeed);
		ImGui::Text(currentState->GetName().c_str());
		ImGui::TreePop();
	}
}

void Player::OnCollisionEnter(Collider* collider)
{
	if (!parent->GetComponent<Health>()->GetIsAlive())return;
	if (collider->GetParent()->GetTag() != Tag::Enemy)return;

	CapsuleCollider* capsuleCollider = parent->GetComponent<CapsuleCollider>("BodyCapsule");
	parent->GetComponent<Transform>()->pos = capsuleCollider->end;
	parent->GetComponent<Transform>()->pos.y -= capsuleCollider->radius;

	if (currentState->GetName() != "Damage" && currentState->GetName() != "Death")
	{
		//HP�����炷
		parent->GetComponent<Health>()->SubtructHp(collider->GetParent()->GetComponent<Enemy>()->GetAttackPower());

		if (parent->GetComponent<Health>()->GetIsAlive())
		{
			ChangeState("Damage");//�_���[�W�X�e�[�g�ɑJ��
			DirectX::XMFLOAT3 knockBackVec = (capsuleCollider->end + (capsuleCollider->cylinderSize * 0.5f)) - collider->center;
			parent->GetComponent<RigidBody>()->AddVelocity({ 0.0f,NormalizeFloat3(knockBackVec).y * knockBackSpeed,0.0f });
			knockBackVec.y = 0.0f;
			currentState->SetParameter(NormalizeFloat3(knockBackVec) * knockBackSpeed);
		}
		else
		{
			ChangeState("Death");//���S�X�e�[�g�ɑJ��
		}
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
			if (currentState)
				currentState->Exit();//�ύX�O�X�e�[�g�̏I������

			currentState = state;//�X�e�[�g�̕ύX
			OutputDebugLog(nextStateName + "�X�e�[�g�ɑJ�ڂ��܂����B\n");
			currentState->Enter();//�X�e�[�g�J�ڎ��̏���
			changeState = true;
			break;
		}
	}

	if (!changeState)
	{
		OutputDebugLog(nextStateName + "�Ƃ������O�̃X�e�[�g�͑��݂��܂���B\n");
	}
}