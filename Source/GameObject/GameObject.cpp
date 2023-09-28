
#include <Stdafx.h>
#include "GameObject.h"
#include "../Component/Component.h"


//初期化
void GameObject::Initialize()
{
}

//終了処理
void GameObject::Finalize()
{
	//各コンポーネントの削除
	std::vector<Component*> removes;
	for (Component* component : components)
	{
		removes.emplace_back(component);
	}
	for(Component* component : removes)
	{
		std::vector<Component*>::iterator it = std::find(components.begin(), components.end(), component);

		if (it != components.end())
		{
			components.erase(it);
		}
		delete component;
	}

	removes.clear();
	components.clear();
}

//更新
void GameObject::Update()
{
	//各コンポーネントの更新
	for(Component* component : components)
	{
		component->Update();
	}

	//ImGui
	DebugGui();

	//子オブジェクトの更新
	for(GameObject* childObj : children)
	{
		childObj->Update();
	}

}

//描画
void GameObject::Draw()
{
	//各コンポーネントの描画
	for (Component* component : components)
	{
		component->Draw();
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
		component->DebugGui();
	}
	ImGui::End();
#endif
}

