#include "Stdafx.h"
#include "ActionDerived.h"

#include "Component/Component.h"
#include "Component/Transform.h"
#include "GameObject/GameObject.h"
#include "Component/ModelRenderer.h"
#include "Component/SphereCollider.h"
#include <SimpleMath.h>
#include <Component/Health.h>

// 打撃行動のアクション
ActionBase::State PunchAction::Run(float elapsedTime)
{
    // アニメーションが再生し終わったら打撃行動を終了
    if (owner->GetParent()->GetComponent<ModelRenderer>()->IsFinishAnimation())
    {
        return ActionBase::State::Complete;
    }

    //runTimer -= SystemManager::Instance().GetElapsedTime();

    //owner->SetState(Enemy::BehaviorState::Punch);

    // 攻撃時の手の位置を取得、計算を有効化
    //owner->SetAttackPunchBone();
    //owner->GetModel()->SetDoCalcAttackBone(true);// 攻撃用の位置を計算するようにフラグをON

    // 攻撃用Sphereを有効にする
    //owner->SetValidAttackSphere(true);
    // ボーンの名前から攻撃の基準位置を取得し攻撃用Sphereの位置を更新
    //owner->SetAttackSpherePosition(owner->GetModel()->GetAttackBonePosition());
    // パンチエフェクトを再生
    //owner->PlayPunchEffect();

    //行動が終わったら切り替え
    //if (owner->ActRun())
    // パンチエフェクトを停止
    //owner->StopPunchEffect();
    // 攻撃用Sphereを無効化
    //owner->SetValidAttackSphere(false);
    // 攻撃用の位置を計算をやめる
    //owner->GetModel()->SetDoCalcAttackBone(false);
    return ActionBase::State::Running;
}

// 打撃行動の初期処理
void PunchAction::Enter()
{
    // 攻撃力の設定
    owner->SetAttackPower(owner->GetPunchAttackPower());

    // アニメーションの設定
    owner->ChangeAnimation(Enemy::AnimationName::LeftSlashAttack,false);
}

// 打撃行動の終了処理
void PunchAction::Exit()
{
}

// 重撃行動のアクション
ActionBase::State SkillAction::Run(float elapsedTime)
{
    // 骨の位置の取得
    DirectX::SimpleMath::Vector3 bonePos = owner->GetParent()->GetComponent<ModelRenderer>()->GetModelResource()->GetBonePositionFromName("rightHand");
    // sphereColliderの位置を設定
    owner->GetParent()->GetComponent<SphereCollider>("attackRightHand")->center = bonePos;


    // アニメーションが再生し終わったら重撃行動を終了
    if (owner->GetParent()->GetComponent<ModelRenderer>()->IsFinishAnimation())
    {
        return ActionBase::State::Complete;
    }

    //runTimer -= SystemManager::Instance().GetElapsedTime();

    //owner->SetState(Enemy::BehaviorState::Skill);

    // スラッシュアニメーションに設定
    //owner->ChangeIdleAnimation();//同じアニメーションを再生しようとすると入らないためidleでリセット
    //owner->ChangeSkillAnimation();
    // 攻撃時の手の位置を取得、計算を有効化
    //owner->SetAttackSkillBone();
    //owner->GetModel()->SetDoCalcAttackBone(true);// 攻撃用の位置を計算するようにフラグをON
    // 軌跡を出す位置を取得
    //owner->GetModel()->SetTrailName("RigLArm2");

    // 攻撃用Sphereを有効にする
    //owner->SetValidAttackSphere(true);
    // パンチエフェクトを停止
    //owner->StopPunchEffect();
    // ボーンの名前から攻撃の基準位置を取得し攻撃用Sphereの位置を更新
    //owner->SetAttackSpherePosition(owner->GetModel()->GetAttackBonePosition());
    //ボーンの名前から攻撃の基準位置を取得
    //owner->AddTrailPos(owner->GetModel()->GetTrailHeadPosition(), owner->GetModel()->GetTrailTailPosition());
    //アニメーションが再生し終わったらアクション終了
    //if (!owner->GetModel()->IsPlayAnimation())
    
    //攻撃用Sphereを無効化
    //owner->SetValidAttackSphere(false);
    //攻撃用の位置を計算をやめる
    //owner->GetModel()->SetDoCalcAttackBone(false);
    //return ActionBase::State::Complete;
    
    return ActionBase::State::Running;
}

// 重撃行動の初期処理
void SkillAction::Enter()
{
    // 攻撃力の設定
    owner->SetAttackPower(owner->GetSlashAttackPower());

    owner->ChangeAnimation(Enemy::AnimationName::RightClawAttack, false);
    
    // 攻撃する手の骨の位置を計算し当たり判定をONにする
    owner->GetParent()->GetComponent<ModelRenderer>()->GetModelResource()->GetBoneData("rightHand")->isCalc = true;
    owner->GetParent()->GetComponent<SphereCollider>("attackRightHand")->SetEnable(true);
}

// 重撃行動の終了処理
void SkillAction::Exit()
{
    owner->GetParent()->GetComponent<ModelRenderer>()->GetModelResource()->GetBoneData("rightHand")->isCalc = false;
    owner->GetParent()->GetComponent<SphereCollider>("attackRightHand")->SetEnable(false);
}

// 休憩行動のアクション
ActionBase::State BreakAction::Run(float elapsedTime)
{
    float runTimer = owner->GetRunTimer();

    // タイマー更新
    runTimer -= elapsedTime;
    owner->SetRunTimer(runTimer);

    // 実行時間が経過したらアクション終了
    if (runTimer <= 0.0f && owner->GetParent()->GetComponent<ModelRenderer>()->IsFinishAnimation())
    {
        return ActionBase::State::Complete;
    }
    return ActionBase::State::Running;
}

// 休憩行動の初期処理
void BreakAction::Enter()
{
    // 待機アニメーションを再生
    owner->ChangeAnimation(Enemy::AnimationName::Idle1, true);
    
    // 実行時間をランダム(1~2秒)で決める
    owner->SetRunTimer(Random::Range(1.0f,2.0f));

}

// 休憩行動の終了処理
void BreakAction::Exit()
{
}

// 徘徊行動のアクション
ActionBase::State WanderAction::Run(float elapsedTime)
{
    // 目的位置へ移動
    owner->MoveToTargetPosition(elapsedTime);
    //姿勢の回転
    owner->RotateTransform(elapsedTime);

    // 目的位置にある程度近づいたらアクション終了
    if(owner->GetLengthToTargetPosition()<1.0f)
    {
        return ActionBase::State::Complete;
    }
    return ActionBase::State::Running;
}

// 徘徊行動の初期処理
void WanderAction::Enter()
{
    // 目的位置をランダムに設定
    owner->SetTargetPositionRandom(owner->GetWanderRange());
    // 前進アニメーションを再生
    owner->ChangeAnimation(Enemy::AnimationName::DashForward, true);
}

// 徘徊行動の終了処理
void WanderAction::Exit()
{
}

// 待機行動のアクション
ActionBase::State IdleAction::Run(float elapsedTime)
{
    //タイマー更新
    float runTimer = owner->GetRunTimer();
    runTimer -= elapsedTime;
    owner->SetRunTimer(runTimer);

    // 実行時間が過ぎたら終了
    if (runTimer <= 0.0f)
    {
        return ActionBase::State::Complete;
    }
    return ActionBase::State::Running;
}

// 待機行動の初期処理
void IdleAction::Enter()
{
    owner->ChangeAnimation(Enemy::AnimationName::Idle2, true);
    // 実行時間をランダム(3~5秒)で決める
    owner->SetRunTimer(Random::Range(3.0f, 5.0f));
}

// 待機行動の終了処理
void IdleAction::Exit()
{
    owner->SetTargetPositionRandom(owner->GetWanderRange());
}

// 追跡攻撃のアクション
ActionBase::State PursuitAction::Run(float elapsedTime)
{
    //プレイヤーオブジェクトを取得
    GameObject* playerObj = owner->GetParent()->GetParent()->GetChild("player");
    if (playerObj != nullptr)
    {
        //プレイヤーの位置をターゲット位置に設定
        owner->SetTargetPosition(playerObj->GetComponent<Transform>()->pos);
    }

    //目的位置まで移動
    owner->MoveToTargetPosition(elapsedTime);

    //姿勢の回転
    owner->RotateTransform(elapsedTime);

    float length = owner->GetLengthToTargetPosition();
    // プレイヤーとの距離が攻撃範囲より小さくなったら終了
    if (length < owner->GetAttackRange()//距離が攻撃範囲より近づくとき
        || length > owner->GetPursuitRange())//距離が追跡範囲をでたとき
    {
        return ActionBase::State::Complete;
    }
    return ActionBase::State::Running;
}

// 追跡行動の初期処理
void PursuitAction::Enter()
{
    owner->ChangeAnimation(Enemy::AnimationName::DashForward, true);
}

// 追跡行動の終了処理
void PursuitAction::Exit()
{
}

// 逃走行動のアクション
ActionBase::State EscapeAction::Run(float elapsedTime)
{    
    // 目的位置とは逆方向へ進む
    owner->MoveToTargetPosition(-elapsedTime);
    //姿勢の回転
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

// 逃走行動の初期処理
void EscapeAction::Enter()
{
    owner->ChangeAnimation(Enemy::AnimationName::DashForward, true);
}

// 逃走行動の終了処理
void EscapeAction::Exit()
{
}

// 死亡行動のアクション
ActionBase::State DieAction::Run(float elapsedTime)
{
    float runTimer = owner->GetRunTimer();
    runTimer -= elapsedTime;
    owner->SetRunTimer(runTimer);
    //行動時間とアニメーションが終了している場合行動終了
    if (runTimer <= 0.0f /*&& owner->GetParent()->GetComponent<Animation>().IsPlayAnimation()*/)
    {
        return ActionBase::State::Complete;
    }
    return ActionBase::State::Running;
}

// 死亡行動の初期処理
void DieAction::Enter()
{
    owner->SetRunTimer(0.6f);
    owner->ChangeAnimation(Enemy::AnimationName::Die, false);
}

// 死亡行動の終了処理
void DieAction::Exit()
{
    // activeフラグを消す
    owner->GetParent()->SetActive(false);
}

// 被弾行動のアクション
ActionBase::State DamageAction::Run(float elapsedTime)
{
    // アニメーションが再生し終わったら被弾行動を終了
    if (owner->GetParent()->GetComponent<ModelRenderer>()->IsFinishAnimation())
    {
        return ActionBase::State::Complete;
    }
    return ActionBase::State::Running;
}

// 被弾行動の初期処理
void DamageAction::Enter()
{
    // HP処理
    owner->GetParent()->GetComponent<Health>()->SubtructHp(2.0f);

    owner->ChangeAnimation(Enemy::AnimationName::TakeDamege, false);
}

// 被弾行動の終了処理
void DamageAction::Exit()
{
}
