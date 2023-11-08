#pragma once

class GameObject;

//プレイヤーの状態
namespace PlayerState
{
	//アニメーション
	enum class Animation
	{
		Attack,//左
		Death,
		Falling,
		GetHit1,//ダメージ小
		GetHit2,//ダメージ大
		Idle,
		Jump,
		JumpFlip,
		Landing,
		Revive,//復活
		Running,
		Walking,
		Punching,//右
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

		//名前の取得
		std::string GetName() { return name; }

		//攻撃力の取得
		float GetAttackPower() { return attackPower; }

	protected:
		//移動ベクトルをプレイヤーコンポーネントに保存
		void SetMoveVelocity(DirectX::XMFLOAT3 velocity);
		//移動ベクトルを計算
		DirectX::XMFLOAT3 CalcMoveVec();
		//コントローラーのLスティックの入力値を取得
		DirectX::XMFLOAT2 GetLStickVec();
		//移動ベクトルに応じたY軸回転
		void YAxisRotate(DirectX::XMFLOAT3 moveVelocity);

		//待機ステートの遷移条件
		bool JudgeIdleState();
		//走りステートの遷移条件
		bool JudgeRunState();
		//ジャンプステートの遷移条件
		bool JudgeJumpState();
		//パンチ（右）ステートの遷移条件
		bool JudgePunchRightState();
		//パンチ（左）ステートの遷移条件
		bool JudgePunchLeftState();


		std::string name = "";//ステートの名前

		GameObject* parent = nullptr;//親オブジェクト

		float rotateSpeed = DirectX::XMConvertToRadians(720);
		float rotateRatio = 0.75f;

		static float attackInterval;
		float acceptAttackTime = 1.5f;

		static bool acceptAttackButton;
		static bool pushAttackButton;

		static int jumpCount;
		const int maxJumpCount = 2;

		float attackPower = 0.0f;
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

}