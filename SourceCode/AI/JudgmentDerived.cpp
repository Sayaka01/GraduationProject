#include "Stdafx.h"
#include "JudgmentDerived.h"

bool BattleJudgment::Judgment()
{
    //if (owner->GetIsPlayerAttack())//�v���C���[��T���Ă���ꍇ
    {   
        //�v���C���[��ǐՔ͈͓��Ō�������Battle�m�[�h��
        //if (owner->SerchPlayer(owner->GetPursuitRange()))
        {
            //�m�[�h�̏�������
            //actionNode->Enter();
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
        //�v���C���[�ƓG�̋��������ȉ��ɂȂ�����U���\
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

    //if (owner->GetIsPlayerAttack())//�v���C���[��T���Ă���ꍇ
    {
        //�v���C���[�ƓG�̋��������ȏ�ɂȂ�����p�j�\
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
    //�����ȊO�̏e�Ƃ̓����蔻��
    //if (BulletManager::Instance().HitCollisionObject(owner))
    //{
    //    return true;
    //}
    //�_���[�W�ʒm���󂯂Ă����ꍇ��Damage�m�[�h��
    //if (owner->GetIsDamaged())
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
    {
        return true;
    }
    return false;
}
