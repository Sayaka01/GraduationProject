#pragma once

#include <string>
#include <stack>
#include <map>
class NodeBase;

class BehaviorData
{
public:
    //コンストラクタ
    BehaviorData() { Init(); }

    //シーケンスノードをプッシュ
    void PushSequenceNode(NodeBase* node) { sequenceStack.push(node); }

    //シーケンスノードをポップ(先頭の要素を１つ削除)
    NodeBase* PopSequenceNode();

    //シーケンスステップのゲッター
    int GetSequenceStep(std::string name);

    //シーケンスステップのセッター
    void SetSequenceStep(std::string name, int step);

    //初期化
    void Init();

private:
    std::stack<NodeBase*> sequenceStack;//実行する中間ノードをスタック
    std::map<std::string, int> runSequenceStepMap;//実行中の中間ノードのステップを記録

};