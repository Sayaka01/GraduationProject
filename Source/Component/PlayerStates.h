#pragma once

//�v���C���[�̏��
namespace PlayerState
{
	//���N���X
	class Default
	{
	public:
		Default() = default;
		virtual ~Default() = default;
		
		//��������
		virtual void Enter() {}
		//�X�V
		virtual void Update() {}
		//�X�e�[�g�ύX���邩�ǂ���
		virtual void Judge() {}
		//�I������
		virtual void Exit() {}

	};

	//�ҋ@
	class Idle : public Default
	{
	public:
		Idle() = default;
		~Idle() = default;

		//��������
		void Enter()override {}
		//�X�V
		void Update()override {}
		//�X�e�[�g�ύX���邩�ǂ���
		void Judge()override {}
		//�I������
		void Exit()override {}
	};
}