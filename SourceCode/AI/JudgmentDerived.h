#pragma once

#include "JudgementBase.h"
#include "Component/enemy.h"

//Battle�m�[�h�ւ̕ϊ��𔻒�i�퓬�j
class BattleJudgment :public JudgmentBase
{
public:
    BattleJudgment(Enemy* enemy) :JudgmentBase(enemy) {}
    bool Judgment()override;
};

//Escape�m�[�h�ւ̕ϊ��𔻒�i�����j
class EscapeJudgment :public JudgmentBase
{
public:
    EscapeJudgment(Enemy* enemy) :JudgmentBase(enemy) {}
    bool Judgment()override;
};

//Attack�m�[�h�ւ̕ϊ��𔻒�i�U���j
class AttackJudgment :public JudgmentBase
{
public:
    AttackJudgment(Enemy* enemy) :JudgmentBase(enemy) {}
    bool Judgment()override;
};

//Damage�m�[�h�ւ̕ϊ��𔻒�i��e�j
class DamageJudgment :public JudgmentBase
{
public:
    DamageJudgment(Enemy* enemy) :JudgmentBase(enemy) {}
    bool Judgment()override;
};

//Die�m�[�h�ւ̕ϊ��𔻒�i���S�j
class DeathJudgment :public JudgmentBase
{
public:
    DeathJudgment(Enemy* enemy) :JudgmentBase(enemy) {}
    bool Judgment()override;
};

//Wander�m�[�h�ւ̕ϊ��𔻒�i�p�j�j
class WanderJudgment :public JudgmentBase
{
public:
    WanderJudgment(Enemy* enemy) :JudgmentBase(enemy) {}
    bool Judgment()override;
};
