
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

#include <magic_enum.hpp>


using namespace PlayerState;

void Player::Initialize()
{
	name = "Player";

	//-----< �A�j���[�V�����X�s�[�h�̐ݒ� >-----//
	{
		animationSpeed[(int)PlayerState::Animation::Idle] = 1.0f;//�ҋ@
		animationSpeed[(int)PlayerState::Animation::Walking] = 1.0f;//����
		animationSpeed[(int)PlayerState::Animation::Running] = 1.0f;//����

		animationSpeed[(int)PlayerState::Animation::Jump] = 2.0f;//�W�����v
		animationSpeed[(int)PlayerState::Animation::Falling] = 1.0f;//����
		animationSpeed[(int)PlayerState::Animation::Landing] = 2.0f;//���n
		animationSpeed[(int)PlayerState::Animation::JumpFlip] = 1.5f;//2�i�W�����v�i�󒆉���j

		animationSpeed[(int)PlayerState::Animation::Avoid] = 1.5f;//���

		animationSpeed[(int)PlayerState::Animation::PunchRight] = 2.0f;//�E�p���`
		animationSpeed[(int)PlayerState::Animation::PunchLeft] = 2.0f;//���p���`
		animationSpeed[(int)PlayerState::Animation::Kick] = 1.5f;//�L�b�N	
		animationSpeed[(int)PlayerState::Animation::JumpPunch] = 1.5f;//�󒆍U��

		animationSpeed[(int)PlayerState::Animation::Damage] = 1.5f;//�_���[�W
		animationSpeed[(int)PlayerState::Animation::Death] = 1.5f;//���S

		animationSpeed[(int)PlayerState::Animation::DangleWire] = 1.0f;//���C���[�Ԃ牺����

		animationSpeed[(int)PlayerState::Animation::Thrust] = 1.2f;//����
		animationSpeed[(int)PlayerState::Animation::Wield] = 1.2f;//�U���
		animationSpeed[(int)PlayerState::Animation::Throw] = 1.7f;//������
	}

	//-----< �X�e�[�g�̒ǉ� >-----//
	states.clear();
	states.emplace_back(new Idle(parent));//�ҋ@
	states.emplace_back(new Run(parent));//����
	states.emplace_back(new Jump(parent));//�W�����v
	states.emplace_back(new Falling(parent));//����
	states.emplace_back(new Landing(parent));//���n
	states.emplace_back(new PunchRight(parent));//�p���`�i�E�j
	states.emplace_back(new PunchLeft(parent));//�p���`�i���j
	states.emplace_back(new Kick(parent));//�p���`�i���j
	states.emplace_back(new Damage(parent));//�_���[�W
	states.emplace_back(new Death(parent));//���S
	states.emplace_back(new AimWire(parent));//���C���[�ł̒����ړ�
	states.emplace_back(new SwingWire(parent));//���C���[�ł̌ʂ��������ړ�
	states.emplace_back(new WireJump(parent));//���C���[�ł̌ʂ��������ړ���̃W�����v
	states.emplace_back(new JumpAttack(parent));//�󒆍U���i�E�j
	states.emplace_back(new Avoid(parent));//���
	states.emplace_back(new AvoidJump(parent));//�󒆉��
	states.emplace_back(new WieldThrow(parent));//�I�u�W�F�N�g�Ƀ��C���[���h���ĐU��񂵂ē�����

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
	//�E��
	parent->GetComponent<ModelRenderer>()->AddBonePositionData("rightAnkle", "mixamorig:RightLeg");
	parent->GetComponent<ModelRenderer>()->GetBoneData("rightAnkle")->isCalc = true;

	//���[�g���[�V�����p
	parent->GetComponent<ModelRenderer>()->StoreNodeIndex("IdleHip",(int)PlayerState::Animation::Idle,"mixamorig:Hips");//�{�[��:�hRobot 4�h�̃m�[�h�ԍ����擾
	parent->GetComponent<ModelRenderer>()->StoreNodeIndex("FallingHip",(int)PlayerState::Animation::Falling,"mixamorig:Hips");//�{�[��:�hRobot 4�h�̃m�[�h�ԍ����擾


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

	//�E���̋�
	DirectX::SimpleMath::Vector3 RightAnklePos = parent->GetComponent<ModelRenderer>()->GetBonePositionFromName("rightAnkle");
	parent->GetComponent<SphereCollider>("RightAnkleSphere")->center = RightAnklePos;

	//�J�v�Z���̈ʒu�␳
	CapsuleCollider* capsuleCollider = parent->GetComponent<CapsuleCollider>("BodyCapsule");
	capsuleCollider->begin = parent->GetComponent<Transform>()->pos;
	capsuleCollider->begin.y += capsuleCollider->radius + capsuleCollider->cylinderSize;
	capsuleCollider->end = parent->GetComponent<Transform>()->pos;
	capsuleCollider->end.y += capsuleCollider->radius;

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
		ImGui::SetNextItemWidth(100.0f);
		ImGui::DragFloat("runSpeed", &runSpeed);
		ImGui::SetNextItemWidth(100.0f);
		ImGui::DragFloat("jumpSpeed", &jumpSpeed);
		ImGui::SetNextItemWidth(100.0f);
		ImGui::DragFloat("knockBackSpeed", &knockBackSpeed);
		ImGui::SetNextItemWidth(100.0f);
		ImGui::DragFloat("wireSpeed", &wireSpeed);
		ImGui::SetNextItemWidth(100.0f);
		ImGui::SliderInt("attackPhase", &attackPhase, 0, maxAttackPhase);
		ImGui::Text(currentState->GetName().c_str());
		
		if (ImGui::TreeNode("AnimationSpeed"))
		{
			ImGui::BeginChild(ImGui::GetID((void*)0), ImVec2(ImGui::GetWindowSize().x, 100), ImGuiWindowFlags_NoTitleBar);
			for (int i = 0; i < (int)PlayerState::Animation::AnimNum; i++)
			{
				std::string str = magic_enum::enum_name((PlayerState::Animation)i).data();
				ImGui::SetNextItemWidth(100.0f);
				ImGui::DragFloat(str.c_str(), &(animationSpeed[i]));
			}
			ImGui::EndChild();
			ImGui::TreePop();
		}

		if (ImGui::TreeNode("State"))
		{
			ImGui::BeginChild(ImGui::GetID((void*)0), ImVec2(ImGui::GetWindowSize().x, 100), ImGuiWindowFlags_NoTitleBar);
			for (Default* state : states)
			{
				state->DebugGui();
			}
			ImGui::EndChild();
			ImGui::TreePop();
		}

		ImGui::TreePop();
	}
}

void Player::OnCollisionEnter(Collider* collider)
{
	if (!parent->GetComponent<Health>()->GetIsAlive())return;
	if (collider->GetParent()->GetTag() != Tag::Enemy)return;

	//�ʒu�̕␳
	CapsuleCollider* capsuleCollider = parent->GetComponent<CapsuleCollider>("BodyCapsule");
	parent->GetComponent<Transform>()->pos = capsuleCollider->end;
	parent->GetComponent<Transform>()->pos.y -= capsuleCollider->radius;
	
	if (collider->type == Collider::Type::Offense)
	{
		//�_���[�W���󂯂�
		if (currentState->GetName() != "Damage" && currentState->GetName() != "Death" && currentState->GetName() != "Avoid" && currentState->GetName() != "AvoidJump" && !isAttack)
		{
			//HP�����炷
			parent->GetComponent<Health>()->SubtructHp(collider->GetParent()->GetComponent<Enemy>()->GetAttackPower());

			if (parent->GetComponent<Health>()->GetIsAlive())
			{
				ChangeState("Damage");//�_���[�W�X�e�[�g�ɑJ��
				DirectX::XMFLOAT3 knockBackVec = (capsuleCollider->end + (capsuleCollider->cylinderSize * 0.5f)) - collider->center;
				parent->GetComponent<RigidBody>()->SetVelocity({ 0.0f,NormalizeFloat3(knockBackVec).y * jumpSpeed,0.0f });
				knockBackVec.y = 0.0f;
				currentState->SetParameter(NormalizeFloat3(knockBackVec) * knockBackSpeed);
			}
			else
			{
				ChangeState("Death");//���S�X�e�[�g�ɑJ��
			}
		}
	}
	else
	{
		if (currentState->GetName() != "JumpAttack")
		{
			//�v���C���[����ɂ���Ȃ瓥��
			DirectX::XMFLOAT3 vec = capsuleCollider->end - collider->center;
			DirectX::XMFLOAT3 up = { 0.0f,1.0f,0.0f };
			float dot = DotFloat3(vec, up);
			if (cosf(dot) < DirectX::XMConvertToDegrees(stepDegree) && vec.y > 0.0f)
			{
				//����
				currentState->InitJumpCount();
				ChangeState("Jump");//�W�����v�X�e�[�g�ɑJ��
			}
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