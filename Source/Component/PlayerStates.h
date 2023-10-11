#pragma once

//プレイヤーの状態
namespace PlayerState
{
	//基底クラス
	class Default
	{
	public:
		Default() = default;
		virtual ~Default() = default;
		
		//初期処理
		virtual void Enter() {}
		//更新
		virtual void Update() {}
		//ステート変更するかどうか
		virtual void Judge() {}
		//終了処理
		virtual void Exit() {}

	};

	//待機
	class Idle : public Default
	{
	public:
		Idle() = default;
		~Idle() = default;

		//初期処理
		void Enter()override {}
		//更新
		void Update()override {}
		//ステート変更するかどうか
		void Judge()override {}
		//終了処理
		void Exit()override {}
	};
}