#include "Stdafx.h"
#include "JudgmentDerived.h"

bool BattleJudgment::Judgment()
{
    //if (owner->GetIsPlayerAttack())//プレイヤーを探している場合
    {   
        //プレイヤーを追跡範囲内で見つけたらBattleノードへ
        //if (owner->SerchPlayer(owner->GetPursuitRange()))
        {
            //ノードの初期処理
            //actionNode->Enter();
            return true;
        }
    }

    return false;
}

bool EscapeJudgment::Judgment()
{
    //対象のHPが通常の60パー以下になったらEscapeノードへ
    //if (owner->GetHp() < owner->GetMaxHp() * 0.4f)
    {
        //return true;
    }
    return false;
}

bool AttackJudgment::Judgment()
{
        //プレイヤーと敵の距離が一定以下になったら攻撃可能
        //float lengthPl = Math::Length(Math::SubAll(owner->GetPosition(), owner->GetPlayerPosition()));
        //if (lengthPl < owner->GetAttackRange())
        {
            return true;
        }

    return false;
}

bool WanderJudgment::Judgment()
{
    //float length = Math::Length(Math::SubAll(owner->GetPosition(), owner->GetTargetPosition()));

    //if (owner->GetIsPlayerAttack())//プレイヤーを探している場合
    {
        //プレイヤーと敵の距離が一定以上になったら徘徊可能
        //float lengthPl = Math::Length(Math::SubAll(owner->GetPosition(), owner->GetPlayerPosition()));
        //if (lengthPl > owner->GetPursuitRange() && length>=1.5f)
        {
            return true;
        }

    }
    return false;
}

bool DamageJudgment::Judgment()
{
    //自分以外の銃との当たり判定
    //if (BulletManager::Instance().HitCollisionObject(owner))
    //{
    //    return true;
    //}
    //ダメージ通知を受けていた場合はDamageノードへ
    //if (owner->GetIsDamaged())
    {
        //owner->SetIsDamaged(false);
        return true;
    }
    return false;
}

bool DeathJudgment::Judgment()
{
    //HPが０以下になったらDeathノードへ
    //if (owner->GetHp() <= 0)
    {
        return true;
    }
    return false;
}
