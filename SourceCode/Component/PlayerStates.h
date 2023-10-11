#pragma once

class GameObject;

//プレイヤーの状態
namespace PlayerState
{
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
		virtual std::string Judge() { return ""; }

		//名前の取得
		std::string GetName() { return name; }

	protected:
		std::string name = "";//ステートの名前

		GameObject* parent = nullptr;
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
		std::string Judge()override;
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
		std::string Judge()override;
	};
}