#pragma once

#include <string>
#include <vector>

class Component;

enum class Tag//タグ（当たり判定などオブジェクトを特定するために使用）
{
	Untagged,
	Player,
	Enemy,
};

enum class Layer
{
	Default,
};

//GameObjectクラス
class GameObject
{
public:
	GameObject();
	GameObject(std::string name);
	virtual ~GameObject() = default; //デストラクタ


	//-----< 関数 >-----//

	//初期化
	virtual void Initialize();
	//終了処理
	virtual void Finalize();
	//更新
	virtual void Update();
	//描画
	virtual void Draw();
	//ImGui
	virtual void DebugGui();


	//コンポーネントの追加（1つずつ）
	void AddComponent(Component* newComponent);
	//コンポーネントの取得（1つずつ）
	template<class T>
	T* GetComponent()
	{
		const std::string compName = typeid(T).name();
		for (Component* component : components)
		{
			if (typeid(*component).name() == compName)
			{
				T* com = dynamic_cast<T*>(component);
				if (com != nullptr) return com;
			}
		}
		return nullptr;
	}
	//コンポーネントの取得（複数）
	template<class T>
	std::vector<T*> GetComponents()
	{
		std::vector<T*> comVec;
		comVec.clear();

		const std::string compName = typeid(T).name();
		for (Component* component : components)
		{
			if (typeid(*component).name() == compName)
			{
				T* com = dynamic_cast<T*>(component);
				if (com != nullptr)
				{
					comVec.push_back(com);
				}
			}
		}
		return comVec;
	}
	template<class T>
	//コンポーネントの取得（名前検索）（1つずつ）
	T* GetComponent(std::string compName)
	{
		for (Component* component : components)
		{
			if (component->GetName() == compName)
			{
				T* com = dynamic_cast<T*>(component);
				if (com != nullptr) return com;
			}
		}
		return nullptr;
	}

	//コンポーネントの削除
	void ClearComponent();


	//子オブジェクトの追加
	void AddChild(GameObject* childObj);
	//子オブジェクトの取得(名前検索）
	[[nodiscard]] GameObject* GetChild(std::string name) const;
	//子オブジェクトの取得（インデックス）
	[[nodiscard]] GameObject* GetGameObj(size_t index)const;
	//active=falseの子オブジェクト削除
	void RemoveChildren();
	//子オブジェクトの削除準備(名前検索）
	void RemoveChild(std::string name);
	//子オブジェクトの削除準備
	void RemoveChild(GameObject* obj);
	//子オブジェクトの削除
	void DeleteChildren();
	//子オブジェクトの全削除
	void AllDeleteChildren();
	//子オブジェクト数の取得
	[[nodiscard]] size_t GetChildrenCount()const;


	//オブジェクトの名前の取得
	std::string GetName() { return name; }

	//親オブジェクトの取得
	GameObject* GetParent() { return parent; }
	//親オブジェクトの設定
	void SetParent(GameObject* parent)
	{
		this->parent = parent;
	}

	//タグの取得
	Tag GetTag() { return tag; }
	//タグの設定
	void SetTag(Tag tag) { this->tag = tag; }

protected:
	std::string name{};//オブジェクトの名前
	Tag tag = Tag::Untagged;//タグ（当たり判定などオブジェクトを特定するために使用）
	Layer layer = Layer::Default;//レイヤー（Rendering設定のグループ分けに使用）

	bool active = true;//オブジェクトのアクティブ状態
	//bool static = false;//オブジェクトの静的状態

	std::vector<Component*> components{};

	GameObject* parent = nullptr;//親オブジェクト
	std::vector<GameObject*> children{};//子オブジェクト
	std::vector<GameObject*> removeChildren{};//子オブジェクト削除用
};



