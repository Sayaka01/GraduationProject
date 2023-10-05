#pragma once

#include <string>

class GameObject;

class Component
{
public:
	Component() = default;
	virtual ~Component() = default;


	//-----< �֐� >-----//

	//������
	virtual void Initialize(){}
	//�X�V
	virtual void Update(){}
	//�`��
	virtual void Draw(){}
	//ImGui
	virtual void DebugGui(){}


	//-----< Getter, Setter >-----//

	//���̃R���|�[�l���g�̐e�I�u�W�F�N�g�̐ݒ�
	void SetParent(GameObject* parent) { this->parent = parent; }
	//���̃R���|�[�l���g�̐e�I�u�W�F�N�g�̎擾
	[[nodiscard]] GameObject* GetParent() const { return parent; }
	//���̃R���|�[�l���g�̐e�I�u�W�F�N�g�̖��O�̎擾
	[[nodiscard]] std::string GetParentName() const;

	//�R���|�[�l���g�L���t���O�̎擾
	bool GetEnable() const { return enable; }
	//�R���|�[�l���g�L���t���O�̐؂�ւ�
	void SetEnable(bool flag) { enable = flag; }

protected:
	std::string name = "";

	GameObject* parent = nullptr;//���̃R���|�[�l���g�̐e�I�u�W�F�N�g

	bool enable = true;//���̃R���|�[�l���g���L�����ǂ���
};