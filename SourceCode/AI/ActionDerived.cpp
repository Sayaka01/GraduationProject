#include "Stdafx.h"
#include "ActionDerived.h"

#include "Component/Component.h"
#include "Component/Transform.h"
#include "GameObject/GameObject.h"
#include "Component/ModelRenderer.h"
#include "Component/SphereCollider.h"
#include "Component/RigidBody.h"
#include <SimpleMath.h>
#include <Component/Health.h>
#include <Component/Player.h>

// �Ō��s���̃A�N�V����
ActionBase::State PunchAction::Run(float elapsedTime)
{
    //�v���C���[�I�u�W�F�N�g���擾
    GameObject* playerObj = owner->GetParent()->GetParent()->GetChild("player");
    if (playerObj != nullptr)
    {
        DirectX::SimpleMath::Vector3 vec = playerObj->GetComponent<Transform>()->pos - owner->GetComponent<Transform>()->pos;
        vec.Normalize();
        //�p���̉�]
        owner->GetComponent<Enemy>()->RotateTransform(vec, elapsedTime);
    }


    // ���̈ʒu�̎擾
    DirectX::SimpleMath::Vector3 bonePos = owner->GetComponent<ModelRenderer>()->GetBonePositionFromName
    ("leftHand" + std::to_string(owner->GetComponent<Enemy>()->GetOwnIndex()));
    // sphereCollider�̈ʒu��ݒ�
    owner->GetComponent<SphereCollider>("HandCollider")->center = bonePos;

    // �A�j���[�V�������Đ����I�������Ō��s�����I��
    if (owner->GetComponent<ModelRenderer>()->IsFinishAnimation())
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
    Enemy* enemy = owner->GetComponent<Enemy>();
    // �U���͂̐ݒ�
    enemy->SetAttackPower(enemy->GetPunchAttackPower());

    // �A�j���[�V�����̐ݒ�
    enemy->ChangeAnimation(Enemy::AnimationName::LeftSlashAttack,false);

    // �U�������̍��̈ʒu���v�Z�������蔻���ON�ɂ���
    owner->GetComponent<ModelRenderer>()->GetBoneData
    ("leftHand" + std::to_string(enemy->GetOwnIndex()))->isCalc = true;
    owner->GetComponent<SphereCollider>("HandCollider")->SetEnable(true);

    // Collider�̃^�C�v��"�U�ߔ���"�ɐݒ�
    owner->GetComponent<SphereCollider>("waist")->type= Collider::Type::Offense;

    //�X�e�[�g�̖��O��ݒ�
    enemy->SetStateName(Enemy::StateName::Punch);

}

// �Ō��s���̏I������
void PunchAction::Exit()
{
    // �U���͂̐ݒ�
    owner->GetComponent<Enemy>()->SetAttackPower(0);

    // �U�������̍��̈ʒu���v�Z�Ɠ����蔻���OFF�ɂ���
    owner->GetComponent<ModelRenderer>()->GetBoneData
    ("leftHand" + std::to_string(owner->GetComponent<Enemy>()->GetOwnIndex()))->isCalc = false;
    owner->GetComponent<SphereCollider>("HandCollider")->SetEnable(false);

    // Collider�̃^�C�v��"��蔻��"�ɐݒ�
    owner->GetComponent<SphereCollider>("waist")->type = Collider::Type::Deffense;
}

// �d���s���̃A�N�V����
ActionBase::State SkillAction::Run(float elapsedTime)
{
    // ���̈ʒu�̎擾
    DirectX::SimpleMath::Vector3 bonePos = owner->GetComponent<ModelRenderer>()->GetBonePositionFromName
    ("rightHand" + std::to_string(owner->GetComponent<Enemy>()->GetOwnIndex()));
    // sphereCollider�̈ʒu��ݒ�
    owner->GetComponent<SphereCollider>("HandCollider")->center = bonePos;

    //�v���C���[�I�u�W�F�N�g���擾
    GameObject* playerObj = owner->GetParent()->GetParent()->GetChild("player");
    if (playerObj != nullptr)
    {
        DirectX::SimpleMath::Vector3 vec = playerObj->GetComponent<Transform>()->pos - owner->GetComponent<Transform>()->pos;
        vec.Normalize();
        //�p���̉�]
        owner->GetComponent<Enemy>()->RotateTransform(vec, elapsedTime);
    }

    // �A�j���[�V�������Đ����I�������d���s�����I��
    if (owner->GetComponent<ModelRenderer>()->IsFinishAnimation())
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
    Enemy* enemy=owner->GetComponent<Enemy>();
    // �U���͂̐ݒ�
    enemy->SetAttackPower(enemy->GetSlashAttackPower());

    enemy->ChangeAnimation(Enemy::AnimationName::RightClawAttack, false);
    
    // �U�������̍��̈ʒu���v�Z�������蔻���ON�ɂ���
    owner->GetComponent<ModelRenderer>()->GetBoneData
    ("rightHand" + std::to_string(enemy->GetOwnIndex()))->isCalc = true;
    owner->GetComponent<SphereCollider>("HandCollider")->SetEnable(true);
    // Collider�̃^�C�v��"�U�ߔ���"�ɐݒ�
    owner->GetComponent<SphereCollider>("waist")->type = Collider::Type::Offense;

    //�X�e�[�g�̖��O��ݒ�
    enemy->SetStateName(Enemy::StateName::Slash);

}

// �d���s���̏I������
void SkillAction::Exit()
{
    // �U���͂̐ݒ�
    owner->GetComponent<Enemy>()->SetAttackPower(0);

    // �U�������̍��̈ʒu���v�Z�Ɠ����蔻���OFF�ɂ���
    owner->GetComponent<ModelRenderer>()->GetBoneData
    ("rightHand" + std::to_string(owner->GetComponent<Enemy>()->GetOwnIndex()))->isCalc = false;
    owner->GetComponent<SphereCollider>("HandCollider")->SetEnable(false);
    // Collider�̃^�C�v��"��蔻��"�ɐݒ�
    owner->GetComponent<SphereCollider>("waist")->type = Collider::Type::Deffense;
}

// �x�e�s���̃A�N�V����
ActionBase::State BreakAction::Run(float elapsedTime)
{
    Enemy* enemy = owner->GetComponent<Enemy>();
    float runTimer = enemy->GetRunTimer();

    // �^�C�}�[�X�V
    runTimer -= elapsedTime;
    enemy->SetRunTimer(runTimer);

    // ���s���Ԃ��o�߂�����A�N�V�����I��
    if (runTimer <= 0.0f && owner->GetComponent<ModelRenderer>()->IsFinishAnimation())
    {
        return ActionBase::State::Complete;
    }
    return ActionBase::State::Running;
}

// �x�e�s���̏�������
void BreakAction::Enter()
{
    Enemy* enemy= owner->GetComponent<Enemy>();
    // �ҋ@�A�j���[�V�������Đ�
    enemy->ChangeAnimation(Enemy::AnimationName::Idle1, true);
    
    // ���s���Ԃ������_��(1~2�b)�Ō��߂�
    enemy->SetRunTimer(Random::Range(0.1f,0.3f));
    //�X�e�[�g�̖��O��ݒ�
    enemy->SetStateName(Enemy::StateName::Break);

}

// �x�e�s���̏I������
void BreakAction::Exit()
{
}

// �p�j�s���̃A�N�V����
ActionBase::State WanderAction::Run(float elapsedTime)
{
    Enemy* enemy= owner->GetComponent<Enemy>();
    // �ړI�ʒu�ֈړ�
    enemy->MoveToTargetPosition(elapsedTime);
    //�p���̉�]
    enemy->RotateTransform(owner->GetComponent<RigidBody>()->GetVelocity(),elapsedTime);

    // �ړI�ʒu�ɂ�����x�߂Â�����A�N�V�����I��
    if(enemy->GetLengthToTargetPosition()<1.0f)
    {
        return ActionBase::State::Complete;
    }
    return ActionBase::State::Running;
}

// �p�j�s���̏�������
void WanderAction::Enter()
{
    Enemy* enemy= owner->GetComponent<Enemy>();
    // �ړI�ʒu�������_���ɐݒ�
    enemy->SetTargetPositionRandom(enemy->GetWanderRange());
    // �O�i�A�j���[�V�������Đ�
    enemy->ChangeAnimation(Enemy::AnimationName::DashForward, true);

    //�X�e�[�g�̖��O��ݒ�
    enemy->SetStateName(Enemy::StateName::Wander);

}

// �p�j�s���̏I������
void WanderAction::Exit()
{
}

// �ҋ@�s���̃A�N�V����
ActionBase::State IdleAction::Run(float elapsedTime)
{
    Enemy* enemy = owner->GetComponent<Enemy>();
    //�^�C�}�[�X�V
    float runTimer = enemy->GetRunTimer();
    runTimer -= elapsedTime;
    enemy->SetRunTimer(runTimer);

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
    Enemy* enemy = owner->GetComponent<Enemy>();
    enemy->ChangeAnimation(Enemy::AnimationName::Idle2, true);
    // ���s���Ԃ������_��(3~5�b)�Ō��߂�
    enemy->SetRunTimer(Random::Range(1.0f, 2.0f));
    //�X�e�[�g�̖��O��ݒ�
    enemy->SetStateName(Enemy::StateName::Idle);

}

// �ҋ@�s���̏I������
void IdleAction::Exit()
{
    Enemy* enemy = owner->GetComponent<Enemy>();
    enemy->SetTargetPositionRandom(enemy->GetWanderRange());
}

// �ǐՍU���̃A�N�V����
ActionBase::State PursuitAction::Run(float elapsedTime)
{
    Enemy* enemy = owner->GetComponent<Enemy>();
    //�v���C���[�I�u�W�F�N�g���擾
    GameObject* playerObj = owner->GetParent()->GetParent()->GetChild("player");
    if (playerObj != nullptr)
    {
        //�v���C���[�̈ʒu���^�[�Q�b�g�ʒu�ɐݒ�
        enemy->SetTargetPosition(playerObj->GetComponent<Transform>()->pos);
    }

    //�ړI�ʒu�܂ňړ�
    enemy->MoveToTargetPosition(elapsedTime);

    //�p���̉�]
    enemy->RotateTransform(owner->GetComponent<RigidBody>()->GetVelocity(), elapsedTime);

    float length = enemy->GetLengthToTargetPosition();
    // �v���C���[�Ƃ̋������U���͈͂�菬�����Ȃ�����I��
    if (length < enemy->GetAttackRange()//�������U���͈͂��߂Â��Ƃ�
        || length > enemy->GetPursuitRange())//�������ǐՔ͈͂��ł��Ƃ�
    {
        return ActionBase::State::Complete;
    }
    return ActionBase::State::Running;
}

// �ǐՍs���̏�������
void PursuitAction::Enter()
{
    Enemy* enemy = owner->GetComponent<Enemy>();

    enemy->ChangeAnimation(Enemy::AnimationName::DashForward, true);
    //�X�e�[�g�̖��O��ݒ�
    enemy->SetStateName(Enemy::StateName::Pursuit);

}

// �ǐՍs���̏I������
void PursuitAction::Exit()
{
}

// �����s���̃A�N�V����
ActionBase::State EscapeAction::Run(float elapsedTime)
{    
    Enemy* enemy = owner->GetComponent<Enemy>();
    // �ړI�ʒu�Ƃ͋t�����֐i��
    enemy->MoveToTargetPosition(-elapsedTime);
    //�p���̉�]
    enemy->RotateTransform(owner->GetComponent<RigidBody>()->GetVelocity(), elapsedTime);

    float runTimer = enemy->GetRunTimer();
    runTimer -= elapsedTime;
    enemy->SetRunTimer(runTimer);

    if (runTimer < 0 || enemy->GetLengthToTargetPosition()>5.0f)
    {
        return ActionBase::State::Complete;
    }

    return ActionBase::State::Running;
}

// �����s���̏�������
void EscapeAction::Enter()
{
    Enemy* enemy = owner->GetComponent<Enemy>();

    enemy->ChangeAnimation(Enemy::AnimationName::DashForward, true);
    //�X�e�[�g�̖��O��ݒ�
    enemy->SetStateName(Enemy::StateName::Escape);
}

// �����s���̏I������
void EscapeAction::Exit()
{
}

// ���S�s���̃A�N�V����
ActionBase::State DieAction::Run(float elapsedTime)
{
    Enemy* enemy = owner->GetComponent<Enemy>();
    float runTimer = enemy->GetRunTimer();
    runTimer -= elapsedTime;
    enemy->SetRunTimer(runTimer);
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
    Enemy* enemy= owner->GetComponent<Enemy>();
    enemy->SetRunTimer(2.0f);
    enemy->ChangeAnimation(Enemy::AnimationName::Die, false);
    //�X�e�[�g�̖��O��ݒ�
    enemy->SetStateName(Enemy::StateName::Die);

}

// ���S�s���̏I������
void DieAction::Exit()
{
    // active�t���O������
    owner->SetActive(false);
}

// ��e�s���̃A�N�V����
ActionBase::State DamageAction::Run(float elapsedTime)
{
    Enemy* enemy = owner->GetComponent<Enemy>();
    float runTimer = enemy->GetRunTimer();
    runTimer -= elapsedTime;
    enemy->SetRunTimer(runTimer);

    // �A�j���[�V�������Đ����I��������e�s�����I��
    if (owner->GetComponent<ModelRenderer>()->IsFinishAnimation())
    {
        return ActionBase::State::Complete;
    }

    //�s�����Ԃ������̏ꍇ�m�b�N�o�b�N���s��Ȃ�
    if (runTimer <= 0) return ActionBase::State::Running;
       
    // �v���C���[�I�u�W�F�N�g���擾
    GameObject* playerObj = owner->GetParent()->GetParent()->GetChild("player");

    //�v���C���[��������Ȃ��ꍇ�m�b�N�o�b�N���s��Ȃ�
    if (playerObj == nullptr) return ActionBase::State::Running;

    // �m�b�N�o�b�N�̋���
    float knockPower = 15.0f;
    // �v���C���[�̈ʒu���玩���̈ʒu�̃x�N�g�����v�Z
    DirectX::SimpleMath::Vector3 knockBackVec = owner->GetComponent<Transform>()->pos - playerObj->GetComponent<Transform>()->pos;
    knockBackVec.Normalize();//���K��
    // �m�b�N�o�b�N�Ō��֒���
    owner->GetComponent<RigidBody>()->AddVelocity(knockBackVec * knockPower);

    return ActionBase::State::Running;
}

// ��e�s���̏�������
void DamageAction::Enter()
{
    GameObject* playerObj = owner->GetParent()->GetParent()->GetChild("player");
    if (playerObj != nullptr)
    {
        // �v���C���[�̌��݂̍U���͂��擾
        float ap = playerObj->GetComponent<Player>()->GetAttackPower();
        // HP����
        owner->GetComponent<Health>()->SubtructHp(ap);
    }

    Enemy* enemy = owner->GetComponent<Enemy>();
    //�X�e�[�g�̖��O��ݒ�
    enemy->SetStateName(Enemy::StateName::Damage);

    enemy->SetRunTimer(0.3f);

    enemy->ChangeAnimation(Enemy::AnimationName::TakeDamege, false);
}

// ��e�s���̏I������
void DamageAction::Exit()
{
}
