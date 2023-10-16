#include "Stdafx.h"
#include "ActionDerived.h"

#include "System/SystemManager.h"


// �Ō��s���̃A�N�V����
ActionBase::State PunchAction::Run()
{
    runTimer -= SystemManager::Instance().GetElapsedTime();

    //owner->SetState(Enemy::BehaviorState::Punch);

    // �U�����̎�̈ʒu���擾�A�v�Z��L����
    //owner->SetAttackPunchBone();
    //owner->GetModel()->SetDoCalcAttackBone(true);// �U���p�̈ʒu���v�Z����悤�Ƀt���O��ON

    // �U���pSphere��L���ɂ���
    //owner->SetValidAttackSphere(true);
    // �{�[���̖��O����U���̊�ʒu���擾���U���pSphere�̈ʒu���X�V
    //owner->SetAttackSpherePosition(owner->GetModel()->GetAttackBonePosition());
    // �p���`�G�t�F�N�g���Đ�
    //owner->PlayPunchEffect();

    //�s�����I�������؂�ւ�
    //if (owner->ActRun())
    if(runTimer < 0)
    {
        // �p���`�G�t�F�N�g���~
        //owner->StopPunchEffect();
        // �U���pSphere�𖳌���
        //owner->SetValidAttackSphere(false);
        // �U���p�̈ʒu���v�Z����߂�
        //owner->GetModel()->SetDoCalcAttackBone(false);
        return ActionBase::State::Complete;
    }
    return ActionBase::State::Running;
}

// �Ō��s���̏�������
void PunchAction::Enter()
{
    runTimer = 5.0f;
}

// �Ō��s���̏I������
void PunchAction::Exit()
{

}

// �d���s���̃A�N�V����
ActionBase::State SkillAction::Run()
{
    runTimer -= SystemManager::Instance().GetElapsedTime();

    //owner->SetState(Enemy::BehaviorState::Skill);

    // �X���b�V���A�j���[�V�����ɐݒ�
    //owner->ChangeIdleAnimation();//�����A�j���[�V�������Đ����悤�Ƃ���Ɠ���Ȃ�����idle�Ń��Z�b�g
    //owner->ChangeSkillAnimation();
    // �U�����̎�̈ʒu���擾�A�v�Z��L����
    //owner->SetAttackSkillBone();
    //owner->GetModel()->SetDoCalcAttackBone(true);// �U���p�̈ʒu���v�Z����悤�Ƀt���O��ON
    // �O�Ղ��o���ʒu���擾
    //owner->GetModel()->SetTrailName("RigLArm2");

    // �U���pSphere��L���ɂ���
    //owner->SetValidAttackSphere(true);
    // �p���`�G�t�F�N�g���~
    //owner->StopPunchEffect();
    // �{�[���̖��O����U���̊�ʒu���擾���U���pSphere�̈ʒu���X�V
    //owner->SetAttackSpherePosition(owner->GetModel()->GetAttackBonePosition());
    //�{�[���̖��O����U���̊�ʒu���擾
    //owner->AddTrailPos(owner->GetModel()->GetTrailHeadPosition(), owner->GetModel()->GetTrailTailPosition());
    //�A�j���[�V�������Đ����I�������A�N�V�����I��
    //if (!owner->GetModel()->IsPlayAnimation())
    if (runTimer <0)
    {
        //�U���pSphere�𖳌���
        //owner->SetValidAttackSphere(false);
        //�U���p�̈ʒu���v�Z����߂�
        //owner->GetModel()->SetDoCalcAttackBone(false);
        return ActionBase::State::Complete;
    }
    return ActionBase::State::Running;
}

// �d���s���̏�������
void SkillAction::Enter()
{
    runTimer = 5.0f;
}

// �d���s���̏I������
void SkillAction::Exit()
{
}

// �x�e�s���̃A�N�V����
ActionBase::State BreakAction::Run()
{
    runTimer -= SystemManager::Instance().GetElapsedTime();

    //float runTimer = owner->GetRunTimer();
        //owner->SetState(Enemy::BehaviorState::Break);
        // ���s���Ԃ������_��(1~2�b)�Ō��߂�
        //owner->SetRunTimer(1.5f);
        // �x�e�A�j���[�V�����ɐݒ�
        //owner->ChangeBreakAnimation();
        //if (owner->GetIsPlayerAttack())//�v���C���[��T���Ă���ꍇ
        //{
        //    //�v���C���[����G�̈ʒu�ւ̃x�N�g��
        //    DirectX::XMFLOAT3 vec = Math::SubAll(owner->GetPosition(), owner->GetPlayerPosition());
        //    vec = Math::Normalize(vec);
        //    float length = Math::RandomRange(2.0f, 10.0f);
        //    vec = Math::Scale(vec, length);// vec��length�{����

        //    owner->SetTargetPosition(Math::AddAll(owner->GetPosition(), vec));// �^�[�Q�b�g�̈ʒu��ݒ�
        //}

        //runTimer -= elapsedTime;
        //// �^�C�}�[�X�V
        //owner->SetRunTimer(runTimer);
        // ���s���Ԃ��o�߂�����A�N�V�����I��
        //if (runTimer <= 0.0f)
    if (runTimer < 0)
    {
        return ActionBase::State::Complete;
    }
    return ActionBase::State::Running;
}

// �x�e�s���̏�������
void BreakAction::Enter()
{
    runTimer = 5.0f;
}

// �x�e�s���̏I������
void BreakAction::Exit()
{

}

// �p�j�s���̃A�N�V����
ActionBase::State WanderAction::Run()
{
    runTimer -= SystemManager::Instance().GetElapsedTime();

    //owner->SetState(Enemy::BehaviorState::Wander);
    // �p�j�A�j���[�V�����ɐݒ�
    //owner->ChangeWanderAnimation();
    //�p�j���郉���_���̈ʒu���Z�o���邽�߂̊�ʒu�����̈ʒu�ɐݒ�
    //owner->SetReferencePointWander(owner->GetPosition());
    //�ړI�n�ֈړ�
    owner->MoveToTargetPosition(elapsedTime);
    // �ǐՔ͈͓��Ō�΂𔭌�������A�N�V�����I��
    //if (owner->SerchStone(owner->GetPursuitRange()))
    if (runTimer < 0)
    {
        return ActionBase::State::Complete;
    }
    return ActionBase::State::Running;
}

// �p�j�s���̏�������
void WanderAction::Enter()
{


    runTimer = 5.0f;
}

// �p�j�s���̏I������
void WanderAction::Exit()
{

}

// �ҋ@�m�[�h�̃A�N�V����
ActionBase::State IdleAction::Run()
{
    runTimer -= SystemManager::Instance().GetElapsedTime();

    //float runTimer = owner->GetRunTimer();
    //owner->SetState(Enemy::BehaviorState::Idle);
    // ���s���Ԃ������_��(3~5�b)�Ō��߂�
    //owner->SetRunTimer(Math::RandomRange(3.0f, 5.0f));
    // �ҋ@�A�j���[�V�����ɐݒ�
    //owner->ChangeIdleAnimation();
    //runTimer -= elapsedTime;
    //�^�C�}�[�X�V
    //owner->SetRunTimer(runTimer);
    //if (runTimer <= 0.0f)
    if (runTimer < 0)
    {
        return ActionBase::State::Complete;
    }
    return ActionBase::State::Running;
}

// �ǐՍs���̏�������
void IdleAction::Enter()
{
    runTimer = 5.0f;
}

// �ǐՍs���̏I������
void IdleAction::Exit()
{

}

// �ǐՍU���̃A�N�V����
ActionBase::State PursuitAction::Run()
{
    runTimer -= SystemManager::Instance().GetElapsedTime();

    //float runTimer = owner->GetRunTimer();
    //owner->SetState(Enemy::BehaviorState::Pursuit);

    // ���s���Ԃ������_��(3~5�b)�Ō��߂�
    //owner->SetRunTimer(Math::RandomRange(3.0f, 5.0f));
    // �ǐՃA�j���[�V�����ɐݒ�
    //owner->ChangePursuitAnimation();
    //runTimer -= elapsedTime;
    //�^�C�}�[�X�V
    //owner->SetRunTimer(runTimer);
    // ���s���Ԃ��o�߂�����A�N�V�����I��
    //if (runTimer <= 0.0f)
    if (runTimer < 0)
    {
        return ActionBase::State::Complete;
    }
    return ActionBase::State::Running;
}

// �ǐՍs���̏�������
void PursuitAction::Enter()
{
    runTimer = 5.0f;

}

// �ǐՍs���̏I������
void PursuitAction::Exit()
{

}

// �����s���̃A�N�V����
ActionBase::State EscapeAction::Run()
{
    runTimer -= SystemManager::Instance().GetElapsedTime();

    DirectX::XMFLOAT3 targetPosition;

    //owner->SetState(Enemy::BehaviorState::Leave);
    // �����A�j���[�V�����ɐݒ�
    //owner->ChangeLeaveAnimation();

    if (runTimer < 0)
    {
        return ActionBase::State::Complete;
    }

    return ActionBase::State::Running;
}

// �����s���̏�������
void EscapeAction::Enter()
{
    runTimer = 5.0f;

}

// �����s���̏I������
void EscapeAction::Exit()
{

}

// ���S�s���̃A�N�V����
ActionBase::State DieAction::Run()
{
    runTimer -= SystemManager::Instance().GetElapsedTime();

    //float runTimer = owner->GetRunTimer();
    //owner->SetState(Enemy::BehaviorState::Die);
    //owner->SetRunTimer(0.6f);
    // ���S�A�j���[�V�����ɐݒ�
    //owner->ChangeDieAnimation();
    // �p���`�G�t�F�N�g������Ă������~����
    //owner->StopPunchEffect();
    //owner->PlayDieEffect();
    //if (!owner->GetModel()->IsPlayAnimation())
    {
        //runTimer -= elapsedTime;
        //owner->SetRunTimer(runTimer);

        //if (runTimer <= 0.0f)
        if (runTimer < 0)
        {
            //owner->SetDieNotice(true);
            return ActionBase::State::Complete;
        }
    }
    return ActionBase::State::Running;
}

// ���S�s���̏�������
void DieAction::Enter()
{
    runTimer = 5.0f;

}

// ���S�s���̏I������
void DieAction::Exit()
{

}

// ��e�s���̃A�N�V����
ActionBase::State DamageAction::Run()
{
    runTimer -= SystemManager::Instance().GetElapsedTime();

    //owner->SetState(Enemy::BehaviorState::Damage);
    // ��e�A�j���[�V�����ɐݒ�
    //owner->ChangeDamageAnimation();
    //�A�j���[�V�������Đ����I������炨���
    //if (!owner->GetModel()->IsPlayAnimation())
    if (runTimer < 0)
    {
        return ActionBase::State::Complete;
    }
    return ActionBase::State::Running;
}

// ��e�s���̏�������
void DamageAction::Enter()
{
    runTimer = 5.0f;
}

// ��e�s���̏I������
void DamageAction::Exit()
{

}
