#include "Stdafx.h"
#include "ActionDerived.h"


// 打撃行動のアクション
ActionBase::State PunchAction::Run()
{
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
    if (owner->ActRun())
    {
        // パンチエフェクトを停止
        //owner->StopPunchEffect();
        // 攻撃用Sphereを無効化
        //owner->SetValidAttackSphere(false);
        // 攻撃用の位置を計算をやめる
        //owner->GetModel()->SetDoCalcAttackBone(false);
        return ActionBase::State::Complete;
    }
    return ActionBase::State::Running;
}

// 打撃行動の初期処理
void PunchAction::Enter()
{
}

// 打撃行動の終了処理
void PunchAction::Exit()
{
}

// 重撃行動のアクション
ActionBase::State SkillAction::Run()
{
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
    {
        //攻撃用Sphereを無効化
        //owner->SetValidAttackSphere(false);
        //攻撃用の位置を計算をやめる
        //owner->GetModel()->SetDoCalcAttackBone(false);
        return ActionBase::State::Complete;
    }
    return ActionBase::State::Running;
}

// 重撃行動の初期処理
void SkillAction::Enter()
{
}

// 重撃行動の終了処理
void SkillAction::Exit()
{
}

// 休憩行動のアクション
ActionBase::State BreakAction::Run()
{
    //float runTimer = owner->GetRunTimer();
        //owner->SetState(Enemy::BehaviorState::Break);
        // 実行時間をランダム(1~2秒)で決める
        //owner->SetRunTimer(1.5f);
        // 休憩アニメーションに設定
        //owner->ChangeBreakAnimation();
        //if (owner->GetIsPlayerAttack())//プレイヤーを探している場合
        //{
        //    //プレイヤーから敵の位置へのベクトル
        //    DirectX::XMFLOAT3 vec = Math::SubAll(owner->GetPosition(), owner->GetPlayerPosition());
        //    vec = Math::Normalize(vec);
        //    float length = Math::RandomRange(2.0f, 10.0f);
        //    vec = Math::Scale(vec, length);// vecをlength倍する

        //    owner->SetTargetPosition(Math::AddAll(owner->GetPosition(), vec));// ターゲットの位置を設定
        //}

        //runTimer -= elapsedTime;
        //// タイマー更新
        //owner->SetRunTimer(runTimer);
        // 実行時間が経過したらアクション終了
        //if (runTimer <= 0.0f)
    {
        return ActionBase::State::Complete;
    }
    return ActionBase::State::Running;
}

// 休憩行動の初期処理
void BreakAction::Enter()
{
}

// 休憩行動の終了処理
void BreakAction::Exit()
{
}

// 徘徊行動のアクション
ActionBase::State WanderAction::Run()
{
    //owner->SetState(Enemy::BehaviorState::Wander);
    // 徘徊アニメーションに設定
    //owner->ChangeWanderAnimation();
    //徘徊するランダムの位置を算出するための基準位置を今の位置に設定
    //owner->SetReferencePointWander(owner->GetPosition());
    //目的地へ移動
    //owner->MoveToTargetPosition(elapsedTime);
    // 追跡範囲内で護石を発見したらアクション終了
    //if (owner->SerchStone(owner->GetPursuitRange()))
    {
        return ActionBase::State::Complete;
    }
    return ActionBase::State::Running;
}

// 徘徊行動の初期処理
void WanderAction::Enter()
{
}

// 徘徊行動の終了処理
void WanderAction::Exit()
{
}

// 待機ノードのアクション
ActionBase::State IdleAction::Run()
{
    //float runTimer = owner->GetRunTimer();
    //owner->SetState(Enemy::BehaviorState::Idle);
    // 実行時間をランダム(3~5秒)で決める
    //owner->SetRunTimer(Math::RandomRange(3.0f, 5.0f));
    // 待機アニメーションに設定
    //owner->ChangeIdleAnimation();
    //runTimer -= elapsedTime;
    //タイマー更新
    //owner->SetRunTimer(runTimer);
    //if (runTimer <= 0.0f)
    {
        return ActionBase::State::Complete;
    }
    return ActionBase::State::Running;
}

// 追跡行動の初期処理
void IdleAction::Enter()
{
}

// 追跡行動の終了処理
void IdleAction::Exit()
{
}

// 追跡攻撃のアクション
ActionBase::State PursuitAction::Run()
{
    //float runTimer = owner->GetRunTimer();
    //owner->SetState(Enemy::BehaviorState::Pursuit);

    // 実行時間をランダム(3~5秒)で決める
    //owner->SetRunTimer(Math::RandomRange(3.0f, 5.0f));
    // 追跡アニメーションに設定
    //owner->ChangePursuitAnimation();
    //runTimer -= elapsedTime;
    //タイマー更新
    //owner->SetRunTimer(runTimer);
    // 実行時間が経過したらアクション終了
    //if (runTimer <= 0.0f)
    {
        return ActionBase::State::Complete;
    }
    return ActionBase::State::Running;
}

// 追跡行動の初期処理
void PursuitAction::Enter()
{
}

// 追跡行動の終了処理
void PursuitAction::Exit()
{
}

// 逃走行動のアクション
ActionBase::State EscapeAction::Run()
{
    DirectX::XMFLOAT3 targetPosition;

    //owner->SetState(Enemy::BehaviorState::Leave);
    // 離れるアニメーションに設定
    //owner->ChangeLeaveAnimation();

    {
        return ActionBase::State::Complete;
    }

    return ActionBase::State::Running;
}

// 逃走行動の初期処理
void EscapeAction::Enter()
{
}

// 逃走行動の終了処理
void EscapeAction::Exit()
{
}

// 死亡行動のアクション
ActionBase::State DieAction::Run()
{
    //float runTimer = owner->GetRunTimer();
    //owner->SetState(Enemy::BehaviorState::Die);
    //owner->SetRunTimer(0.6f);
    // 死亡アニメーションに設定
    //owner->ChangeDieAnimation();
    // パンチエフェクトが流れていたら停止する
    //owner->StopPunchEffect();
    //owner->PlayDieEffect();
    //if (!owner->GetModel()->IsPlayAnimation())
    {
        //runTimer -= elapsedTime;
        //owner->SetRunTimer(runTimer);

        //if (runTimer <= 0.0f)
        {
            //owner->SetDieNotice(true);
            return ActionBase::State::Complete;
        }
    }
    return ActionBase::State::Running;
}

// 死亡行動の初期処理
void DieAction::Enter()
{
}

// 死亡行動の終了処理
void DieAction::Exit()
{
}

// 被弾行動のアクション
ActionBase::State DamageAction::Run()
{
    //owner->SetState(Enemy::BehaviorState::Damage);
    // 被弾アニメーションに設定
    //owner->ChangeDamageAnimation();
    //アニメーションが再生し終わったらおわり
    //if (!owner->GetModel()->IsPlayAnimation())
    {
        return ActionBase::State::Complete;
    }
    return ActionBase::State::Running;
}

// 被弾行動の初期処理
void DamageAction::Enter()
{
}

// 被弾行動の終了処理
void DamageAction::Exit()
{
}
