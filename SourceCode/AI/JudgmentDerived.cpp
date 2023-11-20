#include "Stdafx.h"
#include "JudgmentDerived.h"
#include "Component/Health.h"

bool BattleJudgment::Judgment()
{    
    //�v���C���[�I�u�W�F�N�g���擾
    GameObject* playerObj = owner->GetParent()->GetParent()->GetChild("player");
    if (playerObj != nullptr)
    {
        //�v���C���[��HP���Ȃ���΍U�����Ȃ�
        if (!playerObj->GetComponent<Health>()->GetIsAlive())
            return false;

        int count = 0;
        // ���̓G���U�����Ȃ�U�����Ȃ�
        GameObject* enemyManager = owner->GetParent();
        for (int i=0;i< enemyManager->GetChildrenCount();i++)
        {
            Enemy* otherEnemy = enemyManager->GetGameObj(i)->GetComponent<Enemy>();
            if (otherEnemy->GetStateName() == Enemy::StateName::Punch)
                return false;
            else if (otherEnemy->GetStateName() == Enemy::StateName::Slash)
                return false;
        }

        //�v���C���[�Ƃ̋���
        DirectX::SimpleMath::Vector3 vector = playerObj->GetComponent<Transform>()->pos - owner->GetComponent<Transform>()->pos;
        float length = vector.Length();

        //�v���C���[�ƓG�̋��������ȉ��ɂȂ�����U���\
        if (length < owner->GetComponent<Enemy>()->GetPursuitRange())//�U���͈͂ɓ����Ă���ꍇ
        {
            return true;
        }
    }

    return false;
}

bool EscapeJudgment::Judgment()
{
    //�Ώۂ�HP���ʏ��60�p�[�ȉ��ɂȂ�����Escape�m�[�h��
    //if (owner->GetHp() < owner->GetMaxHp() * 0.4f)
    {
        //return true;
    }
    return false;
}

bool AttackJudgment::Judgment()
{
    //�v���C���[�I�u�W�F�N�g���擾
    GameObject* playerObj = owner->GetParent()->GetParent()->GetChild("player");
    if (playerObj != nullptr)
    {
        // ���̓G���U�����Ȃ�U�����Ȃ�
        GameObject* enemyManager = owner->GetParent();
        for (int i = 0; i < enemyManager->GetChildrenCount(); i++)
        {
            Enemy* otherEnemy = enemyManager->GetGameObj(i)->GetComponent<Enemy>();
            if (otherEnemy->GetStateName() == Enemy::StateName::Punch)
                return false;
            else if (otherEnemy->GetStateName() == Enemy::StateName::Slash)
                return false;
        }

        //�v���C���[�Ƃ̋���
        DirectX::SimpleMath::Vector3 vector = playerObj->GetComponent<Transform>()->pos - owner->GetComponent<Transform>()->pos;
        float length = vector.Length();

        //�v���C���[�ƓG�̋��������ȉ��ɂȂ�����U���\
        if (length < owner->GetComponent<Enemy>()->GetAttackRange())
        {
            return true;
        }
    }
    return false;
}

bool WanderJudgment::Judgment()
{

    //�v���C���[�I�u�W�F�N�g���擾
    GameObject* playerObj = owner->GetParent()->GetParent()->GetChild("player");
    if (playerObj != nullptr)
    {
        //�v���C���[�Ƃ̋���
        DirectX::SimpleMath::Vector3 vector = playerObj->GetComponent<Transform>()->pos - owner->GetComponent<Transform>()->pos;
        float length = vector.Length();

        //�v���C���[�ƓG�̋��������ȏ�ɂȂ�����p�j
        if (length > owner->GetComponent<Enemy>()->GetPursuitRange() && owner->GetComponent<Enemy>()->GetLengthToTargetPosition() >= 1.5f)
        {
            return true;
        }
    }
    return false;
}

bool DamageJudgment::Judgment()
{
    //�����ȊO�̏e�Ƃ̓����蔻��
    //if (BulletManager::Instance().HitCollisionObject(owner))
    //{
    //    return true;
    //}
    //�_���[�W�ʒm���󂯂Ă����ꍇ��Damage�m�[�h��
    //if (owner->GetIsDamaged())
     //�v���C���[�I�u�W�F�N�g���擾
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
    //HP���O�ȉ��ɂȂ�����Death�m�[�h��
    //if (owner->GetHp() <= 0)
    if(owner->GetComponent<Health>()->GetHpRate()<=0.0f)
    {
        return true;
    }
    return false;
}
