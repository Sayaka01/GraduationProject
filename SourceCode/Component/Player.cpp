
#include "Stdafx.h"
#include "Player.h"

#include "GameObject/GameObject.h"

#include "Component/Transform.h"
#include "Component/SphereCollider.h"

#include "System/UserFunction.h"

using namespace PlayerState;

void Player::Initialize()
{
	name = "Player";

	//ステートの追加
	states.emplace_back(new Idle(parent));//待機
	states.emplace_back(new Run(parent));//走り

	//待機ステートに
	ChangeState("Idle");

	//HitEventの設定
	parent->GetComponent<SphereCollider>()->radius = 2.5f;
	parent->GetComponent<SphereCollider>()->SetHitProcessFunc(this, &Component::OnCollisionEnter);
}

void Player::Update()
{
	//ステートの更新
	currentState->Update();
	
	//位置の更新
	parent->GetComponent<Transform>()->pos += moveVelocity;

	parent->GetComponent<SphereCollider>()->center = parent->GetComponent<Transform>()->pos;
	parent->GetComponent<SphereCollider>()->center.y += 6.0f;
	
	//ステート変更か判定
	std::string nextStateName = currentState->Judge();
	//nextStateNameが""じゃなければステートが変更される
	ChangeState(nextStateName);
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
		ImGui::DragFloat3("moveVelocity", &moveVelocity.x);
		ImGui::TreePop();
	}
}

void Player::OnCollisionEnter(Collider* collider)
{
	parent->GetComponent<Transform>()->pos = parent->GetComponent<SphereCollider>()->center;
	parent->GetComponent<Transform>()->pos.y -= 6.0f;
}

void Player::ChangeState(std::string nextStateName)
{
	if (nextStateName == "")return;

	bool changeState = false;
	for (Default* state : states)
	{
		if (state->GetName() == nextStateName)
		{
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