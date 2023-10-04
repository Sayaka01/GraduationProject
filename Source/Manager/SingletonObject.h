#pragma once

#include "GameObject/GameObject.h"


class SingletonObject : public GameObject
{
private:
	//コンストラクタ
	SingletonObject() = default;
	//デストラクタ
	virtual ~SingletonObject() override = default;

public:
	//インスタンス取得
	static SingletonObject& Instance()
	{
		static SingletonObject instance;
		return instance;
	}
};