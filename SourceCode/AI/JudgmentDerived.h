#pragma once

#include "JudgementBase.h"
#include "Component/enemy.h"

//Battleノードへの変換を判定（戦闘）
class BattleJudgment :public JudgmentBase
{
public:
    BattleJudgment(GameObject* enemy) :JudgmentBase(enemy) {}
    bool Judgment()override;
};

//Escapeノードへの変換を判定（逃走）
class EscapeJudgment :public JudgmentBase
{
public:
    EscapeJudgment(GameObject* enemy) :JudgmentBase(enemy) {}
    bool Judgment()override;
};

//Attackノードへの変換を判定（攻撃）
class AttackJudgment :public JudgmentBase
{
public:
    AttackJudgment(GameObject* enemy) :JudgmentBase(enemy) {}
    bool Judgment()override;
};

//Damageノードへの変換を判定（被弾）
class DamageJudgment :public JudgmentBase
{
public:
    DamageJudgment(GameObject* enemy) :JudgmentBase(enemy) {}
    bool Judgment()override;
};

//Dieノードへの変換を判定（死亡）
class DeathJudgment :public JudgmentBase
{
public:
    DeathJudgment(GameObject* enemy) :JudgmentBase(enemy) {}
    bool Judgment()override;
};

//Wanderノードへの変換を判定（徘徊）
class WanderJudgment :public JudgmentBase
{
public:
    WanderJudgment(GameObject* enemy) :JudgmentBase(enemy) {}
    bool Judgment()override;
};
