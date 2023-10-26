#pragma once

#include "Component/Enemy.h"

class ActionBase
{
public:
    ActionBase(Enemy* enemy) :owner(enemy) {}
    ~ActionBase() { }
    //���s���
    enum class State
    {
        Running,//���s��
        Failed,//���s
        Complete,//����
    };

    virtual ActionBase::State Run(float elapsedTime) = 0;
    virtual void Enter() = 0;
    virtual void Exit() = 0;

    //Enemy::BehaviorState GetBehaviorState() { return state; }

    //int GetStep() { return step; }
    //void SetStep(int s) { step = s; }

protected:
    Enemy* owner{ nullptr };
    //int step = 0;
    //Enemy::BehaviorState state = Enemy::BehaviorState::NONE;
};