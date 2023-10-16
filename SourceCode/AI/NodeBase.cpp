#include "Stdafx.h"
#include "NodeBase.h"
#include <magic_enum.hpp>
#include "System/UserFunction.h"

#include "System/Console.h"

NodeBase::~NodeBase()
{
    SafeDelete(judgment);
    SafeDelete(action);
    //delete judgment;
    //delete action;
}

//index番目の子ノードを取得
NodeBase* NodeBase::GetChild(int index)
{
    if (children.size() > index)
    {
        return nullptr;
    }
    return children.at(index);
}

//末尾の子ノードを取得
NodeBase* NodeBase::GetLastChild()
{
    if (children.size() == 0)
    {
        return nullptr;
    }
    return children.at(children.size() - 1);
}

//先頭の子ノードを取得
NodeBase* NodeBase::GetBeginChild()
{
    if (children.size() == 0)
    {
        return nullptr;
    }
    return children.at(0);
}

bool NodeBase::Judgment()
{
    if (judgment != nullptr)
    {
        if (judgment->Judgment())
        {
            return true;
        }
        else
            return false;
    }
    return true;
}

NodeBase* NodeBase::SelectPriority(std::vector<NodeBase*>* list)
{
    NodeBase* selectNode = nullptr;
    int priority = INT_MAX;

    //優先順位順に条件を満たしているか確認し、満たしていればそのActionを返す
    for (int i = 0; i < list->size(); i++)
    {
        for (int j = 0; j < list->at(i)->GetChildSize(); j++)
        {
            if (list->at(i)->children.at(j)->Judgment() == false) continue;
            if (priority > list->at(i)->children.at(j)->GetPriority())
            {
                priority = list->at(i)->children.at(j)->GetPriority();
                selectNode = list->at(i)->children.at(priority - 1);
            }
        }
    }

    return selectNode;
}

NodeBase* NodeBase::SelectAbsolute(std::vector<NodeBase*>* list)
{
    NodeBase* selectNode = nullptr;
    int priority = INT_MAX;

    //優先順位順に条件を満たしているか確認し、満たしていればそのActionを返す
    for (int i = 0; i < list->size(); i++)
    {
        for (int j = 0; j < list->at(i)->GetChildSize(); j++)
        {
            if (list->at(i)->children.at(j)->Judgment() == false) continue;
            if (priority > list->at(i)->children.at(j)->GetPriority())
            {
                if (list->at(i)->rule == BehaviorTree::Rule::Absolute)
                {
                    priority = list->at(i)->children.at(j)->GetPriority();
                    selectNode = list->at(i)->children.at(priority - 1);
                }
            }
        }
    }

    return selectNode;
}

NodeBase* NodeBase::SelectRandom(Enemy* enemy, std::vector<NodeBase*>* list)
{
    //↓行動に規則を持たせてみた
#if 0

    //攻撃の場合は規則に沿った行動をする
    if ((*list).at(0)->GetParent()->GetName() == "Attack")
    {
        enemy->AddAttackRegulationCount();//カウントを進める
        int selectNo = 0;
        //次の規則タイプをランダムに指定
        if (enemy->GetAttackRegulationCount() <= 0)
        {
            //enemy->AddAttackRegulationCount();//カウントを進める
            selectNo = std::rand() % 4;//0～3でランダム
            enemy->SetAttackRegulationType(selectNo);
            for (auto& li : *list)
            {
                //名前が一致した行動を返す
                if (li->GetName() == magic_enum::enum_name(enemy->GetStateAttackRegulation()).data())
                {
                    //enemy->AddAttackRegulationCount();//カウントを進める
                    return li;
                }
            }
        }
        else//既に規則のタイプが決まっている場合（カウントが１以上の場合）
        {          
            for (auto& li : *list)
            {
                //名前が一致した行動を返す
                if (li->GetName() == magic_enum::enum_name(enemy->GetStateAttackRegulation()).data())
                {
                    //enemy->AddAttackRegulationCount();//カウントを進める
                    return li;
                }
            }
        }
    }
    //攻撃以外のときはランダム
    else
#endif 
    {
        //攻撃以外の為、攻撃規則のカウントを初期化
        //enemy->SetAttackRegulationCount(0);
        int selectNo = 0;
        selectNo = std::rand() % list->size();//リストのサイズからランダム値を取得
          //ランダムに選ばれたActionを返す
        return (*list).at(selectNo);
    }
}

NodeBase* NodeBase::SelectSequence(std::vector<NodeBase*>* list, BehaviorData* data)
{
    int step = 0;

    // 指定されている中間ノードのがシーケンスがどこまで実行されたか取得する
    step = data->GetSequenceStep(name);

    // 中間ノードに登録されているノード数以上の場合、
    if (step >= children.size())
    {
        //ルールによって処理を切り替える
        // ルールがBehaviorTree::SelectRule::SequentialLoopingのときは最初から実行するため、stepに0を代入
        // ルールがBehaviorTree::SelectRule::Sequenceのときは次に実行できるノードがないため、nullptrをリターン
        if (rule == BehaviorTree::Rule::SequenceLooping)
            step = 0;
        else if (rule == BehaviorTree::Rule::Sequence)
            return nullptr;
    }
    // 実行可能リストに登録されているデータの数だけループを行う
    for (auto itr = list->begin(); itr != list->end(); itr++)
    {
        // 子ノードが実行可能リストに含まれているか
        if (children.at(step)->GetName() == (*itr)->GetName())
        {
            // 現在の実行ノード(this)の保存、次に実行するステップ(step+1)の保存を行った後、
            // 現在のステップ番号のノードを返す
            data->PushSequenceNode(this);
            data->SetSequenceStep(this->GetName(), step + 1);
            return children.at(step);
        }
    }
    // 指定された中間ノードに実行可能ノードがないのでnullptrをリターンする
    return nullptr;
}

NodeBase* NodeBase::SearchNode(std::string serchName)
{
    if (name == serchName)
    {
        return this;
    }
    for (auto it = children.begin(); it != children.end(); it++)
    {
        NodeBase* serch = (*it)->SearchNode(serchName);
        if (serch != nullptr)
        {
            return serch;
        }
    }
    return nullptr;
}

NodeBase* NodeBase::Inference(Enemy* enemy, BehaviorData* data)
{
    std::vector<NodeBase*> list;
    NodeBase* result = nullptr;

    for (int i = 0; i < children.size(); i++)
    {
        if (children.at(i)->judgment != nullptr)
        {
            if (children.at(i)->judgment->Judgment())
            {
                list.emplace_back(children.at(i));
            }
        }
        else
        {
            list.emplace_back(children.at(i));
        }
    }

    //ルールによる処理の分岐
    switch (rule)
    {
    case BehaviorTree::Rule::Priority://優先順位
        result = SelectPriority(&list);
        break;
    case BehaviorTree::Rule::Random://確率
        result = SelectRandom(enemy,&list);
        break;
    case BehaviorTree::Rule::Sequence://上から順番に
    case BehaviorTree::Rule::SequenceLooping://上から順番に最後まで実行後初めから
        result = SelectSequence(&list,data);
        break;
    case BehaviorTree::Rule::Absolute://優先的に実行
        result = SelectAbsolute(&list);
        break;

    }

    if (result != nullptr)
    {
        if (result->HaveAction())
        {
            return result;
        }
        else
        {
            result = result->Inference(enemy, data);
        }
    }
    return result;
}

NodeBase* NodeBase::InferenceAbsolute(Enemy* enemy, BehaviorData* data)
{
    std::vector<NodeBase*> list;
    NodeBase* result = nullptr;

    for (int i = 0; i < children.size(); i++)
    {
        if (children.at(i)->judgment != nullptr)
        {
            if (children.at(i)->judgment->Judgment() && children.at(i)->rule== BehaviorTree::Rule::Absolute)
            {
                list.emplace_back(children.at(i));
            }
        }
    }
    if (list.size() <= 0)
        return nullptr;
    result = SelectAbsolute(&list);

    if (result != nullptr)
    {
        if (result->HaveAction())
        {
            return result;
        }
        else
        {
            result = result->Inference(enemy, data);
        }
    }
    return result;
}

ActionBase::State NodeBase::Run()
{
    if (action != nullptr)
    {
        //行動を実行する
        return action->Run();
    }

    return ActionBase::State::Failed;
}

//行動の初期処理
void NodeBase::Enter()
{
    if (action != nullptr)
    {
        ConsoleFunc::WriteEndl("Enter");
        action->Enter();
    }
}

//行動の終了処理
void NodeBase::Exit()
{
    if (action != nullptr)
    {
        ConsoleFunc::WriteEndl("Exit");
        action->Exit();
    }
}
