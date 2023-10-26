#include "Stdafx.h"
#include "JudgmentDerived.h"

bool BattleJudgment::Judgment()
{    
    //�v���C���[�I�u�W�F�N�g���擾
    GameObject* playerObj = owner->GetParent()->GetParent()->GetChild("player");
    if (playerObj != nullptr)
    {
        //�v���C���[�Ƃ̋���
        DirectX::SimpleMath::Vector3 vector = playerObj->GetComponent<Transform>()->pos - owner->GetParent()->GetComponent<Transform>()->pos;
        float length = vector.Length();

        //�v���C���[�ƓG�̋��������ȉ��ɂȂ�����U���\
        if (length < owner->GetPursuitRange())//�U���͈͂ɓ����Ă���ꍇ
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
        //�v���C���[�Ƃ̋���
        DirectX::SimpleMath::Vector3 vector = playerObj->GetComponent<Transform>()->pos - owner->GetParent()->GetComponent<Transform>()->pos;
        float length = vector.Length();

        //�v���C���[�ƓG�̋��������ȉ��ɂȂ�����U���\
        if (length < owner->GetAttackRange())
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
        DirectX::SimpleMath::Vector3 vector = playerObj->GetComponent<Transform>()->pos - owner->GetParent()->GetComponent<Transform>()->pos;
        float length = vector.Length();

        //�v���C���[�ƓG�̋��������ȏ�ɂȂ�����p�j
        if (length > owner->GetPursuitRange() && owner->GetLengthToTargetPosition() >= 1.5f)
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
        //return true;
    }
    return false;
}

bool DeathJudgment::Judgment()
{
    //HP���O�ȉ��ɂȂ�����Death�m�[�h��
    //if (owner->GetHp() <= 0)
    {
        //return true;
    }
    return false;
}
