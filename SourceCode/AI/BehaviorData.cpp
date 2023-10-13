#include "Stdafx.h"
#include "BehaviorData.h"

//シーケンスノードをポップ(先頭の要素を１つ削除)
NodeBase* BehaviorData::PopSequenceNode()
{
    //空の場合はnullptrを返す
    if (sequenceStack.empty())
        return nullptr;
    NodeBase* node = sequenceStack.top();
    if (node != nullptr)
    {
        //取り出したデータを削除
        sequenceStack.pop();
    }
    return node;
}

//シーケンスステップのゲッター
int BehaviorData::GetSequenceStep(std::string name)
{
    if (runSequenceStepMap.count(name) == 0)
    {
        //キーが存在しなければ追加
        runSequenceStepMap.insert(std::make_pair(name, 0));

    }
    return runSequenceStepMap.at(name);
}

//シーケンスステップのセッター
void BehaviorData::SetSequenceStep(std::string name, int step)
{
    runSequenceStepMap.at(name) = step;
}

//初期化
void BehaviorData::Init()
{
    runSequenceStepMap.clear();
    while (sequenceStack.size() > 0)
    {
        sequenceStack.pop();
    }
}
