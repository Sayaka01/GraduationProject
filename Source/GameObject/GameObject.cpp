
#include <Stdafx.h>
#include "GameObject.h"
#include "Component/Component.h"

#include "Component/Transform.h"

//�R���X�g���N�^
GameObject::GameObject()
{
	Initialize();
}
GameObject::GameObject(std::string name) : name(std::move(name))
{
	Initialize();
}


//������
void GameObject::Initialize()
{
	//Transform Component�̒ǉ�
	AddComponent(new Transform());
}

//�I������
void GameObject::Finalize()
{
	//�R���|�[�l���g�̍폜
	ClearComponent();

	//�q�I�u�W�F�N�g�S�폜
	AllDeleteChildren();
}

//�X�V
void GameObject::Update()
{
	//�e�R���|�[�l���g�̍X�V
	for(Component* component : components)
	{
		if (component->GetEnable())
			component->Update();
	}

	//ImGui
	DebugGui();

	//�q�I�u�W�F�N�g�̍X�V
	for(GameObject* childObj : children)
	{
		childObj->Update();
	}

	//�q�I�u�W�F�N�g�̍폜
	DeleteChildren();
}

//�`��
void GameObject::Draw()
{
	//�e�R���|�[�l���g�̕`��
	for (Component* component : components)
	{
		if (component->GetEnable())
			component->Draw();
	}

	//�q�I�u�W�F�N�g�̕`��
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
	//�e�R���|�[�l���g�̃f�o�b�O�`��
	for (Component* component : components)
	{
		if (component->GetEnable())
			component->DebugGui();
	}
	//�q�I�u�W�F�N�g�̃f�o�b�O�`��
	for (GameObject* childObj : children)
	{
		childObj->DebugGui();
	}
	ImGui::End();
#endif
}

//�R���|�[�l���g�̒ǉ��i1���j
void GameObject::AddComponent(Component* newComponent)
{
	//�ǉ��\��̃R���|�[�l���g��nullptr�Ȃ�return
	if (newComponent == nullptr) return;

	newComponent->SetParent(this);
	components.emplace_back(newComponent);
	newComponent->Initialize();
}

//�R���|�[�l���g�̍폜
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


//�q�I�u�W�F�N�g�̒ǉ�
void GameObject::AddChild(GameObject* childObj)
{
	childObj->SetParent(this);
	children.emplace_back(childObj);
}
//�q�I�u�W�F�N�g�̎擾(���O�����j
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
//�q�I�u�W�F�N�g�̎擾�i�C���f�b�N�X�j
[[nodiscard]] GameObject* GameObject::GetGameObj(size_t index)const
{
	return children.at(index);
}
//�q�I�u�W�F�N�g�̍폜����(���O�����j
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
//�q�I�u�W�F�N�g�̍폜����
void GameObject::RemoveChild(GameObject* obj)
{
	removeChildren.emplace_back(obj);
}
//�q�I�u�W�F�N�g�̍폜
void GameObject::DeleteChildren()
{
	for (GameObject* child : removeChildren)
	{
		//std::vector����v�f���폜����Ƃ��̓C�e���[�^�[�ō폜���Ȃ���΂Ȃ�Ȃ�
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
//�q�I�u�W�F�N�g�̑S�폜
void GameObject::AllDeleteChildren()
{
	for (GameObject* child : children)
	{
		removeChildren.emplace_back(child);
	}

	DeleteChildren();
}
//�q�I�u�W�F�N�g���̎擾
[[nodiscard]] size_t GameObject::GetChildrenCount()const
{
	return static_cast<int>(children.size());
}
