#include "Stdafx.h"
#include "JudgmentDerived.h"
#include "Component/Health.h"

bool BattleJudgment::Judgment()
{    
    //プレイヤーオブジェクトを取得
    GameObject* playerObj = owner->GetParent()->GetParent()->GetChild("player");
    if (playerObj != nullptr)
    {
        //プレイヤーのHPがなければ攻撃しない
        if (!playerObj->GetComponent<Health>()->GetIsAlive())
            return false;

        int count = 0;
        // 他の敵が攻撃中なら攻撃しない
        GameObject* enemyManager = owner->GetParent();
        for (int i=0;i< enemyManager->GetChildrenCount();i++)
        {
            Enemy* otherEnemy = enemyManager->GetGameObj(i)->GetComponent<Enemy>();
            if (otherEnemy->GetStateName() == Enemy::StateName::Punch)
                return false;
            else if (otherEnemy->GetStateName() == Enemy::StateName::Slash)
                return false;
        }

        //プレイヤーとの距離
        DirectX::SimpleMath::Vector3 vector = playerObj->GetComponent<Transform>()->pos - owner->GetComponent<Transform>()->pos;
        float length = vector.Length();

        //プレイヤーと敵の距離が一定以下になったら攻撃可能
        if (length < owner->GetComponent<Enemy>()->GetPursuitRange())//攻撃範囲に入っている場合
        {
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
    //プレイヤーオブジェクトを取得
    GameObject* playerObj = owner->GetParent()->GetParent()->GetChild("player");
    if (playerObj != nullptr)
    {
        // 他の敵が攻撃中なら攻撃しない
        GameObject* enemyManager = owner->GetParent();
        for (int i = 0; i < enemyManager->GetChildrenCount(); i++)
        {
            Enemy* otherEnemy = enemyManager->GetGameObj(i)->GetComponent<Enemy>();
            if (otherEnemy->GetStateName() == Enemy::StateName::Punch)
                return false;
            else if (otherEnemy->GetStateName() == Enemy::StateName::Slash)
                return false;
        }

        //プレイヤーとの距離
        DirectX::SimpleMath::Vector3 vector = playerObj->GetComponent<Transform>()->pos - owner->GetComponent<Transform>()->pos;
        float length = vector.Length();

        //プレイヤーと敵の距離が一定以下になったら攻撃可能
        if (length < owner->GetComponent<Enemy>()->GetAttackRange())
        {
            return true;
        }
    }
    return false;
}

bool WanderJudgment::Judgment()
{

    //プレイヤーオブジェクトを取得
    GameObject* playerObj = owner->GetParent()->GetParent()->GetChild("player");
    if (playerObj != nullptr)
    {
        //プレイヤーとの距離
        DirectX::SimpleMath::Vector3 vector = playerObj->GetComponent<Transform>()->pos - owner->GetComponent<Transform>()->pos;
        float length = vector.Length();

        //プレイヤーと敵の距離が一定以上になったら徘徊
        if (length > owner->GetComponent<Enemy>()->GetPursuitRange() && owner->GetComponent<Enemy>()->GetLengthToTargetPosition() >= 1.5f)
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
     //プレイヤーオブジェクトを取得
    //GameObject* playerObj = owner->GetParent()->GetParent()->GetParent()->GetChild("player");
    //if (playerObj != nullptr)
    //{
    //    playerObj->
    //}
    if (owner->GetComponent<Enemy>()->GetHitFlag())
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
    if(owner->GetComponent<Health>()->GetHpRate()<=0.0f)
    {
        return true;
    }
    return false;
}
