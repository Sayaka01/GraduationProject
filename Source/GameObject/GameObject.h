#pragma once

#include <string>
#include <vector>

class Component;

enum class Tag
{
	Untagged,
};

enum class Layer
{
	Default,
};

//GameObjectクラス
class GameObject
{
public:
	GameObject() = default; //コンストラクタ
	GameObject(std::string name):name(std::move(name)){}
	~GameObject() = default; //デストラクタ


	//-----< 関数 >-----//

	//初期化
	virtual void Initialize();
	//終了処理
	virtual void Finalize();
	//更新
	virtual void Update();
	//描画
	virtual void Draw();
	//デバッグ描画
	virtual void DebugDraw();


	//コンポーネントの追加（1つずつ）
	template<class T>
	T* AddComponent(T* newComponent)
	{
		//追加予定のコンポーネントがnullptrならreturn
		if (newComponent == nullptr) return nullptr;

		newComponent->SetParent(this);
		components.emplace_back(newComponent);
		newComponent->Initialize();
		return newComponent;
	}
	//コンポーネントの取得（1つずつ）
	template<class T>
	T* GetComponent(const std::string& componentName)
	{
		//名前がないならreturn
		if (componentName == "") return nullptr;

		for (Component* component : components)
		{
			if (component->GetName() == componentName)
			{
				T* com = dynamic_cast<T*>(component);
				if (com != nullptr) return com;
			}
		}

		return nullptr;//"componentName"という名前のコンポーネントはこのオブジェクトにはなかった
	}

	//子オブジェクトの追加
	void AddChild(GameObject* childObj)
	{
		childObj->SetParent(this);
		children.emplace_back(childObj);
	}

	//子オブジェクトの取得(名前検索）
	GameObject* GetChild(std::string name)
	{
		for (GameObject* child : children)
		{
			if(child->name == name)
			{
				return child;
			}
		}
	}

	//-----< Getter, Setter >-----//

	//オブジェクトの名前の取得
	std::string GetName() { return name; }

	//親オブジェクトの設定
	void SetParent(GameObject* parent)
	{
		this->parent = parent;
	}


protected:
	std::string name;//オブジェクトの名前
	Tag tag = Tag::Untagged;//タグ（当たり判定などオブジェクトを特定するために使用）
	Layer layer = Layer::Default;//レイヤー（Rendering設定のグループ分けに使用）

	bool active = true;//オブジェクトのアクティブ状態
	//bool static = false;//オブジェクトの静的状態

	std::vector<Component*> components;

	GameObject* parent = nullptr;//親オブジェクト
	std::vector<GameObject*> children;//子オブジェクト
};



