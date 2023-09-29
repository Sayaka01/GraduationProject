#pragma once

#include <vector>

#include "GameObject/GameObject.h"


class SingletonObject : public GameObject
{
private:
	//コンストラクタ
	SingletonObject() = default;
	//デストラクタ
	~SingletonObject() override = default;

public:
	//インスタンス取得
	static SingletonObject& Instance()
	{
		static SingletonObject instance;
		return instance;
	}

	//初期化
	void Initialize() override;
	//更新
	void Update() override;
	//描画
	void Draw() override;
	//ImGui
	void DebugGui() override;
	

};