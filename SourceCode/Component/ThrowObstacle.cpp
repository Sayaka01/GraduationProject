

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

	//フラグの初期化
	throwFlag = false;
}

void ThrowObstacle::Update()
{
	if (throwFlag)
	{
		float elapsedTime = SystemManager::Instance().GetElapsedTime();

		//空気抵抗仮実装
		throwSpeed -= throwSpeed * airResistance * elapsedTime;

		if (throwTarget)
		{
			//自分の位置
			Vector3 pos = parent->GetComponent<Transform>()->pos;
			
			//ターゲットへのベクトル
			throwVec = targetPos - pos;
			throwVec.Normalize();

			//RigidBodyコンポーネントを取得
			RigidBody* rigidBody = parent->GetComponent<RigidBody>();
			//RigidBodyコンポーネントに速度を設定
			rigidBody->AddVelocity(throwVec * throwSpeed);


		}
		else
		{
			//RigidBodyコンポーネントを取得
			RigidBody* rigidBody = parent->GetComponent<RigidBody>();
			//RigidBodyコンポーネントに速度を設定
			rigidBody->AddVelocity(throwVec * throwSpeed);

			//速度が一定を下回ったら止める
			if (throwSpeed < 0.01f)
			{
				throwFlag = false;
				rigidBody->SetVelocity({ 0.0f,0.0f,0.0f });

				//守り属性に変更
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
	//エネミーのコライダーでないならreturn
	if (collider->GetParent()->GetTag() != Tag::Enemy)return;
	//投げられていないならreturn
	if (!throwFlag)return;

	//XZ平面の移動量を0に
	RigidBody* rigidBody = parent->GetComponent<RigidBody>();
	rigidBody->SetVelocity({ 0.0f,rigidBody->GetVelocity().y,0.0f });

}

void ThrowObstacle::GraspFromPlayer()
{
	//コライダーを攻撃属性に変更
	parent->GetComponent<SphereCollider>()->type = Collider::Type::Offense;

	//重力を無効化
	RigidBody* rigidBody = parent->GetComponent<RigidBody>();
	rigidBody->useGravity = false;
	rigidBody->SetVelocity({ 0.0f,0.0f,0.0f });

}

void ThrowObstacle::ThrowFree(Vector3 velocity, float speed)
{
	//変数の設定
	throwVec = velocity;
	throwSpeed = speed;

	//フラグをオン
	throwFlag = true;
	//目標設定は無し
	throwTarget = false;

	//重力を使用
	parent->GetComponent<RigidBody>()->useGravity = true;

}

void ThrowObstacle::ThrowTarget(Vector3 targetPos, float speed)
{
	//変数の設定
	this->targetPos = targetPos;
	throwSpeed = speed;

	//フラグをオン
	throwFlag = true;
	//目標を設定
	throwTarget = true;

	//重力を使用
	parent->GetComponent<RigidBody>()->useGravity = true;

}

#endif