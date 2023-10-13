
#include "Stdafx.h"
#include "Enemy.h"

#include "System/UserFunction.h"

using namespace PlayerState;

void Enemy::Initialize()
{
	name = "Player";

	//ステートの追加
	//states.emplace_back(new Idle(parent));//待機
	//states.emplace_back(new Run(parent));//走り

	//待機ステートに
	ChangeState("Idle");
}

void Enemy::Update()
{
	//currentState->Update();//ステートの更新
	//std::string nextStateName = currentState->Judge();//ステート変更か判定
	//ChangeState(nextStateName);//nextStateNameが""じゃなければステートが変更される
}

void Enemy::Draw()
{

}

void Enemy::Finalize()
{
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
	if (ImGui::TreeNode(name.c_str()))
	{

		ImGui::TreePop();
	}
}

void Enemy::ChangeState(std::string nextStateName)
{
	if (nextStateName == "")return;

	bool changeState = false;
	//for (Default* state : states)
	//{
	//	if (state->GetName() == nextStateName)
	//	{
	//		currentState = state;//ステートの変更
	//		currentState->Enter();//ステート遷移時の処理
	//		changeState = true;
	//		OutputDebugLog(nextStateName + "ステートに遷移しました。\n");
	//		break;
	//	}
	//}

	if (!changeState)
	{
		OutputDebugLog(nextStateName + "という名前のステートは存在しません。\n");
	}
}