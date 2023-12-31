#pragma once

#include "Component/Enemy.h"

class ActionBase
{
public:
    ActionBase(GameObject* enemy) :owner(enemy) {}
    ~ActionBase() { }
    //実行情報
    enum class State
    {
        Running,//実行中
        Failed,//失敗
        Complete,//成功
    };

    virtual ActionBase::State Run(float elapsedTime) = 0;
    virtual void Enter() = 0;
    virtual void Exit() = 0;

    //Enemy::BehaviorState GetBehaviorState() { return state; }

    //int GetStep() { return step; }
    //void SetStep(int s) { step = s; }

protected:
    GameObject* owner{ nullptr };
    //int step = 0;
    //Enemy::BehaviorState state = Enemy::BehaviorState::NONE;
};