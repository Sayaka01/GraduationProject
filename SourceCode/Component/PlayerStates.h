#pragma once

class GameObject;

//�v���C���[�̏��
namespace PlayerState
{
	//�A�j���[�V����
	enum class Animation
	{
		Attack,//��
		Death,
		Falling,
		GetHit1,//�_���[�W��
		GetHit2,//�_���[�W��
		Idle,
		Jump,
		JumpFlip,
		Landing,
		Revive,//����
		Running,
		Walking,
		Punching,//�E
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
		virtual void Update();
		//�I������
		virtual void Exit() {}
		//�X�e�[�g�ύX���邩�ǂ���
		virtual std::string GetNext() { return ""; }

		//���O�̎擾
		std::string GetName() { return name; }

		//�U���͂̎擾
		float GetAttackPower() { return attackPower; }

	protected:
		//�ړ��x�N�g�����v���C���[�R���|�[�l���g�ɕۑ�
		void SetMoveVelocity(DirectX::XMFLOAT3 velocity);
		//�ړ��x�N�g�����v�Z
		DirectX::XMFLOAT3 CalcMoveVec();
		//�R���g���[���[��L�X�e�B�b�N�̓��͒l���擾
		DirectX::XMFLOAT2 GetLStickVec();
		//�ړ��x�N�g���ɉ�����Y����]
		void YAxisRotate(DirectX::XMFLOAT3 moveVelocity);

		//�ҋ@�X�e�[�g�̑J�ڏ���
		bool JudgeIdleState();
		//����X�e�[�g�̑J�ڏ���
		bool JudgeRunState();
		//�W�����v�X�e�[�g�̑J�ڏ���
		bool JudgeJumpState();
		//�p���`�i�E�j�X�e�[�g�̑J�ڏ���
		bool JudgePunchRightState();
		//�p���`�i���j�X�e�[�g�̑J�ڏ���
		bool JudgePunchLeftState();


		std::string name = "";//�X�e�[�g�̖��O

		GameObject* parent = nullptr;//�e�I�u�W�F�N�g

		float rotateSpeed = DirectX::XMConvertToRadians(720);
		float rotateRatio = 0.75f;

		static float attackInterval;
		float acceptAttackTime = 1.5f;

		static bool acceptAttackButton;
		static bool pushAttackButton;

		static int jumpCount;
		const int maxJumpCount = 2;

		float attackPower = 0.0f;
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
		std::string GetNext()override;
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
		std::string GetNext()override;
	};

	//�W�����v
	class Jump : public Default
	{
	public:
		Jump();
		Jump(GameObject* parent);
		~Jump() = default;

		//�X�e�[�g�J�ڎ��̏���
		void Enter()override;
		//�X�V
		void Update()override;
		//�I������
		void Exit()override;
		//�X�e�[�g�ύX���邩�ǂ���
		std::string GetNext()override;
	};

	//����
	class Falling : public Default
	{
	public:
		Falling();
		Falling(GameObject* parent);
		~Falling() = default;

		//�X�e�[�g�J�ڎ��̏���
		void Enter()override;
		//�X�V
		void Update()override;
		//�I������
		void Exit()override;
		//�X�e�[�g�ύX���邩�ǂ���
		std::string GetNext()override;
	};

	//���n
	class Landing : public Default
	{
	public:
		Landing();
		Landing(GameObject* parent);
		~Landing() = default;

		//�X�e�[�g�J�ڎ��̏���
		void Enter()override;
		//�X�V
		void Update()override;
		//�I������
		void Exit()override;
		//�X�e�[�g�ύX���邩�ǂ���
		std::string GetNext()override;
	};

	//�p���`�i�E�j
	class PunchRight : public Default
	{
	public:
		PunchRight();
		PunchRight(GameObject* parent);
		~PunchRight() = default;

		//�X�e�[�g�J�ڎ��̏���
		void Enter()override;
		//�X�V
		void Update()override;
		//�I������
		void Exit()override;
		//�X�e�[�g�ύX���邩�ǂ���
		std::string GetNext()override;
	};

	//�p���`�i���j
	class PunchLeft : public Default
	{
	public:
		PunchLeft();
		PunchLeft(GameObject* parent);
		~PunchLeft() = default;

		//�X�e�[�g�J�ڎ��̏���
		void Enter()override;
		//�X�V
		void Update()override;
		//�I������
		void Exit()override;
		//�X�e�[�g�ύX���邩�ǂ���
		std::string GetNext()override;
	};

	//�_���[�W
	class Damage : public Default
	{
	public:
		Damage();
		Damage(GameObject* parent);
		~Damage() = default;

		//�X�e�[�g�J�ڎ��̏���
		void Enter()override;
		//�X�V
		void Update()override;
		//�I������
		void Exit()override;
		//�X�e�[�g�ύX���邩�ǂ���
		std::string GetNext()override;
	};

	//���S
	class Death : public Default
	{
	public:
		Death();
		Death(GameObject* parent);
		~Death() = default;

		//�X�e�[�g�J�ڎ��̏���
		void Enter()override;
		//�X�V
		void Update()override;
		//�I������
		void Exit()override;
		//�X�e�[�g�ύX���邩�ǂ���
		std::string GetNext()override;
	};

}