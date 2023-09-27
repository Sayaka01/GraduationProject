
#include "GameObject.h"

#include "../Component/Component.h"

#include "../../imgui/imgui.h"


//������
void GameObject::Initialize()
{
}

//�I������
void GameObject::Finalize()
{
	//�e�R���|�[�l���g�̍폜
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

//�X�V
void GameObject::Update()
{
	//�e�R���|�[�l���g�̍X�V
	for(Component* component : components)
	{
		component->Update();
	}
}

//�`��
void GameObject::Draw()
{
	//�e�R���|�[�l���g�̕`��
	for (Component* component : components)
	{
		component->Draw();
	}
}

//�f�o�b�O�`��
void GameObject::DebugDraw()
{
#ifdef USE_IMGUI
	ImGui::Begin(name.c_str());
	//�e�R���|�[�l���g�̃f�o�b�O�`��
	for (Component* component : components)
	{
		component->DebugDraw();
	}
	ImGui::End();
#endif
}

