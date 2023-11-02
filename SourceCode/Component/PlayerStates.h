#pragma once

class GameObject;

//プレイヤーの状態
namespace PlayerState
{
	//アニメーション
	enum class Animation
	{
		Attack,
		Death,
		Falling,
		GetHit1,
		GetHit2,
		Idle,
		Jump,
		JumpFlip,
		Landing,
		Revive,
		Running,
		Walking,
		Punching,
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
		virtual void Update() {}
		//終了処理
		virtual void Exit() {}
		//ステート変更するかどうか
		virtual std::string GetNext() { return ""; }

		//名前の取得
		std::string GetName() { return name; }

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


		std::string name = "";//ステートの名前

		GameObject* parent = nullptr;//親オブジェクト

		float rotateSpeed = DirectX::XMConvertToRadians(720);
		float rotateRatio = 0.75f;
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

	//落下
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
}