#pragma once

class GameObject;

//�v���C���[�̏��
namespace PlayerState
{
	//�A�j���[�V����
	enum class Animation
	{
		Attack,
		Death,
		Falling,
		GetHit1,
		GetHit2,
		Idle,
		Jump,
		JumpFlip,
		Landing,
		Revive,
		Running,
		Walking,
		Punching,
	};

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
		//�ړ��x�N�g�����v���C���[�R���|�[�l���g�ɕۑ�
		void SetMoveVelocity(DirectX::XMFLOAT3 velocity);
		//�ړ��x�N�g�����v�Z
		DirectX::XMFLOAT3 CalcMoveVec();
		//�R���g���[���[��L�X�e�B�b�N�̓��͒l���擾
		DirectX::XMFLOAT2 GetLStickVec();
		//�ړ��x�N�g���ɉ�����Y����]
		void YAxisRotate(DirectX::XMFLOAT3 moveVelocity);

		std::string name = "";//�X�e�[�g�̖��O

		GameObject* parent = nullptr;//�e�I�u�W�F�N�g

		float rotateSpeed = DirectX::XMConvertToRadians(720);
		float rotateRatio = 0.75f;
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

	private:
		float runSpeed = 30.0f;
	};
}