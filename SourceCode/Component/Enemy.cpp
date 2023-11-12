
#include "Stdafx.h"
#include "Enemy.h"

#include "AI/BehaviorTree.h"
#include "AI/NodeBase.h"
#include "AI/BehaviorData.h"
#include "AI/ActionDerived.h"
#include "AI/JudgmentDerived.h"

//#include "../Sources/Tools/Console.h"
//#include "../Sources/Tools/InlineUtl.h"

#include "Component/Transform.h"
#include "Component/ModelRenderer.h"
#include "Component/RigidBody.h"
#include "Component/SphereCollider.h"

#include "GameObject/GameObject.h"

#include "System/SystemManager.h"
#include "System/UserFunction.h"

#include <SimpleMath.h>


void Enemy::Initialize()
{
#ifdef OpenConsole
	ConsoleFunc::OpenWindow();
#endif

	name = "Enemy";

	// AIツリーの設定
	SetBehaviorTree();

	//"待機アニメーション"を再生
	//parent->GetComponent<ModelRenderer>()->PlayAnimation((int)AnimationName::Idle1, true);
	
	//重力を一旦無視する
	parent->GetComponent<RigidBody>()->SetUseGravity(false);

	//攻撃用に腕の関節位置を取得
	parent->GetComponent<ModelRenderer>()->GetModelResource()->AddBonePositionData("rightHand", "RigRArm2");
	//腰の位置を取得
	parent->GetComponent<ModelRenderer>()->GetModelResource()->AddBonePositionData("waist", "RigMain");
	//取得したい骨の位置の計算をオンにする
	parent->GetComponent<ModelRenderer>()->GetModelResource()->GetBoneData("waist")->isCalc = true;
	parent->GetComponent<SphereCollider>("waist")->radius = 3.0f;

	parent->GetComponent<SphereCollider>("attackRightHand")->SetEnable(false);

	parent->GetComponent<SphereCollider>()->SetHitProcessFunc(this, &Component::OnCollisionEnter);

}

void Enemy::Update()
{

	//ビヘイビアツリー
	if (activeNode == nullptr)
		activeNode = aiTree->ActiveNodeInference(behaviorData);
	else
		activeNode = aiTree->Run(activeNode, behaviorData,SystemManager::Instance().GetElapsedTime());
		//activeNode = aiTree->Run(activeNode, behaviorData,elapsedTime);

	// 腰の位置の取得
	DirectX::SimpleMath::Vector3 waistPos = parent->GetComponent<ModelRenderer>()->GetModelResource()->GetBonePositionFromName("waist");
	DirectX::SimpleMath::Vector3 forward = parent->GetComponent<Transform>()->GetForward();
	waistPos -= forward * 1.5f;


	// sphereColliderの位置を設定
	parent->GetComponent<SphereCollider>("waist")->center = waistPos;

	// 当たりフラグを初期化させてEnterの処理
	hitFlag = false;

}

void Enemy::Finalize()
{
#ifdef OpenConsole
	ConsoleFunc::CloseWiindow();
#endif

	SafeDelete(aiTree);
	SafeDelete(behaviorData);
	//SafeDelete(activeNode);

	//std::vector<PlayerState::Default*> removes;//状態

	//for (PlayerState::Default* state : states)
	//{
	//	removes.emplace_back(state);
	//}
	//for (PlayerState::Default* state : removes)
	//{
	//	std::vector<PlayerState::Default*>::iterator it = std::find(states.begin(), states.end(), state);

	//	if (it != states.end())
	//	{
	//		states.erase(it);
	//	}
	//	delete state;
	//}

	//removes.clear();
	//states.clear();
}

void Enemy::DebugGui()
{
	if (ImGui::TreeNode("AI"))
	{
		std::string stateStr = "";
		std::string parentStateStr = "";
		if (activeNode != nullptr)
		{
			stateStr = activeNode->GetName();
			parentStateStr = activeNode->GetParent()->GetName();
		}
		ImGui::Text("Behavior :%s : %s", parentStateStr.c_str(), stateStr.c_str());
		//ImGui::InputFloat3("right hand position", &pos.x);

		ImGui::SliderFloat("AttackRange", &attackRange, 1,100);
		ImGui::SliderFloat("PursuitRange", &pursuitRange, 1,100);
		ImGui::SliderFloat("WanderRange", &wanderRange, 1,100);
		ImGui::TreePop();
	}
}

void Enemy::SetBehaviorTree()
{
	//ビヘイビアツリー設定
	behaviorData = new BehaviorData();
	aiTree = new BehaviorTree(this);

	aiTree->AddNode("", "Root", 0, BehaviorTree::Rule::Priority, nullptr, nullptr);

	//aiTree->AddNode("Root", "Escape", 1, BehaviorTree::Rule::Sequence, new EscapeJudgment(this), nullptr);
	aiTree->AddNode("Root", "Death", 1, BehaviorTree::Rule::Absolute, new DamageJudgment(this), nullptr);
	aiTree->AddNode("Root", "Battle", 2, BehaviorTree::Rule::Priority, new BattleJudgment(this), nullptr);
	aiTree->AddNode("Root", "Scout", 3, BehaviorTree::Rule::Priority, nullptr, nullptr);

	aiTree->AddNode("Death", "Die", 1, BehaviorTree::Rule::Non, new DeathJudgment(this), new DieAction(this));
	aiTree->AddNode("Death", "Damage", 2, BehaviorTree::Rule::Non, nullptr, new DamageAction(this));

	aiTree->AddNode("Battle", "Attack", 1, BehaviorTree::Rule::Random, new AttackJudgment(this), nullptr);
	aiTree->AddNode("Battle", "Discovery", 2, BehaviorTree::Rule::Random, nullptr, nullptr);

	aiTree->AddNode("Scout", "Wander", 1, BehaviorTree::Rule::Non, new WanderJudgment(this), new WanderAction(this));
	aiTree->AddNode("Scout", "Idle", 2, BehaviorTree::Rule::Non, nullptr, new IdleAction(this));

	aiTree->AddNode("Attack", "Punch", 1, BehaviorTree::Rule::Non, nullptr, new PunchAction(this));
	aiTree->AddNode("Attack", "Skill", 2, BehaviorTree::Rule::Non, nullptr, new SkillAction(this));
	aiTree->AddNode("Attack", "Break", 3, BehaviorTree::Rule::Non, nullptr, new BreakAction(this));

	aiTree->AddNode("Discovery", "Pursuit", 1, BehaviorTree::Rule::Non, nullptr, new PursuitAction(this));
}


void Enemy::ChangeAnimation(AnimationName animeIndex, bool isLoop)
{
	//アニメーションを切り替え	
	parent->GetComponent<ModelRenderer>()->PlayAnimation((int)animeIndex, isLoop);
}

//目的位置へ移動する
void Enemy::MoveToTargetPosition(float elapsedTime)
{
	Transform* ownerTransform = parent->GetComponent<Transform>();

	// プレイヤーと自身のベクトルを計算
	DirectX::SimpleMath::Vector3 vector = targetPosition - ownerTransform->pos;

	// ベクトルの長さ
	float length = vector.Length();
	// 正規化
	vector.Normalize();

	// 速度に追加
	//ownerTransform->velocity +=  vector* length;
	//ownerTransform->pos += vector * 5.0f * elapsedTime;
	DirectX::SimpleMath::Vector3 velocity = vector * moveSpeed;
	parent->GetComponent<RigidBody>()->AddVelocity({ velocity.x ,0.0f,velocity.z });
}



// 姿勢回転
void Enemy::RotateTransform(float elapsedTime)
{
	DirectX::SimpleMath::Vector3 velo = parent->GetComponent<RigidBody>()->GetVelocity();
	velo.y = 0;//ｙ軸の移動は考慮しない

	////速度の距離を取得
	//float length{ 0.0f };
	//length = velo.Length();
	//if (length < FLT_EPSILON)return;

	////正規化
	//velo.Normalize();

	////ワールド行列を取得
	//DirectX::SimpleMath::Matrix world = parent->GetComponent<Transform>()->world;

	////自身の回転値から前方向を求める
	//DirectX::SimpleMath::Vector3 frontVec = world.Forward();
	//frontVec.Normalize();

	////回転角を求める
	//const float dot = (frontVec.x * velo.x) + (frontVec.z * velo.z);

	////回転速度を調整する
	//float rot = acosf(dot);

	////左右判定
	//float cross = (frontVec.x * velo.z) - (frontVec.z * velo.x);

	//float speed = rotateSpeed * elapsedTime;

	//if (speed > rot) speed = rot;
	//if (cross < 0.0f)
	//{
	//	parent->GetComponent<Transform>()->radian.y += speed;
	//}
	//else
	//{
	//	parent->GetComponent<Transform>()->radian.y -= speed;
	//}


	//引数の移動ベクトルがゼロベクトルの場合は回転処理を行わない
	velo.y = 0.0f;
	const float length = LengthFloat3(velo);
	if (length < FLT_EPSILON)return;

	//移動ベクトルを正規化する
	velo = NormalizeFloat3(velo);

	//回転軸を求める
	DirectX::XMFLOAT3 up = { 0.0f, 1.0f, 0.0f };
	DirectX::XMVECTOR Up = DirectX::XMLoadFloat3(&up);

	//playerのワールド行列
	DirectX::XMFLOAT4X4 world = parent->GetComponent<Transform>()->world;

	//自身の回転値から前方向を求める
	DirectX::XMFLOAT3 frontVec = NormalizeFloat3({ world._31,world._32 ,world._33 });
	up = frontVec;
	//回転角を求める
	const float dot = (frontVec.x * velo.x) + (frontVec.z * velo.z);

	//回転速度を調整する
	float rot = acosf(dot);

	//左右判定
	float cross = (frontVec.x * velo.z) - (frontVec.z * velo.x);

	//orientationの取得
	DirectX::XMFLOAT4 orientation = parent->GetComponent<Transform>()->orientation;
	DirectX::XMVECTOR OldOrientation = DirectX::XMLoadFloat4(&orientation);

	DirectX::XMVECTOR Q;
	float speed = rotateSpeed * SystemManager::Instance().GetElapsedTime();
	if (speed > rot) speed = rot;
	if (cross < 0.0f)
	{
		Q = DirectX::XMQuaternionRotationAxis(Up, speed);
	}
	else
	{
		Q = DirectX::XMQuaternionRotationAxis(Up, -speed);
	}
	DirectX::XMVECTOR Orientation = DirectX::XMQuaternionMultiply(OldOrientation, Q);

	Orientation = DirectX::XMQuaternionSlerp(OldOrientation, Orientation, rotateRatio);

	DirectX::XMStoreFloat4(&orientation, Orientation);

	parent->GetComponent<Transform>()->orientation = orientation;

}

void Enemy::OnCollisionEnter(Collider* collider)
{
	if (collider->GetParent()->GetTag() != Tag::Player)return;

	hitFlag = true;

	//SphereCollider* spCollider = parent->GetComponent<SphereCollider>();
	//parent->GetComponent<Transform>()->pos = spCollider->center;
}
