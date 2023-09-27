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

//GameObject�N���X
class GameObject
{
public:
	GameObject() = default; //�R���X�g���N�^
	GameObject(std::string name):name(std::move(name)){}
	~GameObject() = default; //�f�X�g���N�^


	//-----< �֐� >-----//

	//������
	virtual void Initialize();
	//�I������
	virtual void Finalize();
	//�X�V
	virtual void Update();
	//�`��
	virtual void Draw();
	//�f�o�b�O�`��
	virtual void DebugDraw();


	//�R���|�[�l���g�̒ǉ��i1���j
	template<class T>
	T* AddComponent(T* newComponent)
	{
		//�ǉ��\��̃R���|�[�l���g��nullptr�Ȃ�return
		if (newComponent == nullptr) return nullptr;

		newComponent->SetParent(this);
		components.emplace_back(newComponent);
		newComponent->Initialize();
		return newComponent;
	}
	//�R���|�[�l���g�̎擾�i1���j
	template<class T>
	T* GetComponent(const std::string& componentName)
	{
		//���O���Ȃ��Ȃ�return
		if (componentName == "") return nullptr;

		for (Component* component : components)
		{
			if (component->GetName() == componentName)
			{
				T* com = dynamic_cast<T*>(component);
				if (com != nullptr) return com;
			}
		}

		return nullptr;//"componentName"�Ƃ������O�̃R���|�[�l���g�͂��̃I�u�W�F�N�g�ɂ͂Ȃ�����
	}

	//�q�I�u�W�F�N�g�̒ǉ�
	void AddChild(GameObject* childObj)
	{
		childObj->SetParent(this);
		children.emplace_back(childObj);
	}

	//�q�I�u�W�F�N�g�̎擾(���O�����j
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

	//�I�u�W�F�N�g�̖��O�̎擾
	std::string GetName() { return name; }

	//�e�I�u�W�F�N�g�̐ݒ�
	void SetParent(GameObject* parent)
	{
		this->parent = parent;
	}


protected:
	std::string name;//�I�u�W�F�N�g�̖��O
	Tag tag = Tag::Untagged;//�^�O�i�����蔻��ȂǃI�u�W�F�N�g����肷�邽�߂Ɏg�p�j
	Layer layer = Layer::Default;//���C���[�iRendering�ݒ�̃O���[�v�����Ɏg�p�j

	bool active = true;//�I�u�W�F�N�g�̃A�N�e�B�u���
	//bool static = false;//�I�u�W�F�N�g�̐ÓI���

	std::vector<Component*> components;

	GameObject* parent = nullptr;//�e�I�u�W�F�N�g
	std::vector<GameObject*> children;//�q�I�u�W�F�N�g
};



