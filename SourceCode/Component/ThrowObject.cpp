

#include "Stdafx.h"

#include "ThrowObstacle.h"

#include "Component/Transform.h"
#include "Component/SphereCollider.h"
#include "Component/RigidBody.h"

#include "GameObject/GameObject.h"

#include "System/SystemManager.h"

using namespace DirectX::SimpleMath;

#if _APPEND

void ThrowObstacle::Initialize()
{
	name = "ThrowObstacle";

	//�t���O�̏�����
	throwFlag = false;
}

void ThrowObstacle::Update()
{
	if (throwFlag)
	{
		float elapsedTime = SystemManager::Instance().GetElapsedTime();

		//��C��R������
		throwSpeed -= throwSpeed * airResistance * elapsedTime;

		if (throwTarget)
		{
			//�����̈ʒu
			Vector3 pos = parent->GetComponent<Transform>()->pos;
			
			//�^�[�Q�b�g�ւ̃x�N�g��
			throwVec = targetPos - pos;
			throwVec.Normalize();

			//RigidBody�R���|�[�l���g���擾
			RigidBody* rigidBody = parent->GetComponent<RigidBody>();
			//RigidBody�R���|�[�l���g�ɑ��x��ݒ�
			rigidBody->AddVelocity(throwVec * throwSpeed);


		}
		else
		{
			//RigidBody�R���|�[�l���g���擾
			RigidBody* rigidBody = parent->GetComponent<RigidBody>();
			//RigidBody�R���|�[�l���g�ɑ��x��ݒ�
			rigidBody->AddVelocity(throwVec * throwSpeed);

			//���x���������������~�߂�
			if (throwSpeed < 0.01f)
			{
				throwFlag = false;
				rigidBody->SetVelocity({ 0.0f,0.0f,0.0f });

				//��葮���ɕύX
				parent->GetComponent<SphereCollider>()->type = Collider::Type::Deffense;
			}
		}
	}

	Transform* t = parent->GetComponent<Transform>();
	if (t->pos.y < floorPos)
	{
		t->pos.y = floorPos;
	}
}

void ThrowObstacle::DebugGui()
{
	if (ImGui::TreeNode(name.c_str()))
	{
		ImGui::SetNextItemWidth(100.0f);
		ImGui::DragFloat("throwSpeed", &throwSpeed);
		ImGui::SetNextItemWidth(100.0f);
		ImGui::DragFloat("airResistance", &airResistance);

		ImGui::TreePop();
	}

}

void ThrowObstacle::OnCollisionEnter(Collider* collider)
{
	//�G�l�~�[�̃R���C�_�[�łȂ��Ȃ�return
	if (collider->GetParent()->GetTag() != Tag::Enemy)return;
	//�������Ă��Ȃ��Ȃ�return
	if (!throwFlag)return;

	//XZ���ʂ̈ړ��ʂ�0��
	RigidBody* rigidBody = parent->GetComponent<RigidBody>();
	rigidBody->SetVelocity({ 0.0f,rigidBody->GetVelocity().y,0.0f });

}

void ThrowObstacle::GraspFromPlayer()
{
	//�R���C�_�[���U�������ɕύX
	parent->GetComponent<SphereCollider>()->type = Collider::Type::Offense;

	//�d�͂𖳌���
	RigidBody* rigidBody = parent->GetComponent<RigidBody>();
	rigidBody->useGravity = false;
	rigidBody->SetVelocity({ 0.0f,0.0f,0.0f });

}

void ThrowObstacle::ThrowFree(Vector3 velocity, float speed)
{
	//�ϐ��̐ݒ�
	throwVec = velocity;
	throwSpeed = speed;

	//�t���O���I��
	throwFlag = true;
	//�ڕW�ݒ�͖���
	throwTarget = false;

	//�d�͂��g�p
	parent->GetComponent<RigidBody>()->useGravity = true;

}

void ThrowObstacle::ThrowTarget(Vector3 targetPos, float speed)
{
	//�ϐ��̐ݒ�
	this->targetPos = targetPos;
	throwSpeed = speed;

	//�t���O���I��
	throwFlag = true;
	//�ڕW��ݒ�
	throwTarget = true;

	//�d�͂��g�p
	parent->GetComponent<RigidBody>()->useGravity = true;

}

#endif