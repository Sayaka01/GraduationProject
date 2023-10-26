#pragma once

//#include "../Objects/enemy.h"s

//前方宣言
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
        Non,//末端ノード用
        Priority,//優先順位順に実行
        Sequence,//子ノードを上から順に実行
        SequenceLooping,//子ノードを上から順に実行し末端が完了した場合始めに戻る
        Random,//確率(ランダム)で実行
        Absolute,//絶対的に優先
    };

    BehaviorTree() :root(nullptr), owner(nullptr) {}
    BehaviorTree(Enemy* enemy) :root(nullptr), owner(enemy) {}
    ~BehaviorTree();

    //実行ノードを推論
    NodeBase* ActiveNodeInference(BehaviorData* data);

    //シーケンスノードから推論
    NodeBase* SequenceBack(NodeBase* sequenceNode, BehaviorData* data);

    //ノード追加
    void AddNode(std::string parentName, std::string entryName, int priority, Rule selectRule, JudgmentBase* judgment, ActionBase* action);

    //実行
    NodeBase* Run(NodeBase* actionNode, BehaviorData* data, float elapsedTime);

private:
    //ノード全削除
    void NodeAllClear(NodeBase* delNode);
private:
    NodeBase* root{ nullptr };
    Enemy* owner{nullptr};
};