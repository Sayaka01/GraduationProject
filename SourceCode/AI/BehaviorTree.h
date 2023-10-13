#pragma once

//#include "../Objects/enemy.h"s

//�O���錾
class NodeBase;
class BehaviorData;
class ActionBase;
class JudgmentBase;
class Enemy;

class BehaviorTree
{
public:
    enum class Rule
    {
        Non,//���[�m�[�h�p
        Priority,//�D�揇�ʏ��Ɏ��s
        Sequence,//�q�m�[�h���ォ�珇�Ɏ��s
        SequenceLooping,//�q�m�[�h���ォ�珇�Ɏ��s�����[�����������ꍇ�n�߂ɖ߂�
        Random,//�m��(�����_��)�Ŏ��s
        Absolute,//��ΓI�ɗD��
    };

    BehaviorTree() :root(nullptr), owner(nullptr) {}
    BehaviorTree(Enemy* enemy) :root(nullptr), owner(enemy) {}
    ~BehaviorTree();

    //���s�m�[�h�𐄘_
    NodeBase* ActiveNodeInference(BehaviorData* data);

    //�V�[�P���X�m�[�h���琄�_
    NodeBase* SequenceBack(NodeBase* sequenceNode, BehaviorData* data);

    //�m�[�h�ǉ�
    void AddNode(std::string parentName, std::string entryName, int priority, Rule selectRule, JudgmentBase* judgment, ActionBase* action);

    //���s
    NodeBase* Run(NodeBase* actionNode, BehaviorData* data, float elapsedTime);

private:
    //�m�[�h�S�폜
    void NodeAllClear(NodeBase* delNode);
private:
    NodeBase* root{ nullptr };
    Enemy* owner{nullptr};
};