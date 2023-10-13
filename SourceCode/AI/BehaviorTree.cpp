#include "Stdafx.h"
#include "BehaviorTree.h"
#include "ActionBase.h"
#include "BehaviorData.h"
#include "NodeBase.h"
#include "Component/enemy.h"
#include "System/UserFunction.h"

BehaviorTree::~BehaviorTree()
{
    NodeAllClear(root);
}

//実行ノードを推論
NodeBase* BehaviorTree::ActiveNodeInference(BehaviorData* data)
{
    //データをリセット
    data->Init();
    return root->Inference(owner, data);
}

//シーケンスノードから推論
NodeBase* BehaviorTree::SequenceBack(NodeBase* sequenceNode, BehaviorData* data)
{
    return sequenceNode->Inference(owner, data);
}

//ノード追加
void BehaviorTree::AddNode(std::string parentName, std::string entryName, int priority, Rule selectRule, JudgmentBase* judgment, ActionBase* action)
{
    if (parentName != "")
    {
        NodeBase* parentNode = root->SearchNode(parentName);

        if (parentNode != nullptr)
        {
            NodeBase* sibling = parentNode->GetLastChild();
            NodeBase* addNode = new NodeBase(entryName, parentNode, sibling, priority, selectRule, judgment, action, parentNode->GetHierrchyNo() + 1);

            parentNode->AddChild(addNode);
        }
    }
    else
    {
        if (root == nullptr)
        {
            root = new NodeBase(entryName, nullptr, nullptr, priority, selectRule, judgment, action, 1);
        }
    }
}

//実行
NodeBase* BehaviorTree::Run(NodeBase* actionNode, BehaviorData* data, float elapsedTime)
{
    //ノード実行
    ActionBase::State state = actionNode->Run(elapsedTime);

    //正常終了
    if (state == ActionBase::State::Complete)
    {
        //シーケンスの途中かを判断
        NodeBase* sequenceNode = data->PopSequenceNode();

        //途中じゃないなら終了
        if (sequenceNode == nullptr)
        {
            //ノードの終了処理を実行
            //actionNode->Exit();
            return nullptr;
        }
        else
        {
            //途中ならそこから始める
            return SequenceBack(sequenceNode, data);
        }
    }
    else if (state == ActionBase::State::Failed)
    {
        return nullptr;
    }
    
    NodeBase* node = root->InferenceAbsolute(owner, data);
    if (node != nullptr)
        return node;

    return actionNode;
}

//ノード全削除
void BehaviorTree::NodeAllClear(NodeBase* delNode)
{
    size_t count = delNode->children.size();

    if (count > 0)
    {
        for (const auto& node : delNode->children)
        {
            NodeAllClear(node);
        }
        delete delNode;
    }
    else
    {
        delete delNode;
    }
}
