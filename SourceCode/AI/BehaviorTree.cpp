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

//���s�m�[�h�𐄘_
NodeBase* BehaviorTree::ActiveNodeInference(BehaviorData* data)
{
    //�f�[�^�����Z�b�g
    data->Init();
    return root->Inference(owner, data);
}

//�V�[�P���X�m�[�h���琄�_
NodeBase* BehaviorTree::SequenceBack(NodeBase* sequenceNode, BehaviorData* data)
{
    return sequenceNode->Inference(owner, data);
}

//�m�[�h�ǉ�
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

//���s
NodeBase* BehaviorTree::Run(NodeBase* actionNode, BehaviorData* data, float elapsedTime)
{
    //�m�[�h���s
    ActionBase::State state = actionNode->Run(elapsedTime);

    //����I��
    if (state == ActionBase::State::Complete)
    {
        //�V�[�P���X�̓r�����𔻒f
        NodeBase* sequenceNode = data->PopSequenceNode();

        //�r������Ȃ��Ȃ�I��
        if (sequenceNode == nullptr)
        {
            //�m�[�h�̏I�����������s
            //actionNode->Exit();
            return nullptr;
        }
        else
        {
            //�r���Ȃ炻������n�߂�
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

//�m�[�h�S�폜
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
