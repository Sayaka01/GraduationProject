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
	GameObject();
	GameObject(std::string name);
	virtual ~GameObject() = default; //�f�X�g���N�^


	//-----< �֐� >-----//

	//������
	virtual void Initialize();
	//�I������
	virtual void Finalize();
	//�X�V
	virtual void Update();
	//�`��
	virtual void Draw();
	//ImGui
	virtual void DebugGui();


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
	//�R���|�[�l���g�̍폜
	void ClearComponent();


	//�q�I�u�W�F�N�g�̒ǉ�
	void AddChild(GameObject* childObj);
	//�q�I�u�W�F�N�g�̎擾(���O�����j
	GameObject* GetChild(std::string name) const;
	//�q�I�u�W�F�N�g�̎擾�i�C���f�b�N�X�j
	[[nodiscard]] GameObject* GetGameObj(size_t index)const;
	//�q�I�u�W�F�N�g�̍폜����(���O�����j
	void RemoveChild(std::string name);
	//�q�I�u�W�F�N�g�̍폜����
	void RemoveChild(GameObject* obj);
	//�q�I�u�W�F�N�g�̍폜
	void DeleteChildren();
	//�q�I�u�W�F�N�g�̑S�폜
	void AllDeleteChildren();
	//�q�I�u�W�F�N�g���̎擾
	[[nodiscard]] size_t GetChildrenCount()const;

	

	//�I�u�W�F�N�g�̖��O�̎擾
	std::string GetName() { return name; }

	//�e�I�u�W�F�N�g�̐ݒ�
	void SetParent(GameObject* parent)
	{
		this->parent = parent;
	}


protected:
	std::string name{};//�I�u�W�F�N�g�̖��O
	Tag tag = Tag::Untagged;//�^�O�i�����蔻��ȂǃI�u�W�F�N�g����肷�邽�߂Ɏg�p�j
	Layer layer = Layer::Default;//���C���[�iRendering�ݒ�̃O���[�v�����Ɏg�p�j

	bool active = true;//�I�u�W�F�N�g�̃A�N�e�B�u���
	//bool static = false;//�I�u�W�F�N�g�̐ÓI���

	std::vector<Component*> components{};

	GameObject* parent = nullptr;//�e�I�u�W�F�N�g
	std::vector<GameObject*> children{};//�q�I�u�W�F�N�g
	std::vector<GameObject*> removeChildren{};//�q�I�u�W�F�N�g�폜�p
};


