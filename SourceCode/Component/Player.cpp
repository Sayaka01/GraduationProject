
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

	//ステートの追加
	states.emplace_back(new Idle(parent));//待機
	states.emplace_back(new Run(parent));//走り
	states.emplace_back(new Jump(parent));//ジャンプ
	states.emplace_back(new Falling(parent));//落下
	states.emplace_back(new Landing(parent));//着地
	states.emplace_back(new PunchRight(parent));//パンチ（右）
	states.emplace_back(new PunchLeft(parent));//パンチ（左）

	//待機ステートに
	ChangeState("Idle");

	//HitEventの設定
	CapsuleCollider* capsuleCollider = parent->GetComponent<CapsuleCollider>();
	capsuleCollider->radius = 2.0f;
	capsuleCollider->cylinderSize = 5.0f;
	capsuleCollider->SetHitProcessFunc(this, &Component::OnCollisionEnter);

	//骨の位置と名前を登録
	//右手
	parent->GetComponent<ModelRenderer>()->GetModelResource()->AddBonePositionData("rightHand", "mixamorig:RightHandIndex4");
	parent->GetComponent<ModelRenderer>()->GetModelResource()->GetBoneData("rightHand")->isCalc = true;
	//左手
	parent->GetComponent<ModelRenderer>()->GetModelResource()->AddBonePositionData("leftHand", "mixamorig:LeftHandIndex4");
	parent->GetComponent<ModelRenderer>()->GetModelResource()->GetBoneData("leftHand")->isCalc = true;
}

void Player::Update()
{
	//ステートの更新
	currentState->Update();
	
	//ステート変更か判定
	std::string nextStateName = currentState->GetNext();
	//nextStateNameが""じゃなければステートが変更される
	ChangeState(nextStateName);

	//pos.y < 0.0f なら補正
	if (parent->GetComponent<Transform>()->pos.y < 0.0f)parent->GetComponent<Transform>()->pos.y = 0.0f;

	//右手の球
	DirectX::SimpleMath::Vector3 rightHandPos = parent->GetComponent<ModelRenderer>()->GetModelResource()->GetBonePositionFromName("rightHand");
	parent->GetComponent<SphereCollider>("RightHand")->center = rightHandPos;

	//左手の球
	DirectX::SimpleMath::Vector3 leftHandPos = parent->GetComponent<ModelRenderer>()->GetModelResource()->GetBonePositionFromName("leftHand");
	parent->GetComponent<SphereCollider>("LeftHand")->center = leftHandPos;

	//カプセルの位置補正
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
	std::vector<PlayerState::Default*> removes;//状態

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
				currentState->Exit();//変更前ステートの終了処理

			currentState = state;//ステートの変更
			currentState->Enter();//ステート遷移時の処理
			changeState = true;
			OutputDebugLog(nextStateName + "ステートに遷移しました。\n");
			break;
		}
	}

	if (!changeState)
	{
		OutputDebugLog(nextStateName + "という名前のステートは存在しません。\n");
	}
}