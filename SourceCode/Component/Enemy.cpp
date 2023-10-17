
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

#include "Component/Transform.h"
#include "Component/ModelRenderer.h"

#include "GameObject/GameObject.h"

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
	
	//"�ҋ@�A�j���[�V����"���Đ�
	parent->GetComponent<ModelRenderer>()->PlayAnimation((int)AnimationName::Idle1, true);
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


//***********************************************************************************************
//      �p���`�A�N�V���� 
//***********************************************************************************************
void Enemy::EnterPunchState()
{
	//�p���`�A�j���[�V�������Đ�	
	parent->GetComponent<ModelRenderer>()->PlayAnimation((int)AnimationName::LeftSlashAttack, true);
}

bool Enemy::RunPunchState()
{
	//�A�j���[�V�����̍Đ����I��������X�e�[�g�̐؂�ւ�
	if (!parent->GetComponent<ModelRenderer>()->IsPlayAnimation())
	{
		return true;
	}
	return false;
}

void Enemy::ExitPunchState()
{
}
//***********************************************************************************************

//***********************************************************************************************
//      �d���A�N�V����
//***********************************************************************************************
void Enemy::EnterSkillState()
{
	//�d���A�j���[�V�������Đ�	
	parent->GetComponent<ModelRenderer>()->PlayAnimation((int)AnimationName::BrastAttack, true);
}
bool Enemy::RunSkillState()
{
	//�A�j���[�V�����̍Đ����I��������X�e�[�g�̐؂�ւ�
	if (!parent->GetComponent<ModelRenderer>()->IsPlayAnimation())
	{
		return true;
	}
	return false;
}
void Enemy::ExitSkillState()
{
}
//***********************************************************************************************

//***********************************************************************************************
//      �x�e�A�N�V����
//***********************************************************************************************
void Enemy::EnterBreakState()
{
	//�x�e�A�j���[�V�������Đ�	
	parent->GetComponent<ModelRenderer>()->PlayAnimation((int)AnimationName::Idle2, true);
}
bool Enemy::RunBreakState()
{
	//�A�j���[�V�����̍Đ����I��������X�e�[�g�̐؂�ւ�
	if (!parent->GetComponent<ModelRenderer>()->IsPlayAnimation())
	{
		return true;
	}
	return false;
}
void Enemy::ExitBreakState()
{
}
//***********************************************************************************************

//***********************************************************************************************
//      �p�j�A�N�V����
//***********************************************************************************************
void Enemy::EnterWanderState()
{
	//�����A�j���[�V�������Đ�	
	parent->GetComponent<ModelRenderer>()->PlayAnimation((int)AnimationName::DashForward, false);
}
bool Enemy::RunWanderState()
{
	//�A�j���[�V�����̍Đ����I��������X�e�[�g�̐؂�ւ�
	if (!parent->GetComponent<ModelRenderer>()->IsPlayAnimation())
	{
		return true;
	}
	return false;
}
void Enemy::ExitWanderState()
{
}
//***********************************************************************************************

//***********************************************************************************************
//      �ҋ@�A�N�V����
//***********************************************************************************************
void Enemy::EnterIdleState()
{
	//�ҋ@�A�j���[�V�������Đ�	
	parent->GetComponent<ModelRenderer>()->PlayAnimation((int)AnimationName::Idle1, true);
}
bool Enemy::RunIdleState()
{
	//�A�j���[�V�����̍Đ����I��������X�e�[�g�̐؂�ւ�
	if (!parent->GetComponent<ModelRenderer>()->IsPlayAnimation())
	{
		return true;
	}
	return false;
}
void Enemy::ExitIdleState()
{
}
//***********************************************************************************************

//***********************************************************************************************
//      �ǐՃA�N�V����
//***********************************************************************************************
void Enemy::EnterPursuitState()
{
	//�����A�j���[�V�������Đ�	
	parent->GetComponent<ModelRenderer>()->PlayAnimation((int)AnimationName::DashForward, false);
}
bool Enemy::RunPursuitState()
{
	//�A�j���[�V�����̍Đ����I��������X�e�[�g�̐؂�ւ�
	if (!parent->GetComponent<ModelRenderer>()->IsPlayAnimation())
	{
		return true;
	}
	return false;
}
void Enemy::ExitPursuitState()
{
}
//***********************************************************************************************

//***********************************************************************************************
//      �����A�N�V����
//***********************************************************************************************
void Enemy::EnterEscapeState()
{
	//�����A�j���[�V�������Đ�	
	parent->GetComponent<ModelRenderer>()->PlayAnimation((int)AnimationName::DashForward, true);
}
bool Enemy::RunEscapeState()
{
	//�A�j���[�V�����̍Đ����I��������X�e�[�g�̐؂�ւ�
	if (!parent->GetComponent<ModelRenderer>()->IsPlayAnimation())
	{
		return true;
	}
	return false;
}
void Enemy::ExitEscapeState()
{
}
//***********************************************************************************************

//***********************************************************************************************
//      ���S�A�N�V����
//***********************************************************************************************
void Enemy::EnterDieState()
{
	//���S�A�j���[�V�������Đ�	
	parent->GetComponent<ModelRenderer>()->PlayAnimation((int)AnimationName::Die, true);
}
bool Enemy::RunDieState()
{
	//�A�j���[�V�����̍Đ����I��������X�e�[�g�̐؂�ւ�
	if (!parent->GetComponent<ModelRenderer>()->IsPlayAnimation())
	{
		return true;
	}
	return false;
}
void Enemy::ExitDieState()
{
}
//***********************************************************************************************

//***********************************************************************************************
//      ��e�A�N�V����
//***********************************************************************************************
void Enemy::EnterDamageState()
{
	//��e�A�j���[�V�������Đ�	
	parent->GetComponent<ModelRenderer>()->PlayAnimation((int)AnimationName::TakeDamege, true);
}
bool Enemy::RunDamageState()
{
	//�A�j���[�V�����̍Đ����I��������X�e�[�g�̐؂�ւ�
	if (!parent->GetComponent<ModelRenderer>()->IsPlayAnimation())
	{
		return true;
	}
	return false;
}
void Enemy::ExitDamageState()
{
}
//***********************************************************************************************
