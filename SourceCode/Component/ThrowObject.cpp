#include "Stdafx.h"

#include "ThrowObstacle.h"

#include "Component/Transform.h"
#include "Component/SphereCollider.h"
#include "Component/RigidBody.h"

#include "GameObject/GameObject.h"

void ThrowObstacle::Initialize()
{
	//�t���O�̏�����
	throwFlag = false;
}

void ThrowObstacle::Update()
{
	Transform* t = parent->GetComponent<Transform>();
	if (t->pos.y < floorPos)
	{
		t->pos.y = floorPos;
	}
}

void ThrowObstacle::DebugGui()
{

}

void ThrowObstacle::OnThrowFlag()
{
	//�t���O���I��
	throwFlag = true;

	//�R���C�_�[���U�������ɕύX
	parent->GetComponent<SphereCollider>()->type = Collider::Type::Offense;

	//�d�͂𖳌���
	RigidBody* rigidBody = parent->GetComponent<RigidBody>();
	rigidBody->useGravity = false;
	rigidBody->SetVelocity({ 0.0f,0.0f,0.0f });

}
