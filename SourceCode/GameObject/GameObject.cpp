
#include <Stdafx.h>
#include "GameObject.h"
#include "Component/Component.h"

#include "Component/Transform.h"

//コンストラクタ
GameObject::GameObject()
{
	Initialize();
}
GameObject::GameObject(std::string name) : name(std::move(name))
{
	Initialize();
}


//初期化
void GameObject::Initialize()
{
	//Transform Componentの追加
	AddComponent(new Transform());
}

//終了処理
void GameObject::Finalize()
{
	//コンポーネントの削除
	ClearComponent();

	//子オブジェクト全削除
	AllDeleteChildren();
}

//更新
void GameObject::Update()
{
	//各コンポーネントの更新
	for(Component* component : components)
	{
		if (component->GetEnable())
			component->Update();
	}

	//ImGui
	DebugGui();

	//子オブジェクトの更新
	for(GameObject* childObj : children)
	{
		childObj->Update();
	}

	//子オブジェクトの削除
	DeleteChildren();
}

//描画
void GameObject::Draw()
{
	//各コンポーネントの描画
	for (Component* component : components)
	{
		if (component->GetEnable())
			component->Draw();
	}

	//子オブジェクトの描画
	for (GameObject* childObj : children)
	{
		childObj->Draw();
	}
}

//ImGui
void GameObject::DebugGui()
{
#ifdef USE_IMGUI
	ImGui::Begin(name.c_str());
	//各コンポーネントのデバッグ描画
	for (Component* component : components)
	{
		if (component->GetEnable())
			component->DebugGui();
	}
	//子オブジェクトのデバッグ描画
	for (GameObject* childObj : children)
	{
		childObj->DebugGui();
	}
	ImGui::End();
#endif
}

//コンポーネントの追加（1つずつ）
void GameObject::AddComponent(Component* newComponent)
{
	//追加予定のコンポーネントがnullptrならreturn
	if (newComponent == nullptr) return;

	newComponent->SetParent(this);
	components.emplace_back(newComponent);
	newComponent->Initialize();
}

//コンポーネントの削除
void GameObject::ClearComponent()
{
	std::vector<Component*> removes;
	for (Component* component : components)
	{
		removes.emplace_back(component);
	}
	for (Component* component : removes)
	{
		std::vector<Component*>::iterator it = std::find(components.begin(), components.end(), component);

		if (it != components.end())
		{
			components.erase(it);
		}
		component->Finalize();
		delete component;
	}

	removes.clear();
	components.clear();
}


//子オブジェクトの追加
void GameObject::AddChild(GameObject* childObj)
{
	childObj->SetParent(this);
	children.emplace_back(childObj);
}
//子オブジェクトの取得(名前検索）
GameObject* GameObject::GetChild(std::string name) const
{
	for (GameObject* child : children)
	{
		if (child->name == name)
		{
			return child;
		}
	}
	return nullptr;
}
//子オブジェクトの取得（インデックス）
[[nodiscard]] GameObject* GameObject::GetGameObj(size_t index)const
{
	return children.at(index);
}
//子オブジェクトの削除準備(名前検索）
void GameObject::RemoveChild(std::string name)
{
	for (GameObject* child : children)
	{
		if (child->name == name)
		{
			removeChildren.emplace_back(child);
		}
	}
}
//子オブジェクトの削除準備
void GameObject::RemoveChild(GameObject* obj)
{
	removeChildren.emplace_back(obj);
}
//子オブジェクトの削除
void GameObject::DeleteChildren()
{
	for (GameObject* child : removeChildren)
	{
		//std::vectorから要素を削除するときはイテレーターで削除しなければならない
		std::vector<GameObject*>::iterator it = std::find(children.begin(), children.end(), child);

		if (it != children.end())
		{
			child->Finalize();
			children.erase(it);
		}
		delete child;
		child = nullptr;
	}
	removeChildren.clear();
}
//子オブジェクトの全削除
void GameObject::AllDeleteChildren()
{
	for (GameObject* child : children)
	{
		removeChildren.emplace_back(child);
	}

	DeleteChildren();
}
//子オブジェクト数の取得
[[nodiscard]] size_t GameObject::GetChildrenCount()const
{
	return static_cast<int>(children.size());
}
