#include "Stdafx.h"

#include "ThrowObstacle.h"

#include "Component/Transform.h"
#include "Component/SphereCollider.h"
#include "Component/RigidBody.h"

#include "GameObject/GameObject.h"

void ThrowObstacle::Initialize()
{
	//フラグの初期化
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
	//フラグをオン
	throwFlag = true;

	//コライダーを攻撃属性に変更
	parent->GetComponent<SphereCollider>()->type = Collider::Type::Offense;

	//重力を無効化
	RigidBody* rigidBody = parent->GetComponent<RigidBody>();
	rigidBody->useGravity = false;
	rigidBody->SetVelocity({ 0.0f,0.0f,0.0f });

}
