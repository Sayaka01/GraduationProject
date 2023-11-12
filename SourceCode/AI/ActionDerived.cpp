#include "Stdafx.h"
#include "ActionDerived.h"

#include "Component/Component.h"
#include "Component/Transform.h"
#include "GameObject/GameObject.h"
#include "Component/ModelRenderer.h"
#include "Component/SphereCollider.h"
#include <SimpleMath.h>
#include <Component/Health.h>

// �Ō��s���̃A�N�V����
ActionBase::State PunchAction::Run(float elapsedTime)
{
    // �A�j���[�V�������Đ����I�������Ō��s�����I��
    if (owner->GetParent()->GetComponent<ModelRenderer>()->IsFinishAnimation())
    {
        return ActionBase::State::Complete;
    }

    //runTimer -= SystemManager::Instance().GetElapsedTime();

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
    // �p���`�G�t�F�N�g���~
    //owner->StopPunchEffect();
    // �U���pSphere�𖳌���
    //owner->SetValidAttackSphere(false);
    // �U���p�̈ʒu���v�Z����߂�
    //owner->GetModel()->SetDoCalcAttackBone(false);
    return ActionBase::State::Running;
}

// �Ō��s���̏�������
void PunchAction::Enter()
{
    // �U���͂̐ݒ�
    owner->SetAttackPower(owner->GetPunchAttackPower());

    // �A�j���[�V�����̐ݒ�
    owner->ChangeAnimation(Enemy::AnimationName::LeftSlashAttack,false);
}

// �Ō��s���̏I������
void PunchAction::Exit()
{
}

// �d���s���̃A�N�V����
ActionBase::State SkillAction::Run(float elapsedTime)
{
    // ���̈ʒu�̎擾
    DirectX::SimpleMath::Vector3 bonePos = owner->GetParent()->GetComponent<ModelRenderer>()->GetModelResource()->GetBonePositionFromName("rightHand");
    // sphereCollider�̈ʒu��ݒ�
    owner->GetParent()->GetComponent<SphereCollider>("attackRightHand")->center = bonePos;


    // �A�j���[�V�������Đ����I�������d���s�����I��
    if (owner->GetParent()->GetComponent<ModelRenderer>()->IsFinishAnimation())
    {
        return ActionBase::State::Complete;
    }

    //runTimer -= SystemManager::Instance().GetElapsedTime();

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
    
    //�U���pSphere�𖳌���
    //owner->SetValidAttackSphere(false);
    //�U���p�̈ʒu���v�Z����߂�
    //owner->GetModel()->SetDoCalcAttackBone(false);
    //return ActionBase::State::Complete;
    
    return ActionBase::State::Running;
}

// �d���s���̏�������
void SkillAction::Enter()
{
    // �U���͂̐ݒ�
    owner->SetAttackPower(owner->GetSlashAttackPower());

    owner->ChangeAnimation(Enemy::AnimationName::RightClawAttack, false);
    
    // �U�������̍��̈ʒu���v�Z�������蔻���ON�ɂ���
    owner->GetParent()->GetComponent<ModelRenderer>()->GetModelResource()->GetBoneData("rightHand")->isCalc = true;
    owner->GetParent()->GetComponent<SphereCollider>("attackRightHand")->SetEnable(true);
}

// �d���s���̏I������
void SkillAction::Exit()
{
    owner->GetParent()->GetComponent<ModelRenderer>()->GetModelResource()->GetBoneData("rightHand")->isCalc = false;
    owner->GetParent()->GetComponent<SphereCollider>("attackRightHand")->SetEnable(false);
}

// �x�e�s���̃A�N�V����
ActionBase::State BreakAction::Run(float elapsedTime)
{
    float runTimer = owner->GetRunTimer();

    // �^�C�}�[�X�V
    runTimer -= elapsedTime;
    owner->SetRunTimer(runTimer);

    // ���s���Ԃ��o�߂�����A�N�V�����I��
    if (runTimer <= 0.0f && owner->GetParent()->GetComponent<ModelRenderer>()->IsFinishAnimation())
    {
        return ActionBase::State::Complete;
    }
    return ActionBase::State::Running;
}

// �x�e�s���̏�������
void BreakAction::Enter()
{
    // �ҋ@�A�j���[�V�������Đ�
    owner->ChangeAnimation(Enemy::AnimationName::Idle1, true);
    
    // ���s���Ԃ������_��(1~2�b)�Ō��߂�
    owner->SetRunTimer(Random::Range(1.0f,2.0f));

}

// �x�e�s���̏I������
void BreakAction::Exit()
{
}

// �p�j�s���̃A�N�V����
ActionBase::State WanderAction::Run(float elapsedTime)
{
    // �ړI�ʒu�ֈړ�
    owner->MoveToTargetPosition(elapsedTime);
    //�p���̉�]
    owner->RotateTransform(elapsedTime);

    // �ړI�ʒu�ɂ�����x�߂Â�����A�N�V�����I��
    if(owner->GetLengthToTargetPosition()<1.0f)
    {
        return ActionBase::State::Complete;
    }
    return ActionBase::State::Running;
}

// �p�j�s���̏�������
void WanderAction::Enter()
{
    // �ړI�ʒu�������_���ɐݒ�
    owner->SetTargetPositionRandom(owner->GetWanderRange());
    // �O�i�A�j���[�V�������Đ�
    owner->ChangeAnimation(Enemy::AnimationName::DashForward, true);
}

// �p�j�s���̏I������
void WanderAction::Exit()
{
}

// �ҋ@�s���̃A�N�V����
ActionBase::State IdleAction::Run(float elapsedTime)
{
    //�^�C�}�[�X�V
    float runTimer = owner->GetRunTimer();
    runTimer -= elapsedTime;
    owner->SetRunTimer(runTimer);

    // ���s���Ԃ��߂�����I��
    if (runTimer <= 0.0f)
    {
        return ActionBase::State::Complete;
    }
    return ActionBase::State::Running;
}

// �ҋ@�s���̏�������
void IdleAction::Enter()
{
    owner->ChangeAnimation(Enemy::AnimationName::Idle2, true);
    // ���s���Ԃ������_��(3~5�b)�Ō��߂�
    owner->SetRunTimer(Random::Range(3.0f, 5.0f));
}

// �ҋ@�s���̏I������
void IdleAction::Exit()
{
    owner->SetTargetPositionRandom(owner->GetWanderRange());
}

// �ǐՍU���̃A�N�V����
ActionBase::State PursuitAction::Run(float elapsedTime)
{
    //�v���C���[�I�u�W�F�N�g���擾
    GameObject* playerObj = owner->GetParent()->GetParent()->GetChild("player");
    if (playerObj != nullptr)
    {
        //�v���C���[�̈ʒu���^�[�Q�b�g�ʒu�ɐݒ�
        owner->SetTargetPosition(playerObj->GetComponent<Transform>()->pos);
    }

    //�ړI�ʒu�܂ňړ�
    owner->MoveToTargetPosition(elapsedTime);

    //�p���̉�]
    owner->RotateTransform(elapsedTime);

    float length = owner->GetLengthToTargetPosition();
    // �v���C���[�Ƃ̋������U���͈͂�菬�����Ȃ�����I��
    if (length < owner->GetAttackRange()//�������U���͈͂��߂Â��Ƃ�
        || length > owner->GetPursuitRange())//�������ǐՔ͈͂��ł��Ƃ�
    {
        return ActionBase::State::Complete;
    }
    return ActionBase::State::Running;
}

// �ǐՍs���̏�������
void PursuitAction::Enter()
{
    owner->ChangeAnimation(Enemy::AnimationName::DashForward, true);
}

// �ǐՍs���̏I������
void PursuitAction::Exit()
{
}

// �����s���̃A�N�V����
ActionBase::State EscapeAction::Run(float elapsedTime)
{    
    // �ړI�ʒu�Ƃ͋t�����֐i��
    owner->MoveToTargetPosition(-elapsedTime);
    //�p���̉�]
    owner->RotateTransform(elapsedTime);


    float runTimer = owner->GetRunTimer();
    runTimer -= elapsedTime;
    owner->SetRunTimer(runTimer);

    if (runTimer < 0 || owner->GetLengthToTargetPosition()>5.0f)
    {
        return ActionBase::State::Complete;
    }

    return ActionBase::State::Running;
}

// �����s���̏�������
void EscapeAction::Enter()
{
    owner->ChangeAnimation(Enemy::AnimationName::DashForward, true);
}

// �����s���̏I������
void EscapeAction::Exit()
{
}

// ���S�s���̃A�N�V����
ActionBase::State DieAction::Run(float elapsedTime)
{
    float runTimer = owner->GetRunTimer();
    runTimer -= elapsedTime;
    owner->SetRunTimer(runTimer);
    //�s�����ԂƃA�j���[�V�������I�����Ă���ꍇ�s���I��
    if (runTimer <= 0.0f /*&& owner->GetParent()->GetComponent<Animation>().IsPlayAnimation()*/)
    {
        return ActionBase::State::Complete;
    }
    return ActionBase::State::Running;
}

// ���S�s���̏�������
void DieAction::Enter()
{
    owner->SetRunTimer(0.6f);
    owner->ChangeAnimation(Enemy::AnimationName::Die, false);
}

// ���S�s���̏I������
void DieAction::Exit()
{
    // active�t���O������
    owner->GetParent()->SetActive(false);
}

// ��e�s���̃A�N�V����
ActionBase::State DamageAction::Run(float elapsedTime)
{
    // �A�j���[�V�������Đ����I��������e�s�����I��
    if (owner->GetParent()->GetComponent<ModelRenderer>()->IsFinishAnimation())
    {
        return ActionBase::State::Complete;
    }
    return ActionBase::State::Running;
}

// ��e�s���̏�������
void DamageAction::Enter()
{
    // HP����
    owner->GetParent()->GetComponent<Health>()->SubtructHp(2.0f);

    owner->ChangeAnimation(Enemy::AnimationName::TakeDamege, false);
}

// ��e�s���̏I������
void DamageAction::Exit()
{
}
