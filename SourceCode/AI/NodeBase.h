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
    //コンストラクタ
    NodeBase(std::string name, NodeBase* parent, NodeBase* sibling, int priority,
        BehaviorTree::Rule selectRule, JudgmentBase* judgment, ActionBase* action, int hierarchyNo):
        name(name),parent(parent),sibling(sibling),priority(priority),
        rule(selectRule),judgment(judgment),action(action),hierarchyNo(hierarchyNo),children(NULL){}

    //デストラクタ
    ~NodeBase();

    //ゲッター
    std::string GetName() { return name; }
    NodeBase* GetParent() { return parent; }
    NodeBase* GetChild(int index);//index番目の子ノードを取得
    NodeBase* GetLastChild();//末尾の子ノードを取得
    NodeBase* GetBeginChild();//先頭の子ノードを取得
    NodeBase* GetSibling() { return sibling; }
    int GetHierrchyNo() { return hierarchyNo; }
    int GetPriority() { return priority; }
    //セッター
    void SetParent(NodeBase* parent) { this->parent = parent; }
    //子ノード追加
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
    void Enter();// 行動の初期処理
    void Exit();// 行動の終了処理
    std::vector<NodeBase*> children;//子ノード
protected:
    std::string         name;//名前
    BehaviorTree::Rule  rule;//ノードを選択するルール
    JudgmentBase*       judgment;//判定するクラス
    ActionBase*         action;//実行するクラス
    int                 priority;//優先順位
    NodeBase*           parent{ nullptr };//親ノード
    NodeBase*           sibling{ nullptr };//兄弟ノード
    int                 hierarchyNo;//階層番号
    std::list<ActionBase*> actionBaseList;
};