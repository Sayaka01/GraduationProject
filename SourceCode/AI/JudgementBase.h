#pragma once

#include "Component/Enemy.h"

class JudgmentBase
{
public:
    JudgmentBase(GameObject* enemy) :owner(enemy) {}
    virtual bool Judgment() = 0;
protected:
    GameObject* owner;
};