#pragma once

#include <vector>

#include "GameObject/GameObject.h"


class Manager : public GameObject
{
private:
	//コンストラクタ
	Manager() = default;
	//デストラクタ
	virtual ~Manager() = default;

public:
	//インスタンス取得
	static Manager& Instance()
	{
		static Manager instance;
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

	//登録
	void Register(GameObject* obj);
	//削除
	void Remove(GameObject* obj);
	//全削除
	void Clear();

	//ゲームオブジェクト数を取得
	[[nodiscard]] int GetGameObjCount()const { return static_cast<int>(gameObjects.size()); }
	//ゲームオブジェクトを取得
	[[nodiscard]] GameObject* GetGameObj(size_t index)const { return gameObjects.at(index); }

protected:
	std::vector<GameObject*> gameObjects;
	std::vector<GameObject*> removes;

};