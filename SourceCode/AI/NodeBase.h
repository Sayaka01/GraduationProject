#pragma once

#include <string>
#include <list>
#include "BehaviorTree.h"
#include "ActionBase.h"
#include "JudgementBase.h"
#include "BehaviorData.h"

class NodeBase
{
public:
    //�R���X�g���N�^
    NodeBase(std::string name, NodeBase* parent, NodeBase* sibling, int priority,
        BehaviorTree::Rule selectRule, JudgmentBase* judgment, ActionBase* action, int hierarchyNo):
        name(name),parent(parent),sibling(sibling),priority(priority),
        rule(selectRule),judgment(judgment),action(action),hierarchyNo(hierarchyNo),children(NULL){}

    //�f�X�g���N�^
    ~NodeBase();

    //�Q�b�^�[
    std::string GetName() { return name; }
    NodeBase* GetParent() { return parent; }
    NodeBase* GetChild(int index);//index�Ԗڂ̎q�m�[�h���擾
    NodeBase* GetLastChild();//�����̎q�m�[�h���擾
    NodeBase* GetBeginChild();//�擪�̎q�m�[�h���擾
    NodeBase* GetSibling() { return sibling; }
    int GetHierrchyNo() { return hierarchyNo; }
    int GetPriority() { return priority; }
    //�Z�b�^�[
    void SetParent(NodeBase* parent) { this->parent = parent; }
    //�q�m�[�h�ǉ�
    void AddChild(NodeBase* child) { children.emplace_back(child); }
    int GetChildSize() { return static_cast<int>(children.size()); }
    void SetSibling(NodeBase* sibling) { this->sibling = sibling; }
    bool HaveAction() { return action != nullptr ? true : false; }
    bool Judgment();
    NodeBase* SelectPriority(std::vector<NodeBase*>* list);
    NodeBase* SelectAbsolute(std::vector<NodeBase*>* list);
    NodeBase* SelectRandom(GameObject* enemy, std::vector<NodeBase*>* list);
    NodeBase* SelectSequence(std::vector<NodeBase*>* list, BehaviorData* data);
    NodeBase* SearchNode(std::string serchName);
    NodeBase* Inference(GameObject* enemy, BehaviorData* data);
    NodeBase* InferenceAbsolute(GameObject* enemy, BehaviorData* data);
    ActionBase::State Run(float elapsedTime);
    void Enter();// �s���̏�������
    void Exit();// �s���̏I������
    std::vector<NodeBase*> children;//�q�m�[�h
protected:
    std::string         name;//���O
    BehaviorTree::Rule  rule;//�m�[�h��I�����郋�[��
    JudgmentBase*       judgment;//���肷��N���X
    ActionBase*         action;//���s����N���X
    int                 priority;//�D�揇��
    NodeBase*           parent{ nullptr };//�e�m�[�h
    NodeBase*           sibling{ nullptr };//�Z��m�[�h
    int                 hierarchyNo;//�K�w�ԍ�
    std::list<ActionBase*> actionBaseList;
};