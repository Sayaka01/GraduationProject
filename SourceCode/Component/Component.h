#pragma once

#include <string>

class GameObject;
class Collider;

class Component
{
public:
	Component() = default;
	virtual ~Component() = default;


	//-----< 関数 >-----//

	//初期化
	virtual void Initialize(){}
	//更新
	virtual void Update(){}
	//描画
	virtual void Draw3D(){}
	virtual void Draw2D(){}
	//終了処理
	virtual void Finalize(){}
	//ImGui
	virtual void DebugGui(){}

	//ヒットイベント
	virtual void OnCollisionEnter(Collider* collider) {}


	//-----< Getter, Setter >-----//

	//コンポーネントの名前の取得
	std::string GetName() { return name; }

	//このコンポーネントの親オブジェクトの設定
	void SetParent(GameObject* parent) { this->parent = parent; }
	//このコンポーネントの親オブジェクトの取得
	[[nodiscard]] GameObject* GetParent() const { return parent; }
	//このコンポーネントの親オブジェクトの名前の取得
	[[nodiscard]] std::string GetParentName() const;

	//コンポーネント有効フラグの取得
	bool GetEnable() const { return enable; }
	//コンポーネント有効フラグの切り替え
	void SetEnable(bool flag) { enable = flag; }

protected:
	std::string name = "";

	GameObject* parent = nullptr;//このコンポーネントの親オブジェクト

	bool enable = true;//このコンポーネントが有効かどうか
};