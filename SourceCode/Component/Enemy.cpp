
#include "Stdafx.h"
#include "Enemy.h"

#include "System/UserFunction.h"

#include "AI/BehaviorTree.h"
#include "AI/NodeBase.h"
#include "AI/BehaviorData.h"
#include "AI/ActionDerived.h"
#include "AI/JudgmentDerived.h"

#include "System/SystemManager.h"

#include "System/Console.h"

using namespace PlayerState;


void Enemy::Initialize()
{
#ifdef OpenConsole
	ConsoleFunc::OpenWindow();
#endif

	name = "Enemy";

	//�r�w�C�r�A�c���[�ݒ�
	behaviorData = new BehaviorData();
	aiTree = new BehaviorTree(this);

	//ToDo�F�G�N�Z���œǂݍ��ނ悤�ɕύX
	aiTree->AddNode("", "Root", 0, BehaviorTree::Rule::Random, nullptr, nullptr);
	//aiTree->AddNode("", "Root", 0, BehaviorTree::Rule::Priority, nullptr, nullptr);

	//aiTree->AddNode("Root", "Escape", 1, BehaviorTree::Rule::Sequence, new EscapeJudgment(this), nullptr);
	aiTree->AddNode("Root", "Death", 1, BehaviorTree::Rule::Random, new DamageJudgment(this), nullptr);
	aiTree->AddNode("Root", "Battle", 2, BehaviorTree::Rule::Random, new BattleJudgment(this), nullptr);
	aiTree->AddNode("Root", "Scout", 3, BehaviorTree::Rule::Random, nullptr, nullptr);
	//aiTree->AddNode("Root", "Death", 1, BehaviorTree::Rule::Absolute, new DamageJudgment(this), nullptr);
	//aiTree->AddNode("Root", "Battle", 2, BehaviorTree::Rule::Priority, new BattleJudgment(this), nullptr);
	//aiTree->AddNode("Root", "Scout", 3, BehaviorTree::Rule::Priority, nullptr, nullptr);

	aiTree->AddNode("Death", "Die", 1, BehaviorTree::Rule::Random, new DeathJudgment(this), new DieAction(this));
	aiTree->AddNode("Death", "Damage", 2, BehaviorTree::Rule::Random, nullptr, new DamageAction(this));
	//aiTree->AddNode("Death", "Die", 1, BehaviorTree::Rule::Non, new DeathJudgment(this), new DieAction(this));
	//aiTree->AddNode("Death", "Damage", 2, BehaviorTree::Rule::Non, nullptr, new DamageAction(this));

	aiTree->AddNode("Battle", "Attack", 1, BehaviorTree::Rule::Random, new AttackJudgment(this), nullptr);
	aiTree->AddNode("Battle", "Discovery", 2, BehaviorTree::Rule::Random, nullptr, nullptr);

	aiTree->AddNode("Scout", "Wander", 1, BehaviorTree::Rule::Random, new WanderJudgment(this), new WanderAction(this));
	aiTree->AddNode("Scout", "Idle", 2, BehaviorTree::Rule::Random, nullptr, new IdleAction(this));
	//aiTree->AddNode("Scout", "Wander", 1, BehaviorTree::Rule::Non, new WanderJudgment(this), new WanderAction(this));
	//aiTree->AddNode("Scout", "Idle", 2, BehaviorTree::Rule::Non, nullptr, new IdleAction(this));

	aiTree->AddNode("Attack", "Punch", 1, BehaviorTree::Rule::Random, nullptr, new PunchAction(this));
	aiTree->AddNode("Attack", "Skill", 2, BehaviorTree::Rule::Random, nullptr, new SkillAction(this));
	aiTree->AddNode("Attack", "Break", 3, BehaviorTree::Rule::Random, nullptr, new BreakAction(this));
	//aiTree->AddNode("Attack", "Punch", 1, BehaviorTree::Rule::Non, nullptr, new PunchAction(this));
	//aiTree->AddNode("Attack", "Skill", 2, BehaviorTree::Rule::Non, nullptr, new SkillAction(this));
	//aiTree->AddNode("Attack", "Break", 3, BehaviorTree::Rule::Non, nullptr, new BreakAction(this));

	aiTree->AddNode("Discovery", "Pursuit", 1, BehaviorTree::Rule::Non, nullptr, new PursuitAction(this));


	//�X�e�[�g�̒ǉ�
	//states.emplace_back(new Idle(parent));//�ҋ@
	//states.emplace_back(new Run(parent));//����

	//�ҋ@�X�e�[�g��
	//ChangeState("Idle");
}

void Enemy::Update()
{

	//�r�w�C�r�A�c���[
	if (activeNode == nullptr)
		activeNode = aiTree->ActiveNodeInference(behaviorData);
	else
		activeNode = aiTree->Run(activeNode, behaviorData);

	//currentState->Update();//�X�e�[�g�̍X�V
	//std::string nextStateName = currentState->Judge();//�X�e�[�g�ύX������
	//ChangeState(nextStateName);//nextStateName��""����Ȃ���΃X�e�[�g���ύX�����
}

void Enemy::Draw()
{

}

void Enemy::Finalize()
{
#ifdef OpenConsole
	ConsoleFunc::CloseWiindow();
#endif

	SafeDelete(aiTree);
	SafeDelete(behaviorData);
	//SafeDelete(activeNode);

	//std::vector<PlayerState::Default*> removes;//���

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
		std::string stateStr = "";
		std::string parentStateStr = "";
		if (activeNode != nullptr)
		{
			stateStr = activeNode->GetName();
			parentStateStr = activeNode->GetParent()->GetName();
		}
		ImGui::Text("Behavior :%s : %s", parentStateStr.c_str(), stateStr.c_str());

		ImGui::TreePop();
	}
}
