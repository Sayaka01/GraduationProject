#pragma once

#include "ActionBase.h"
#include "Component/Enemy.h"

//打撃
class PunchAction :public ActionBase
{
public:
    PunchAction(Enemy* enemy) :ActionBase(enemy) {}
    ActionBase::State Run(float elapsedTime)override;    // 行動中
    void Enter()override;   // 行動の初期処理
    void Exit()override;    // 行動の終了処理
};

//重撃
class SkillAction :public ActionBase
{
public:
    SkillAction(Enemy* enemy) :ActionBase(enemy) {}
    ActionBase::State Run(float elapsedTime)override;    // 行動中
    void Enter()override;   // 行動の初期処理
    void Exit()override;    // 行動の終了処理
};

//休憩
class BreakAction :public ActionBase
{
public:
    BreakAction(Enemy* enemy) :ActionBase(enemy) { }
    ActionBase::State Run(float elapsedTime)override;    // 行動中
    void Enter()override;   // 行動の初期処理
    void Exit()override;    // 行動の終了処理
};

//徘徊
class WanderAction :public ActionBase
{
public:
    WanderAction(Enemy* enemy) :ActionBase(enemy) { }
    ActionBase::State Run(float elapsedTime)override;    // 行動中
    void Enter()override;   // 行動の初期処理
    void Exit()override;    // 行動の終了処理
};

//待機
class IdleAction :public ActionBase
{
public:
    IdleAction(Enemy* enemy) :ActionBase(enemy) { }
    ActionBase::State Run(float elapsedTime)override;    // 行動中
    void Enter()override;   // 行動の初期処理
    void Exit()override;    // 行動の終了処理
};

//追跡
class PursuitAction :public ActionBase
{
public:
    PursuitAction(Enemy* enemy) :ActionBase(enemy) { }
    ActionBase::State Run(float elapsedTime)override;    // 行動中
    void Enter()override;   // 行動の初期処理
    void Exit()override;    // 行動の終了処理
};

//銃撃
//class ShotAction :public ActionBase
//{
//public:
//    ShotAction(Enemy* enemy) :ActionBase(enemy) { }
//    ActionBase::State Run()override;
//    void Enter()override;
//    void Exit()override;
//};

//逃走
class EscapeAction :public ActionBase
{
public:
    EscapeAction(Enemy* enemy) :ActionBase(enemy) { }
    ActionBase::State Run(float elapsedTime)override;    // 行動中
    void Enter()override;   // 行動の初期処理
    void Exit()override;    // 行動の終了処理
};

//死亡
class DieAction :public ActionBase
{
public:
    DieAction(Enemy* enemy) :ActionBase(enemy) { }
    ActionBase::State Run(float elapsedTime)override;    // 行動中
    void Enter()override;   // 行動の初期処理
    void Exit()override;    // 行動の終了処理
};

//被弾
class DamageAction :public ActionBase
{
public:
    DamageAction(Enemy* enemy) :ActionBase(enemy) { }
    ActionBase::State Run(float elapsedTime)override;    // 行動中
    void Enter()override;   // 行動の初期処理
    void Exit()override;    // 行動の終了処理

private:

};

