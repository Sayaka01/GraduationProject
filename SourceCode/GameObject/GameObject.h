#pragma once

#include <string>
#include <vector>

class Component;

enum class Tag//�^�O�i�����蔻��ȂǃI�u�W�F�N�g����肷�邽�߂Ɏg�p�j
{
	Untagged,
	Player,
	Enemy,
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
	void AddComponent(Component* newComponent);
	//�R���|�[�l���g�̎擾�i1���j
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
	//�R���|�[�l���g�̎擾�i�����j
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
	//�R���|�[�l���g�̎擾�i���O�����j�i1���j
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

	//�R���|�[�l���g�̍폜
	void ClearComponent();


	//�q�I�u�W�F�N�g�̒ǉ�
	void AddChild(GameObject* childObj);
	//�q�I�u�W�F�N�g�̎擾(���O�����j
	[[nodiscard]] GameObject* GetChild(std::string name) const;
	//�q�I�u�W�F�N�g�̎擾�i�C���f�b�N�X�j
	[[nodiscard]] GameObject* GetGameObj(size_t index)const;
	//active=false�̎q�I�u�W�F�N�g�폜
	void RemoveChildren();
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

	//�e�I�u�W�F�N�g�̎擾
	GameObject* GetParent() { return parent; }
	//�e�I�u�W�F�N�g�̐ݒ�
	void SetParent(GameObject* parent)
	{
		this->parent = parent;
	}

	//�^�O�̎擾
	Tag GetTag() { return tag; }
	//�^�O�̐ݒ�
	void SetTag(Tag tag) { this->tag = tag; }

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



