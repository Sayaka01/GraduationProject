#pragma once

#include "ActionBase.h"
#include "Component/Enemy.h"

//�Ō�
class PunchAction :public ActionBase
{
public:
    PunchAction(GameObject* enemy) :ActionBase(enemy) {}
    ActionBase::State Run(float elapsedTime)override;    // �s����
    void Enter()override;   // �s���̏�������
    void Exit()override;    // �s���̏I������
};

//�d��
class SkillAction :public ActionBase
{
public:
    SkillAction(GameObject* enemy) :ActionBase(enemy) {}
    ActionBase::State Run(float elapsedTime)override;    // �s����
    void Enter()override;   // �s���̏�������
    void Exit()override;    // �s���̏I������
};

//�x�e
class BreakAction :public ActionBase
{
public:
    BreakAction(GameObject* enemy) :ActionBase(enemy) { }
    ActionBase::State Run(float elapsedTime)override;    // �s����
    void Enter()override;   // �s���̏�������
    void Exit()override;    // �s���̏I������
};

//�p�j
class WanderAction :public ActionBase
{
public:
    WanderAction(GameObject* enemy) :ActionBase(enemy) { }
    ActionBase::State Run(float elapsedTime)override;    // �s����
    void Enter()override;   // �s���̏�������
    void Exit()override;    // �s���̏I������
};

//�ҋ@
class IdleAction :public ActionBase
{
public:
    IdleAction(GameObject* enemy) :ActionBase(enemy) { }
    ActionBase::State Run(float elapsedTime)override;    // �s����
    void Enter()override;   // �s���̏�������
    void Exit()override;    // �s���̏I������
};

//�ǐ�
class PursuitAction :public ActionBase
{
public:
    PursuitAction(GameObject* enemy) :ActionBase(enemy) { }
    ActionBase::State Run(float elapsedTime)override;    // �s����
    void Enter()override;   // �s���̏�������
    void Exit()override;    // �s���̏I������
};

//�e��
//class ShotAction :public ActionBase
//{
//public:
//    ShotAction(Enemy* enemy) :ActionBase(enemy) { }
//    ActionBase::State Run()override;
//    void Enter()override;
//    void Exit()override;
//};

//����
class EscapeAction :public ActionBase
{
public:
    EscapeAction(GameObject* enemy) :ActionBase(enemy) { }
    ActionBase::State Run(float elapsedTime)override;    // �s����
    void Enter()override;   // �s���̏�������
    void Exit()override;    // �s���̏I������
};

//���S
class DieAction :public ActionBase
{
public:
    DieAction(GameObject* enemy) :ActionBase(enemy) { }
    ActionBase::State Run(float elapsedTime)override;    // �s����
    void Enter()override;   // �s���̏�������
    void Exit()override;    // �s���̏I������
};

//��e
class DamageAction :public ActionBase
{
public:
    DamageAction(GameObject* enemy) :ActionBase(enemy) { }
    ActionBase::State Run(float elapsedTime)override;    // �s����
    void Enter()override;   // �s���̏�������
    void Exit()override;    // �s���̏I������

private:

};

