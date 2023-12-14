
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

	//-----< アニメーションスピードの設定 >-----//
	{
		animationSpeed[(int)PlayerState::Animation::Idle] = 1.0f;//待機
		animationSpeed[(int)PlayerState::Animation::Walking] = 1.0f;//歩き
		animationSpeed[(int)PlayerState::Animation::Running] = 1.0f;//走り

		animationSpeed[(int)PlayerState::Animation::Jump] = 2.0f;//ジャンプ
		animationSpeed[(int)PlayerState::Animation::Falling] = 1.0f;//落下
		animationSpeed[(int)PlayerState::Animation::Landing] = 2.0f;//着地
		animationSpeed[(int)PlayerState::Animation::JumpFlip] = 1.5f;//2段ジャンプ（空中回避）

		animationSpeed[(int)PlayerState::Animation::Avoid] = 1.5f;//回避

		animationSpeed[(int)PlayerState::Animation::PunchRight] = 2.0f;//右パンチ
		animationSpeed[(int)PlayerState::Animation::PunchLeft] = 2.0f;//左パンチ
		animationSpeed[(int)PlayerState::Animation::Kick] = 1.5f;//キック	
		animationSpeed[(int)PlayerState::Animation::JumpPunch] = 1.5f;//空中攻撃

		animationSpeed[(int)PlayerState::Animation::Damage] = 1.5f;//ダメージ
		animationSpeed[(int)PlayerState::Animation::Death] = 1.5f;//死亡

		animationSpeed[(int)PlayerState::Animation::DangleWire] = 1.0f;//ワイヤーぶら下がり

		animationSpeed[(int)PlayerState::Animation::Thrust] = 1.2f;//さす
		animationSpeed[(int)PlayerState::Animation::Wield] = 1.2f;//振り回す
		animationSpeed[(int)PlayerState::Animation::Throw] = 1.7f;//投げる
	}

	//-----< ステートの追加 >-----//
	states.clear();
	states.emplace_back(new Idle(parent));//待機
	states.emplace_back(new Run(parent));//走り
	states.emplace_back(new Jump(parent));//ジャンプ
	states.emplace_back(new Falling(parent));//落下
	states.emplace_back(new Landing(parent));//着地
	states.emplace_back(new PunchRight(parent));//パンチ（右）
	states.emplace_back(new PunchLeft(parent));//パンチ（左）
	states.emplace_back(new Kick(parent));//パンチ（左）
	states.emplace_back(new Damage(parent));//ダメージ
	states.emplace_back(new Death(parent));//死亡
	states.emplace_back(new AimWire(parent));//ワイヤーでの直線移動
	states.emplace_back(new SwingWire(parent));//ワイヤーでの弧を書いた移動
	states.emplace_back(new WireJump(parent));//ワイヤーでの弧を書いた移動後のジャンプ
	states.emplace_back(new JumpAttack(parent));//空中攻撃（右）
	states.emplace_back(new Avoid(parent));//回避
	states.emplace_back(new AvoidJump(parent));//空中回避
	states.emplace_back(new WieldThrow(parent));//オブジェクトにワイヤーを刺して振り回して投げる

	//待機ステートに
	ChangeState("Idle");

	//HitEventの設定
	CapsuleCollider* capsuleCollider = parent->GetComponent<CapsuleCollider>("BodyCapsule");
	capsuleCollider->radius = 1.5f;
	capsuleCollider->cylinderSize = 5.0f;
	capsuleCollider->SetHitProcessFunc(this, &Component::OnCollisionEnter);

	//骨の位置と名前を登録
	//右手
	parent->GetComponent<ModelRenderer>()->AddBonePositionData("rightHand", "mixamorig:RightHandIndex4");
	parent->GetComponent<ModelRenderer>()->GetBoneData("rightHand")->isCalc = true;
	//左手
	parent->GetComponent<ModelRenderer>()->AddBonePositionData("leftHand", "mixamorig:LeftHandIndex4");
	parent->GetComponent<ModelRenderer>()->GetBoneData("leftHand")->isCalc = true;
	//右足
	parent->GetComponent<ModelRenderer>()->AddBonePositionData("rightAnkle", "mixamorig:RightLeg");
	parent->GetComponent<ModelRenderer>()->GetBoneData("rightAnkle")->isCalc = true;

	//ルートモーション用
	parent->GetComponent<ModelRenderer>()->StoreNodeIndex("IdleHip",(int)PlayerState::Animation::Idle,"mixamorig:Hips");//ボーン:”Robot 4”のノード番号を取得
	parent->GetComponent<ModelRenderer>()->StoreNodeIndex("FallingHip",(int)PlayerState::Animation::Falling,"mixamorig:Hips");//ボーン:”Robot 4”のノード番号を取得


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
	DirectX::SimpleMath::Vector3 rightHandPos = parent->GetComponent<ModelRenderer>()->GetBonePositionFromName("rightHand");
	parent->GetComponent<SphereCollider>("RightHandSphere")->center = rightHandPos;

	//左手の球
	DirectX::SimpleMath::Vector3 leftHandPos = parent->GetComponent<ModelRenderer>()->GetBonePositionFromName("leftHand");
	parent->GetComponent<SphereCollider>("LeftHandSphere")->center = leftHandPos;

	//右足の球
	DirectX::SimpleMath::Vector3 RightAnklePos = parent->GetComponent<ModelRenderer>()->GetBonePositionFromName("rightAnkle");
	parent->GetComponent<SphereCollider>("RightAnkleSphere")->center = RightAnklePos;

	//カプセルの位置補正
	CapsuleCollider* capsuleCollider = parent->GetComponent<CapsuleCollider>("BodyCapsule");
	capsuleCollider->begin = parent->GetComponent<Transform>()->pos;
	capsuleCollider->begin.y += capsuleCollider->radius + capsuleCollider->cylinderSize;
	capsuleCollider->end = parent->GetComponent<Transform>()->pos;
	capsuleCollider->end.y += capsuleCollider->radius;

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

	//位置の補正
	CapsuleCollider* capsuleCollider = parent->GetComponent<CapsuleCollider>("BodyCapsule");
	parent->GetComponent<Transform>()->pos = capsuleCollider->end;
	parent->GetComponent<Transform>()->pos.y -= capsuleCollider->radius;
	
	if (collider->type == Collider::Type::Offense)
	{
		//ダメージを受ける
		if (currentState->GetName() != "Damage" && currentState->GetName() != "Death" && currentState->GetName() != "Avoid" && currentState->GetName() != "AvoidJump" && !isAttack)
		{
			//HPを減らす
			parent->GetComponent<Health>()->SubtructHp(collider->GetParent()->GetComponent<Enemy>()->GetAttackPower());

			if (parent->GetComponent<Health>()->GetIsAlive())
			{
				ChangeState("Damage");//ダメージステートに遷移
				DirectX::XMFLOAT3 knockBackVec = (capsuleCollider->end + (capsuleCollider->cylinderSize * 0.5f)) - collider->center;
				parent->GetComponent<RigidBody>()->SetVelocity({ 0.0f,NormalizeFloat3(knockBackVec).y * jumpSpeed,0.0f });
				knockBackVec.y = 0.0f;
				currentState->SetParameter(NormalizeFloat3(knockBackVec) * knockBackSpeed);
			}
			else
			{
				ChangeState("Death");//死亡ステートに遷移
			}
		}
	}
	else
	{
		if (currentState->GetName() != "JumpAttack")
		{
			//プレイヤーが上にいるなら踏む
			DirectX::XMFLOAT3 vec = capsuleCollider->end - collider->center;
			DirectX::XMFLOAT3 up = { 0.0f,1.0f,0.0f };
			float dot = DotFloat3(vec, up);
			if (cosf(dot) < DirectX::XMConvertToDegrees(stepDegree) && vec.y > 0.0f)
			{
				//踏んだ
				currentState->InitJumpCount();
				ChangeState("Jump");//ジャンプステートに遷移
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
				currentState->Exit();//変更前ステートの終了処理

			currentState = state;//ステートの変更
			OutputDebugLog(nextStateName + "ステートに遷移しました。\n");
			currentState->Enter();//ステート遷移時の処理
			changeState = true;
			break;
		}
	}

	if (!changeState)
	{
		OutputDebugLog(nextStateName + "という名前のステートは存在しません。\n");
	}
}