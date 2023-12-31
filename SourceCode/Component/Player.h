#pragma once

#include <DirectXMath.h>

#include "Component.h"

#include "PlayerStates.h"

class Player : public Component
{
public:
	Player() = default;
	~Player() override = default;

	//-----< 関数 >-----//

	//初期化
	void Initialize() override;
	//更新
	void Update() override;
	//終了処理
	void Finalize()override;
	//ImGui
	void DebugGui() override;

	void OnCollisionEnter(Collider* collider) override;

	//-----< Getter, Setter >-----//
	float GetHeight() { return height; }
	float GetRunSpeed() { return runSpeed; }
	float GetJumpSpeed() { return jumpSpeed; }
	float GetWireSpeed() { return wireSpeed; }

	float GetAttackPower() { return currentState->GetAttackPower(); }
	float GetAttackInterval() { return currentState->GetAttackInterval(); }

	void SetIsDead(bool b) { isDead = b; }
	bool GetIsDead() { return isDead; }

	bool GetIsHitAttackToEnemy() { return isHitAttackToEnemy; }
	void SetIsHitAttackToEnemy(bool b) { isHitAttackToEnemy = b; }

	bool GetIsAttack() { return isAttack; }
	void SetIsAttack(bool flag) { isAttack = flag; }

	int GetAttackPhase() { return attackPhase; }
	void SetAttackPhase(int phase) { attackPhase = phase; }
	void InitAttackPhase() { attackPhase = 0; }
	void NextAttackPhase() { attackPhase++; if (attackPhase >= maxAttackPhase)attackPhase = 0; }

	float GetAnimationSpeed(int index) { return animationSpeed[index]; }
	void SetAnimationSpeed(int index, float speed) { animationSpeed[index] = speed; }

	PlayerState::Default* GetCurrentState() { return currentState; }

	GameObject* GetThrowObj() { return throwObj; }
	void SetThrowObj(GameObject* obj) { throwObj = obj; }

private:

	//-----< 関数 >-----//
	void ChangeState(std::string nextStateName);

	//-----< 変数 >-----//
	std::vector<PlayerState::Default*> states;//状態
	PlayerState::Default* currentState = nullptr;

	float height = 6.0f;//プレイヤーの身長

	float runSpeed = 40.0f;//走る速さ
	float jumpSpeed = 50.0f;//ジャンプの高さ
	float knockBackSpeed = 10.0f;//ノックバックの強さ
	float wireSpeed = 100.0f;//ワイヤー移動のスピード

	bool isDead = false;//完全に死んでいるかどうか

	float stepDegree = 30.0f;

	bool isHitAttackToEnemy = false;//敵用の攻撃が当たっているかのフラグ

	bool isAttack = false;//プレイヤーが攻撃ステートかどうか

	int attackPhase = 0;//攻撃段階（何段目か）
	int maxAttackPhase = 3;

	float animationSpeed[(int)PlayerState::Animation::AnimNum];

	GameObject* throwObj = nullptr;//投げるオブジェクト
};