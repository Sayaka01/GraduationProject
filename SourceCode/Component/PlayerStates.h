#pragma once

#include "SimpleMath.h"

class GameObject;
class Transform;
#include "Enemy.h"

//プレイヤーの状態
namespace PlayerState
{
	//アニメーション
	enum class Animation
	{
		Idle,//待機
		Walking,//歩き
		Running,//走り

		Jump,//ジャンプ
		Falling,//落下
		Landing,//着地
		JumpFlip,//2段ジャンプ（空中回避）

		Avoid,//回避

		PunchRight,//右パンチ
		PunchLeft,//左パンチ
		Kick,//キック	
		JumpPunch,//空中攻撃

		Damage,//ダメージ
		Death,//死亡

		DangleWire,//ワイヤーぶら下がり

		Thrust,//突きさす
		Wield,//振り回す
		Throw,//投げる

		AnimNum,
	};

	
	//基底クラス
	class Default
	{
	public:
		Default() = default;
		virtual ~Default() = default;
		
		//ステート遷移時の処理
		virtual void Enter() {}
		//更新
		virtual void Update();
		//終了処理
		virtual void Exit() {}
		//ステート変更するかどうか
		virtual std::string GetNext() { return ""; }
		//ImGui
		virtual void DebugGui() {}

		//名前の取得
		std::string GetName() { return name; }

		//攻撃力の取得
		float GetAttackPower() { return attackPower; }

		//攻撃間隔の取得
		float GetAttackInterval() { return attackInterval; }

		//パラメーターの設定
		void SetParameter(DirectX::XMFLOAT3 param) { parameter = param; }

		//ジャンプ回数の初期化
		void InitJumpCount() { jumpCount = 0; }

		//敵の攻撃被弾フラグを全てOFFに
		void SetClearEnemyHitFlag();

	protected:
		//移動ベクトルをRigidBodyコンポーネントに保存
		void SetMoveVelocity(DirectX::XMFLOAT3 velocity);
		//移動ベクトルをRigidBodyコンポーネントに追加
		void AddMoveVelocity(DirectX::XMFLOAT3 velocity);
		//カメラの前方向を取得
		DirectX::XMFLOAT3 GetCameraFront();
		//カメラの右方向を取得
		DirectX::XMFLOAT3 GetCameraRight();
		//移動ベクトルを計算
		DirectX::XMFLOAT3 CalcMoveVec();
		//コントローラーのLスティックの入力値を取得
		DirectX::XMFLOAT2 GetLStickVec();
		//移動ベクトルに応じたY軸回転
		void YAxisRotate(DirectX::XMFLOAT3 moveVelocity);
		//一番近い敵を取得
#if _APPEND
		void SearchNearEnemy(Transform* transform = nullptr);
#endif
		//アニメーションスピードを取得
		float GetAnimationSpeed(int index);
		//アニメーションスピードの設定
		void SetAnimationSpeed(int index, float speed);

		//待機ステートの遷移条件
		bool JudgeIdleState();
		//走りステートの遷移条件
		bool JudgeRunState();
		//ジャンプステートの遷移条件
		bool JudgeJumpState();
		//攻撃ステートの遷移条件
		bool JudgeAttackState();
		//ワイヤーでの直線移動ステートの遷移条件
		bool JudgeAimWireState();
		//ワイヤーでの弧を書いた移動ステートの遷移条件
		bool JudgeSwingWireState();
		//回避ステートの遷移条件
		bool JudgeAvoidState();
		//振り回して投げるステートの遷移条件
		bool JudgeWieldThrowState();

		//振り回すオブジェクトの検索
		void SearchThrowObj();


		std::string name = "";//ステートの名前

		GameObject* parent = nullptr;//親オブジェクト

		float rotateSpeed = DirectX::XMConvertToRadians(1440);
		float rotateRatio = 0.75f;

		static float attackInterval;
		float acceptAttackTime = 1.5f;

		static bool acceptAttackButton;
		static bool pushAttackButton;

		static int jumpCount;
		const int maxJumpCount = 2;

		float attackPower = 0.0f;

		static DirectX::XMFLOAT3 parameter;//凡庸パラメータ

		static bool isAvoid;//回避したかどうか

		float searchRange = 70.0f;

	};

	//待機
	class Idle : public Default
	{
	public:
		Idle();
		Idle(GameObject* parent);
		~Idle() = default;

		//ステート遷移時の処理
		void Enter()override;
		//更新
		void Update()override;
		//終了処理
		void Exit()override;
		//ステート変更するかどうか
		std::string GetNext()override;
	};

	//走り
	class Run : public Default
	{
	public:
		Run();
		Run(GameObject* parent);
		~Run() = default;

		//ステート遷移時の処理
		void Enter()override;
		//更新
		void Update()override;
		//終了処理
		void Exit()override;
		//ステート変更するかどうか
		std::string GetNext()override;
	};

	//ジャンプ
	class Jump : public Default
	{
	public:
		Jump();
		Jump(GameObject* parent);
		~Jump() = default;

		//ステート遷移時の処理
		void Enter()override;
		//更新
		void Update()override;
		//終了処理
		void Exit()override;
		//ステート変更するかどうか
		std::string GetNext()override;
	};

	//落下
	class Falling : public Default
	{
	public:
		Falling();
		Falling(GameObject* parent);
		~Falling() = default;

		//ステート遷移時の処理
		void Enter()override;
		//更新
		void Update()override;
		//終了処理
		void Exit()override;
		//ステート変更するかどうか
		std::string GetNext()override;
	};

	//着地
	class Landing : public Default
	{
	public:
		Landing();
		Landing(GameObject* parent);
		~Landing() = default;

		//ステート遷移時の処理
		void Enter()override;
		//更新
		void Update()override;
		//終了処理
		void Exit()override;
		//ステート変更するかどうか
		std::string GetNext()override;
	};

	//パンチ（右）
	class PunchRight : public Default
	{
	public:
		PunchRight();
		PunchRight(GameObject* parent);
		~PunchRight() = default;

		//ステート遷移時の処理
		void Enter()override;
		//更新
		void Update()override;
		//終了処理
		void Exit()override;
		//ステート変更するかどうか
		std::string GetNext()override;

	private:
		float attackRangeMin = 8.0f;
		float attackRangeMax = 25.0f;
	};

	//パンチ（左）
	class PunchLeft : public Default
	{
	public:
		PunchLeft();
		PunchLeft(GameObject* parent);
		~PunchLeft() = default;

		//ステート遷移時の処理
		void Enter()override;
		//更新
		void Update()override;
		//終了処理
		void Exit()override;
		//ステート変更するかどうか
		std::string GetNext()override;
	private:
		float attackRangeMin = 8.0f;
		float attackRangeMax = 25.0f;
	};

	//キック
	class Kick : public Default
	{
	public:
		Kick();
		Kick(GameObject* parent);
		~Kick() = default;

		//ステート遷移時の処理
		void Enter()override;
		//更新
		void Update()override;
		//終了処理
		void Exit()override;
		//ステート変更するかどうか
		std::string GetNext()override;
	private:
		float attackRangeMin = 8.0f;
		float attackRangeMax = 25.0f;
	};

	//ダメージ
	class Damage : public Default
	{
	public:
		Damage();
		Damage(GameObject* parent);
		~Damage() = default;

		//ステート遷移時の処理
		void Enter()override;
		//更新
		void Update()override;
		//終了処理
		void Exit()override;
		//ステート変更するかどうか
		std::string GetNext()override;
	};

	//死亡
	class Death : public Default
	{
	public:
		Death();
		Death(GameObject* parent);
		~Death() = default;

		//ステート遷移時の処理
		void Enter()override;
		//更新
		void Update()override;
		//終了処理
		void Exit()override;
		//ステート変更するかどうか
		std::string GetNext()override;
	};

	//狙った位置にワイヤーを刺す、直線的なワイヤー移動
	class AimWire : public Default
	{
	public:
		AimWire();
		AimWire(GameObject* parent);
		~AimWire() = default;

		//ステート遷移時の処理
		void Enter()override;
		//更新
		void Update()override;
		//終了処理
		void Exit()override;
		//ステート変更するかどうか
		std::string GetNext()override;
	};

	//ワイヤーでの弧を書いた移動
	class SwingWire : public Default
	{
	public:
		SwingWire();
		SwingWire(GameObject* parent);
		~SwingWire() = default;

		//ステート遷移時の処理
		void Enter()override;
		//更新
		void Update()override;
		//終了処理
		void Exit()override;
		//ステート変更するかどうか
		std::string GetNext()override;

	private:
		float maxWireLength = 40.0f;
		DirectX::XMFLOAT3 oldPosition[2]{};
		float swingTimer = 0.0f;
		float maxSwingTime = 0.5f;
	};

	//ワイヤーでの弧を書いた移動後のジャンプ
	class WireJump : public Default
	{
	public:
		WireJump();
		WireJump(GameObject* parent);
		~WireJump() = default;

		//ステート遷移時の処理
		void Enter()override;
		//更新
		void Update()override;
		//終了処理
		void Exit()override;
		//ステート変更するかどうか
		std::string GetNext()override;

	private:
		DirectX::XMFLOAT3 moveDirection = {};
		DirectX::XMFLOAT3 oldPosition[2]{};
		float jumpSpeed = 0.0f;
		float inputRatio = 0.5f;

		float coolTimer = 0.0f;//ワイヤーを刺すためのクールタイム
		float maxCoolTime = 1.0f;

		bool falling = false;
	};

	//空中攻撃
	class JumpAttack : public Default
	{
	public:
		JumpAttack();
		JumpAttack(GameObject* parent);
		~JumpAttack() = default;

		//ステート遷移時の処理
		void Enter()override;
		//更新
		void Update()override;
		//終了処理
		void Exit()override;
		//ステート変更するかどうか
		std::string GetNext()override;

	private:
		float attackRangeMin = 8.0f;
		float attackRangeMax = 30.0f;
		float maxMoveTime = 25.0f;//ここまでに移動を終える
		float attackTimer = 0.0f;
	};

	//回避
	class Avoid : public Default
	{
	public:
		Avoid();
		Avoid(GameObject* parent);
		~Avoid() = default;

		//ステート遷移時の処理
		void Enter()override;
		//更新
		void Update()override;
		//終了処理
		void Exit()override;
		//ステート変更するかどうか
		std::string GetNext()override;

	private:
		float avoidSpeed = 50.0f;
		DirectX::XMFLOAT3 avoidVec = {};
	};

	//空中回避
	class AvoidJump : public Default
	{
	public:
		AvoidJump();
		AvoidJump(GameObject* parent);
		~AvoidJump() = default;

		//ステート遷移時の処理
		void Enter()override;
		//更新
		void Update()override;
		//終了処理
		void Exit()override;
		//ステート変更するかどうか
		std::string GetNext()override;

	private:
		float avoidSpeed = 50.0f;
		float avoidJumpPower = 30.0f;
		DirectX::XMFLOAT3 avoidVec = {};

	};

	//オブジェクトにワイヤーを刺して振り回して投げる
	class WieldThrow : public Default
	{
	public:
		WieldThrow();
		WieldThrow(GameObject* parent);
		~WieldThrow() = default;

		//ステート遷移時の処理
		void Enter()override;
		//更新
		void Update()override;
		//終了処理
		void Exit()override;
		//ステート変更するかどうか
		std::string GetNext()override;

#if _APPEND
		//IMGUI
		void DebugGui() override;
#endif
#if _APPEND
		//ワイヤーの長さを振り回しながら調整する
		void CorWireLength();
#endif
	private:
		enum State
		{
			Thrust,
			Wield,
			Throw,
		};
		int state = Thrust;
#if _APPEND
		//-----< Thrustで使用 >-----//
		float maxThrustInterval = 24.0f;
		float maxThrustTime = 0.5f;
		float wireSpeed = 0.0f;
		DirectX::SimpleMath::Vector3 wireTipPos{};//ワイヤーの先端位置
		bool appearWire = false;//ワイヤーの出現
		bool thrust = false;//オブジェクトに刺さっているかどうか
		//-----< Wieldで使用 >-----//
		float wieldRadian = 0.0f;//回転角
		float wieldSpeed = 0.0f;//回転速度
		float maxWieldRadian = DirectX::XMConvertToRadians(270.0f);
		DirectX::SimpleMath::Vector3 rotateVec{};
		float minWireLength = 15.0f;
		float maxWireLength = 30.0f;
		float beforeWireLength = 0.0f;
		float maxPullRatio = 0.45f;
		bool pull = false;
		//-----< Throwで使用 >-----//
		float maxThrowTime = 27.0f;
		float maxRotateTime = 20.0f;
		float maxThrowRadian = DirectX::XMConvertToRadians(235.0f);
		DirectX::SimpleMath::Vector3 rotateAxis{};
		Transform* enemyTransform = nullptr;
		float accelRatio = 2.0f;
		float oldWieldSpeed = 0.0f;
		float throwSpeed = 0.0f;
		bool throwFlag = false;
		DirectX::SimpleMath::Vector3 throwVelocity{};
#endif
	};
}