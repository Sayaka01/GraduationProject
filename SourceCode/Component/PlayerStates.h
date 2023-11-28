#pragma once

class GameObject;

//�v���C���[�̏��
namespace PlayerState
{
	//�A�j���[�V����
	enum class Animation
	{
		Idle,//�ҋ@
		Walking,//����
		Running,//����

		Jump,//�W�����v
		Falling,//����
		Landing,//���n
		JumpFlip,//2�i�W�����v�i�󒆉���j

		Avoid,//���

		PunchRight,//�E�p���`
		PunchLeft,//���p���`
		Kick,//�L�b�N	
		JumpPunch,//�󒆍U��

		Damage,//�_���[�W
		Death,//���S

		DangleWire,//���C���[�Ԃ牺����

		Thrust,//�˂�����
		Wield,//�U���
		Throw,//������

		AnimNum,
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
		//ImGui
		virtual void DebugGui();

		//���O�̎擾
		std::string GetName() { return name; }

		//�U���͂̎擾
		float GetAttackPower() { return attackPower; }

		//�U���Ԋu�̎擾
		float GetAttackInterval() { return attackInterval; }

		//�p�����[�^�[�̐ݒ�
		void SetParameter(DirectX::XMFLOAT3 param) { parameter = param; }

		//�W�����v�񐔂̏�����
		void InitJumpCount() { jumpCount = 0; }

	protected:
		//�ړ��x�N�g����RigidBody�R���|�[�l���g�ɕۑ�
		void SetMoveVelocity(DirectX::XMFLOAT3 velocity);
		//�ړ��x�N�g����RigidBody�R���|�[�l���g�ɒǉ�
		void AddMoveVelocity(DirectX::XMFLOAT3 velocity);
		//�J�����̑O�������擾
		DirectX::XMFLOAT3 GetCameraFront();
		//�J�����̉E�������擾
		DirectX::XMFLOAT3 GetCameraRight();
		//�ړ��x�N�g�����v�Z
		DirectX::XMFLOAT3 CalcMoveVec();
		//�R���g���[���[��L�X�e�B�b�N�̓��͒l���擾
		DirectX::XMFLOAT2 GetLStickVec();
		//�ړ��x�N�g���ɉ�����Y����]
		void YAxisRotate(DirectX::XMFLOAT3 moveVelocity);
		//��ԋ߂��G���擾
		void CalcEnemyDistance();
		//�A�j���[�V�����X�s�[�h���擾
		float GetAnimationSpeed(int index);
		//�A�j���[�V�����X�s�[�h�̐ݒ�
		void SetAnimationSpeed(int index, float speed);

		//�ҋ@�X�e�[�g�̑J�ڏ���
		bool JudgeIdleState();
		//����X�e�[�g�̑J�ڏ���
		bool JudgeRunState();
		//�W�����v�X�e�[�g�̑J�ڏ���
		bool JudgeJumpState();
		//�U���X�e�[�g�̑J�ڏ���
		bool JudgeAttackState();
		//���C���[�ł̒����ړ��X�e�[�g�̑J�ڏ���
		bool JudgeAimWireState();
		//���C���[�ł̌ʂ��������ړ��X�e�[�g�̑J�ڏ���
		bool JudgeSwingWireState();
		//����X�e�[�g�̑J�ڏ���
		bool JudgeAvoidState();
		//�U��񂵂ē�����X�e�[�g�̑J�ڏ���
		bool JudgeWieldThrowState();

		//�U��񂷃I�u�W�F�N�g�̌���
		void SearchThrowObj();


		std::string name = "";//�X�e�[�g�̖��O

		GameObject* parent = nullptr;//�e�I�u�W�F�N�g

		float rotateSpeed = DirectX::XMConvertToRadians(1440);
		float rotateRatio = 0.75f;

		static float attackInterval;
		float acceptAttackTime = 1.5f;

		static bool acceptAttackButton;
		static bool pushAttackButton;

		static int jumpCount;
		const int maxJumpCount = 2;

		float attackPower = 0.0f;

		static DirectX::XMFLOAT3 parameter;//�}�f�p�����[�^

		static bool isAvoid;//����������ǂ���

		float searchRange = 300.0f;

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

	private:
		float attackRangeMin = 8.0f;
		float attackRangeMax = 25.0f;
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
	private:
		float attackRangeMin = 8.0f;
		float attackRangeMax = 25.0f;
	};

	//�L�b�N
	class Kick : public Default
	{
	public:
		Kick();
		Kick(GameObject* parent);
		~Kick() = default;

		//�X�e�[�g�J�ڎ��̏���
		void Enter()override;
		//�X�V
		void Update()override;
		//�I������
		void Exit()override;
		//�X�e�[�g�ύX���邩�ǂ���
		std::string GetNext()override;
	private:
		float attackRangeMin = 8.0f;
		float attackRangeMax = 25.0f;
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

	//�_�����ʒu�Ƀ��C���[���h���A�����I�ȃ��C���[�ړ�
	class AimWire : public Default
	{
	public:
		AimWire();
		AimWire(GameObject* parent);
		~AimWire() = default;

		//�X�e�[�g�J�ڎ��̏���
		void Enter()override;
		//�X�V
		void Update()override;
		//�I������
		void Exit()override;
		//�X�e�[�g�ύX���邩�ǂ���
		std::string GetNext()override;
	};

	//���C���[�ł̌ʂ��������ړ�
	class SwingWire : public Default
	{
	public:
		SwingWire();
		SwingWire(GameObject* parent);
		~SwingWire() = default;

		//�X�e�[�g�J�ڎ��̏���
		void Enter()override;
		//�X�V
		void Update()override;
		//�I������
		void Exit()override;
		//�X�e�[�g�ύX���邩�ǂ���
		std::string GetNext()override;

	private:
		float maxWireLength = 40.0f;
		DirectX::XMFLOAT3 oldPosition[2]{};
		float swingTimer = 0.0f;
		float maxSwingTime = 0.5f;
	};

	//���C���[�ł̌ʂ��������ړ���̃W�����v
	class WireJump : public Default
	{
	public:
		WireJump();
		WireJump(GameObject* parent);
		~WireJump() = default;

		//�X�e�[�g�J�ڎ��̏���
		void Enter()override;
		//�X�V
		void Update()override;
		//�I������
		void Exit()override;
		//�X�e�[�g�ύX���邩�ǂ���
		std::string GetNext()override;

	private:
		DirectX::XMFLOAT3 moveDirection = {};
		DirectX::XMFLOAT3 oldPosition[2]{};
		float jumpSpeed = 0.0f;
		float inputRatio = 0.5f;

		float coolTimer = 0.0f;//���C���[���h�����߂̃N�[���^�C��
		float maxCoolTime = 1.0f;

		bool falling = false;
	};

	//�󒆍U��
	class JumpAttack : public Default
	{
	public:
		JumpAttack();
		JumpAttack(GameObject* parent);
		~JumpAttack() = default;

		//�X�e�[�g�J�ڎ��̏���
		void Enter()override;
		//�X�V
		void Update()override;
		//�I������
		void Exit()override;
		//�X�e�[�g�ύX���邩�ǂ���
		std::string GetNext()override;

	private:
		float attackRangeMin = 8.0f;
		float attackRangeMax = 30.0f;
		float maxMoveTime = 25.0f;//�����܂łɈړ����I����
		float attackTimer = 0.0f;
	};

	//���
	class Avoid : public Default
	{
	public:
		Avoid();
		Avoid(GameObject* parent);
		~Avoid() = default;

		//�X�e�[�g�J�ڎ��̏���
		void Enter()override;
		//�X�V
		void Update()override;
		//�I������
		void Exit()override;
		//�X�e�[�g�ύX���邩�ǂ���
		std::string GetNext()override;

	private:
		float avoidSpeed = 50.0f;
		DirectX::XMFLOAT3 avoidVec = {};
	};

	//�󒆉��
	class AvoidJump : public Default
	{
	public:
		AvoidJump();
		AvoidJump(GameObject* parent);
		~AvoidJump() = default;

		//�X�e�[�g�J�ڎ��̏���
		void Enter()override;
		//�X�V
		void Update()override;
		//�I������
		void Exit()override;
		//�X�e�[�g�ύX���邩�ǂ���
		std::string GetNext()override;

	private:
		float avoidSpeed = 50.0f;
		float avoidJumpPower = 30.0f;
		DirectX::XMFLOAT3 avoidVec = {};

	};

	//�I�u�W�F�N�g�Ƀ��C���[���h���ĐU��񂵂ē�����
	class WieldThrow : public Default
	{
	public:
		WieldThrow();
		WieldThrow(GameObject* parent);
		~WieldThrow() = default;

		//�X�e�[�g�J�ڎ��̏���
		void Enter()override;
		//�X�V
		void Update()override;
		//�I������
		void Exit()override;
		//�X�e�[�g�ύX���邩�ǂ���
		std::string GetNext()override;

	private:
		enum State
		{
			Thrust,
			Wield,
			Throw,
		};
		int state = Thrust;
	};
}