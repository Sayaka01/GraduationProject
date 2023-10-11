#pragma once

class GameObject;

//�v���C���[�̏��
namespace PlayerState
{
	//���N���X
	class Default
	{
	public:
		Default() = default;
		virtual ~Default() = default;
		
		//�X�e�[�g�J�ڎ��̏���
		virtual void Enter() {}
		//�X�V
		virtual void Update() {}
		//�I������
		virtual void Exit() {}
		//�X�e�[�g�ύX���邩�ǂ���
		virtual std::string Judge() { return ""; }

		//���O�̎擾
		std::string GetName() { return name; }

	protected:
		std::string name = "";//�X�e�[�g�̖��O

		GameObject* parent = nullptr;
	};

	//�ҋ@
	class Idle : public Default
	{
	public:
		Idle();
		Idle(GameObject* parent);
		~Idle() = default;

		//�X�e�[�g�J�ڎ��̏���
		void Enter()override;
		//�X�V
		void Update()override;
		//�I������
		void Exit()override;
		//�X�e�[�g�ύX���邩�ǂ���
		std::string Judge()override;
	};

	//����
	class Run : public Default
	{
	public:
		Run();
		Run(GameObject* parent);
		~Run() = default;

		//�X�e�[�g�J�ڎ��̏���
		void Enter()override;
		//�X�V
		void Update()override;
		//�I������
		void Exit()override;
		//�X�e�[�g�ύX���邩�ǂ���
		std::string Judge()override;
	};
}