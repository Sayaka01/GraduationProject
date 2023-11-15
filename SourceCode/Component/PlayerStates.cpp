
#include "Stdafx.h"

#include "PlayerStates.h"

#include "GameObject/GameObject.h"

#include "Component/Transform.h"
#include "Component/ModelRenderer.h"
#include "Component/Camera.h"
#include "Component/RigidBody.h"
#include "Component/Player.h"
#include "Component/SphereCollider.h"
#include "Component/CapsuleCollider.h"

#include "System/SystemManager.h"
#include "System/UserFunction.h"

using namespace PlayerState;

//static�ϐ��̏�����
float Default::attackInterval = 0.0f;
bool Default::acceptAttackButton = false;
bool Default::pushAttackButton = false;
int Default::jumpCount = 0;
DirectX::XMFLOAT3 Default::parameter = { 0.0f,0.0f,0.0f };

//-----< ���N���X >-----//
void PlayerState::Default::Update()
{
	//�U���{�^����������Ă��邩�ǂ���
	if (acceptAttackButton && attackInterval > FLT_EPSILON)
	{
		//�Q�[���p�b�h�̎擾
		GamePad gamePad = SystemManager::Instance().GetGamePad();
		if (gamePad.GetButtonDown() & GamePad::BTN_B)
		{
			pushAttackButton = true;
		}
	}

	attackInterval += SystemManager::Instance().GetElapsedTime();

	if (pushAttackButton && attackInterval > acceptAttackTime)
	{
		pushAttackButton = false;
	}
}

void Default::SetMoveVelocity(DirectX::XMFLOAT3 velocity)
{
	//RigidBody�R���|�[�l���g��MoveVelocity��ݒ�
	parent->GetComponent<RigidBody>()->SetVelocity(velocity);
}
void PlayerState::Default::AddMoveVelocity(DirectX::XMFLOAT3 velocity)
{
	//RigidBody�R���|�[�l���g��MoveVelocity��ǉ�
	parent->GetComponent<RigidBody>()->AddVelocity(velocity);
}
DirectX::XMFLOAT3 Default::GetCameraFront()
{
	//�J�����̎擾
	Camera* camera = parent->GetParent()->GetChild("CameraController")->GetComponent<Camera>();

	//�J�����̑O�����x�N�g�����擾
	DirectX::XMFLOAT3 cameraFront = camera->GetCameraFront();
	cameraFront.y = 0.0f;

	return NormalizeFloat3(cameraFront);
}
DirectX::XMFLOAT3 PlayerState::Default::GetCameraRight()
{
	//�J�����̎擾
	Camera* camera = parent->GetParent()->GetChild("CameraController")->GetComponent<Camera>();

	//�J�����̉E�����x�N�g�����擾
	DirectX::XMFLOAT3 cameraRight = camera->GetCameraRight();
	cameraRight.y = 0.0f;

	return NormalizeFloat3(cameraRight);
}
DirectX::XMFLOAT3 Default::CalcMoveVec()
{
	//�J�����̑O�����x�N�g�����擾
	DirectX::XMFLOAT3 cameraFront = GetCameraFront();
	//�J�����̉E�����x�N�g�����擾
	DirectX::XMFLOAT3 cameraRight = GetCameraRight();

	//L�X�e�B�b�N�̓��͒l���擾
	DirectX::XMFLOAT2 lStickVec = GetLStickVec();
	//�J�����̃x�N�g�����琅�������̈ړ��x�N�g�����Z�o
	DirectX::XMFLOAT3 vec =
	{
		cameraFront.x * lStickVec.y + cameraRight.x * lStickVec.x,
		0.0f,
		cameraFront.z * lStickVec.y + cameraRight.z * lStickVec.x,
	};

	return NormalizeFloat3(vec);
}
DirectX::XMFLOAT2 Default::GetLStickVec()
{
	GamePad gamePad = SystemManager::Instance().GetGamePad();
	return DirectX::XMFLOAT2(gamePad.GetAxisLX() ,gamePad.GetAxisLY());
}
void Default::YAxisRotate(DirectX::XMFLOAT3 moveVelocity)
{
	//�����̈ړ��x�N�g�����[���x�N�g���̏ꍇ�͉�]�������s��Ȃ�
	moveVelocity.y = 0.0f;
	const float length = LengthFloat3(moveVelocity);
	if (length < FLT_EPSILON)return;

	//�ړ��x�N�g���𐳋K������
	moveVelocity = NormalizeFloat3(moveVelocity);

	//��]�������߂�
	DirectX::XMFLOAT3 up = { 0.0f, 1.0f, 0.0f };
	DirectX::XMVECTOR Up = DirectX::XMLoadFloat3(&up);

	//player�̃��[���h�s��
	DirectX::XMFLOAT4X4 world = parent->GetComponent<Transform>()->world;

	//���g�̉�]�l����O���������߂�
	DirectX::XMFLOAT3 frontVec = NormalizeFloat3({ world._31,world._32 ,world._33 });
	up = frontVec;
	//��]�p�����߂�
	const float dot = (frontVec.x * moveVelocity.x) + (frontVec.z * moveVelocity.z);

	//��]���x�𒲐�����
	float rot = acosf(dot);

	//���E����
	float cross = (frontVec.x * moveVelocity.z) - (frontVec.z * moveVelocity.x);

	//orientation�̎擾
	DirectX::XMFLOAT4 orientation = parent->GetComponent<Transform>()->orientation;
	DirectX::XMVECTOR OldOrientation = DirectX::XMLoadFloat4(&orientation);

	DirectX::XMVECTOR Q;
	float speed = rotateSpeed * SystemManager::Instance().GetElapsedTime();
	if (speed > rot) speed = rot;
	if (cross < 0.0f)
	{
		Q = DirectX::XMQuaternionRotationAxis(Up, speed);
	}
	else
	{
		Q = DirectX::XMQuaternionRotationAxis(Up, -speed);
	}
	DirectX::XMVECTOR Orientation = DirectX::XMQuaternionMultiply(OldOrientation, Q);

	Orientation = DirectX::XMQuaternionSlerp(OldOrientation, Orientation, rotateRatio);

	DirectX::XMStoreFloat4(&orientation, Orientation);

	parent->GetComponent<Transform>()->orientation = orientation;
}
void PlayerState::Default::CalcEnemyDistance()
{
	//���͓G��1�̂Ȃ̂ł��̂����B��Ŋ撣��
	GameObject* enemyManager = parent->GetParent()->GetChild("enemyManager");
	DirectX::SimpleMath::Vector3 playerPos = parent->GetComponent<Transform>()->pos;
	if (enemyManager != nullptr)
	{
		int enemyCounut = int(enemyManager->GetChildrenCount());
		float minLength = FLT_MAX;
		if (enemyCounut > 0)
		{
			for (int i = 0; i < enemyCounut; i++)
			{
				DirectX::SimpleMath::Vector3 enemyPos= enemyManager->GetChild("enemy_" + std::to_string(i))->GetComponent<Transform>()->pos;
				DirectX::SimpleMath::Vector3 vec = playerPos - enemyPos;
				float length = vec.Length();

				if (minLength > length)
				{
					minLength = length;
					parameter = enemyPos;
				}
			}
			return;
		}
	}

	//�G��������Ȃ��ꍇ�͎������g�̈ʒu��Ԃ�
	parameter = playerPos;

	//GameObject* enemy = parent->GetParent()->GetChild("enemy");
	//parameter = enemy->GetComponent<Transform>()->pos;
}

bool PlayerState::Default::JudgeIdleState()
{
	//L�X�e�B�b�N�̓��͒l���擾
	DirectX::XMFLOAT2 lStickVec = GetLStickVec();
	//L�X�e�B�b�N�̓��͂��Ȃ��Ȃ�ҋ@�X�e�[�g�֑J��
	return (LengthFloat2(lStickVec) < FLT_EPSILON);
}
bool PlayerState::Default::JudgeRunState()
{
	//L�X�e�B�b�N�̓��͒l���擾
	DirectX::XMFLOAT2 lStickVec = GetLStickVec();
	//L�X�e�B�b�N�̓��͂�����Ȃ瑖��X�e�[�g�֑J��
	return (LengthFloat2(lStickVec) > 0.0f);
}
bool PlayerState::Default::JudgeJumpState()
{
	if (jumpCount >= maxJumpCount)return false;

	//�Q�[���p�b�h�̎擾
	GamePad gamePad = SystemManager::Instance().GetGamePad();
	//A�{�^����������Ă�����true
	return (gamePad.GetButtonDown() & GamePad::BTN_A);
}
bool PlayerState::Default::JudgePunchRightState()
{
	//�Q�[���p�b�h�̎擾
	GamePad gamePad = SystemManager::Instance().GetGamePad();
	//A�{�^����������Ă�����true
	return (gamePad.GetButtonDown() & GamePad::BTN_B);
}
bool PlayerState::Default::JudgePunchLeftState()
{
	//�Q�[���p�b�h�̎擾
	GamePad gamePad = SystemManager::Instance().GetGamePad();
	//A�{�^����������Ă�����true
	return (pushAttackButton && (attackInterval < acceptAttackTime));
}
bool PlayerState::Default::JudgeAimWireState()
{
	return false;

	//�Q�[���p�b�h�̎擾
	GamePad gamePad = SystemManager::Instance().GetGamePad();

	//ZL�{�^����������Ă�����true
	return (gamePad.GetButtonDown() & GamePad::BTN_LEFT_SHOULDER);
}
bool PlayerState::Default::JudgeSwingWireState()
{
	//�Q�[���p�b�h�̎擾
	GamePad gamePad = SystemManager::Instance().GetGamePad();

	//ZR�{�^����������Ă�����true
	return (gamePad.GetButtonDown() & GamePad::BTN_RIGHT_SHOULDER);
}

//-----< �ҋ@ >-----//
Idle::Idle()
{
	name = "Idle";
}
Idle::Idle(GameObject* parent)
{
	name = "Idle";
	this->parent = parent;
}
void Idle::Enter()
{
	parent->GetComponent<ModelRenderer>()->PlayAnimation((int)Animation::Idle, true);
	jumpCount = 0;
}
void Idle::Update()
{
	Default::Update();
}
void Idle::Exit()
{

}
std::string Idle::GetNext()
{
	//�p���`�i���j�X�e�[�g�֑J�ڂł��邩
	if (JudgePunchLeftState())
	{
		return "PunchLeft";
	}

	//�p���`�i�E�j�X�e�[�g�֑J�ڂł��邩
	if (JudgePunchRightState())
	{
		return "PunchRight";
	}

	//���C���[�ł̒����ړ��X�e�[�g�ɑJ�ڂł��邩
	if (JudgeAimWireState())
	{
		return "AimWire";
	}

	//�W�����v�X�e�[�g�֑J�ڂł��邩
	if (JudgeJumpState())
	{
		return "Jump";
	}

	//����X�e�[�g�֑J�ڂł��邩
	if (JudgeRunState())
	{
		return "Run";
	}
	
	//�ύX�Ȃ�
	return "";
}

//-----< ���� >-----//
Run::Run()
{
	name = "Run";
}
Run::Run(GameObject* parent)
{
	name = "Run";
	this->parent = parent;
}
void Run::Enter()
{
	parent->GetComponent<ModelRenderer>()->PlayAnimation((int)Animation::Running, true);
}
void Run::Update()
{
	DirectX::XMFLOAT3 moveVelocity = CalcMoveVec();
	moveVelocity *= parent->GetComponent<Player>()->GetRunSpeed();

	AddMoveVelocity(moveVelocity);
	YAxisRotate(moveVelocity);

	Default::Update();
}
void Run::Exit()
{

}
std::string Run::GetNext()
{
	//�p���`�i���j�X�e�[�g�֑J�ڂł��邩
	if (JudgePunchLeftState())
	{
		return "PunchLeft";
	}

	//�p���`�i�E�j�X�e�[�g�֑J�ڂł��邩
	if (JudgePunchRightState())
	{
		return "PunchRight";
	}

	//���C���[�ł̒����ړ��X�e�[�g�ɑJ�ڂł��邩
	if (JudgeAimWireState())
	{
		return "AimWire";
	}

	//�W�����v�X�e�[�g�֑J�ڂł��邩
	if (JudgeJumpState())
	{
		return "Jump";
	}

	//�ҋ@�X�e�[�g�֑J�ڂł��邩�ǂ���
	if (JudgeIdleState())
	{
		return "Idle";
	}

	//�ύX�Ȃ�
	return "";
}

//-----< �W�����v >-----//
Jump::Jump()
{
	name = "Jump";
}
Jump::Jump(GameObject* parent)
{
	name = "Jump";
	this->parent = parent;
}
void Jump::Enter()
{
	jumpCount++;
	if (jumpCount <= 1)//�W�����v����
		parent->GetComponent<ModelRenderer>()->PlayAnimation((int)Animation::Jump, false);
	else//����ȍ~
		parent->GetComponent<ModelRenderer>()->PlayAnimation((int)Animation::JumpFlip, false);

	parent->GetComponent<RigidBody>()->Jump(parent->GetComponent<Player>()->GetJumpSpeed());

}
void Jump::Update()
{
	DirectX::XMFLOAT3 moveVelocity = CalcMoveVec();
	moveVelocity *= parent->GetComponent<Player>()->GetRunSpeed();

	AddMoveVelocity(moveVelocity);
	YAxisRotate(moveVelocity);

	Default::Update();
}
void Jump::Exit()
{

}
std::string Jump::GetNext()
{
	//�󒆍U���X�e�[�g�ɑJ�ڂł��邩
	if (JudgePunchRightState())
	{
		return "JumpAttack";
	}

	//���C���[�ł̌ʂ��������ړ��X�e�[�g�ɑJ�ڂł��邩
	if (JudgeSwingWireState())
	{
		return "SwingWire";
	}

	//���C���[�ł̒����ړ��X�e�[�g�ɑJ�ڂł��邩
	if (JudgeAimWireState())
	{
		return "AimWire";
	}

	//�W�����v�X�e�[�g�֑J�ڂł��邩
	if (JudgeJumpState())
	{
		return "Jump";
	}

	//�A�j���[�V�����Đ����I������痎���X�e�[�g�֑J��
	if (parent->GetComponent<ModelRenderer>()->IsFinishAnimation())
	{
		return "Falling";
	}

	//pos.y < 0.0f �Ȃ璅�n�X�e�[�g��
	if (parent->GetComponent<Transform>()->pos.y < 0.0f)
	{
		return "Landing";
	}

	//�ύX�Ȃ�
	return "";
}

//-----< ���� >-----//
Falling::Falling()
{
	name = "Falling";
}
Falling::Falling(GameObject* parent)
{
	name = "Falling";
	this->parent = parent;
}
void Falling::Enter()
{
	parent->GetComponent<ModelRenderer>()->PlayAnimation((int)Animation::Falling, true);

}
void Falling::Update()
{
	DirectX::XMFLOAT3 moveVelocity = CalcMoveVec();
	moveVelocity *= parent->GetComponent<Player>()->GetRunSpeed();

	AddMoveVelocity(moveVelocity);
	YAxisRotate(moveVelocity);

	Default::Update();
}
void Falling::Exit()
{

}
std::string Falling::GetNext()
{
	//�󒆍U���X�e�[�g�ɑJ�ڂł��邩
	if (JudgePunchRightState())
	{
		return "JumpAttack";
	}

	//���C���[�ł̌ʂ��������ړ��X�e�[�g�ɑJ�ڂł��邩
	if (JudgeSwingWireState())
	{
		return "SwingWire";
	}

	//���C���[�ł̒����ړ��X�e�[�g�ɑJ�ڂł��邩
	if (JudgeAimWireState())
	{
		return "AimWire";
	}

	//pos.y < 0.0f �Ȃ璅�n�X�e�[�g��
	if (parent->GetComponent<Transform>()->pos.y < 0.0f)
	{
		return "Landing";
	}

	//�W�����v�X�e�[�g�֑J�ڂł��邩
	if (JudgeJumpState())
	{
		return "Jump";
	}


	//�ύX�Ȃ�
	return "";
}

//-----< ���n >-----//
Landing::Landing()
{
	name = "Landing";
}
Landing::Landing(GameObject* parent)
{
	name = "Landing";
	this->parent = parent;
}
void Landing::Enter()
{
	parent->GetComponent<ModelRenderer>()->PlayAnimation((int)Animation::Landing, false);

	jumpCount = 0;
}
void Landing::Update()
{
	Default::Update();
}
void Landing::Exit()
{

}
std::string Landing::GetNext()
{
	//���C���[�ł̒����ړ��X�e�[�g�ɑJ�ڂł��邩
	if (JudgeAimWireState())
	{
		return "AimWire";
	}

	//����X�e�[�g�֑J�ڂł��邩
	if (JudgeRunState())
	{
		return "Run";
	}

	//�A�j���[�V�����Đ����I������痎���X�e�[�g�֑J��
	if (parent->GetComponent<ModelRenderer>()->IsFinishAnimation())
	{
		return "Idle";
	}

	//�ύX�Ȃ�
	return "";
}

//-----< �p���`�i�E�j >-----//
PunchRight::PunchRight()
{
	name = "PunchRight";
}
PunchRight::PunchRight(GameObject* parent)
{
	name = "PunchRight";
	this->parent = parent;
}
void PunchRight::Enter()
{
	//�A�j���[�V�����̍Đ�
	parent->GetComponent<ModelRenderer>()->PlayAnimation((int)Animation::Punching, false);
	//�A�j���[�V�����X�s�[�h�̒���
	parent->GetComponent<ModelRenderer>()->SetAnimationSpeed(2.5f);
	
	//������U������̂ŏ�����
	attackInterval = 0.0f;
	
	//2�i�ȍ~�̍U���̓��͂̎擾���J�n
	acceptAttackButton = true;

	//�U���p�����蔻��̃R���|�[�l���g��L����
	parent->GetComponent<SphereCollider>("RightHandSphere")->SetEnable(true);

	//�U���͂�ݒ�
	attackPower = 1.0f;

	//��ԋ߂��G�̈ʒu��parameter�Ɋi�[
	CalcEnemyDistance();
}
void PunchRight::Update()
{
	DirectX::XMFLOAT3 vec = parameter - parent->GetComponent<Transform>()->pos;
	vec.y = 0.0f;
	float length = LengthFloat3(vec);
	if (length < attackRangeMax)
	{
		YAxisRotate(vec);
		if (length > attackRangeMin)
			AddMoveVelocity(NormalizeFloat3(vec) * parent->GetComponent<Player>()->GetRunSpeed());
	}
		

	Default::Update();
}
void PunchRight::Exit()
{
	parent->GetComponent<ModelRenderer>()->SetAnimationSpeed(1.0f);
	parent->GetComponent<SphereCollider>("RightHandSphere")->SetEnable(false);
}
std::string PunchRight::GetNext()
{

	//�A�j���[�V�����Đ����I�������
	if (parent->GetComponent<ModelRenderer>()->IsFinishAnimation())
	{
		//�p���`�i���j�X�e�[�g�֑J�ڂł��邩
		if (JudgePunchLeftState())
		{
			return "PunchLeft";
		}

		//�ҋ@�X�e�[�g�֑J��
		return "Idle";
	}

	//�ύX�Ȃ�
	return "";
}

//-----< �p���`�i���j >-----//
PunchLeft::PunchLeft()
{
	name = "PunchLeft";
}
PunchLeft::PunchLeft(GameObject* parent)
{
	name = "PunchLeft";
	this->parent = parent;
}
void PunchLeft::Enter()
{
	//�A�j���[�V�����̍Đ�
	parent->GetComponent<ModelRenderer>()->PlayAnimation((int)Animation::Attack, false);
	//�A�j���[�V�����X�s�[�h�̒���
	parent->GetComponent<ModelRenderer>()->SetAnimationSpeed(1.5f);

	//������U������̂ŏ�����
	attackInterval = 0.0f;

	//�U���̓��̓t���O�Ȃǂ�������
	acceptAttackButton = false;
	pushAttackButton = false;

	//�U���p�����蔻��̃R���|�[�l���g��L����
	parent->GetComponent<SphereCollider>("LeftHandSphere")->SetEnable(true);

	//�U���͂�ݒ�
	attackPower = 3.0f;

	//��ԋ߂��G�̈ʒu��parameter�Ɋi�[
	CalcEnemyDistance();
}
void PunchLeft::Update()
{
	DirectX::XMFLOAT3 vec = parameter - parent->GetComponent<Transform>()->pos;
	vec.y = 0.0f;
	float length = LengthFloat3(vec);
	if (length < attackRangeMax)
	{
		YAxisRotate(vec);
		if (length > attackRangeMin)
			AddMoveVelocity(NormalizeFloat3(vec) * parent->GetComponent<Player>()->GetRunSpeed());
	}

	Default::Update();
}
void PunchLeft::Exit()
{
	parent->GetComponent<SphereCollider>("LeftHandSphere")->SetEnable(false);
	//�A�j���[�V�����X�s�[�h�̒���
	parent->GetComponent<ModelRenderer>()->SetAnimationSpeed(1.0f);
}
std::string PunchLeft::GetNext()
{
	//�A�j���[�V�����Đ����I������痎���X�e�[�g�֑J��
	if (parent->GetComponent<ModelRenderer>()->IsFinishAnimation())
	{
		return "Idle";
	}

	//�ύX�Ȃ�
	return "";
}

//-----< �_���[�W >-----//
Damage::Damage()
{
	name = "Damage";
}
Damage::Damage(GameObject* parent)
{
	name = "Damage";
	this->parent = parent;
}
void Damage::Enter()
{
	parent->GetComponent<ModelRenderer>()->PlayAnimation((int)Animation::GetHit2, false);
}
void Damage::Update()
{
	//�m�b�N�o�b�N��velocity
	AddMoveVelocity(parameter);

	Default::Update();
}
void Damage::Exit()
{

}
std::string Damage::GetNext()
{
	//�A�j���[�V�����Đ����I�������ҋ@�X�e�[�g�֑J��
	if (parent->GetComponent<ModelRenderer>()->IsFinishAnimation())
	{
		if (parent->GetComponent<Transform>()->pos.y > 0.0f)
			return "Falling";
		else
			return "Idle";
	}

	//�ύX�Ȃ�
	return "";
}

//-----< ���S >-----//
Death::Death()
{
	name = "Death";
}
Death::Death(GameObject* parent)
{
	name = "Death";
	this->parent = parent;
}
void Death::Enter()
{
	parent->GetComponent<ModelRenderer>()->PlayAnimation((int)Animation::Death, false);

}
void Death::Update()
{
	// �A�j���[�V�������I������玀�S�t���O�𗧂Ă�
	if (parent->GetComponent<ModelRenderer>()->IsFinishAnimation())
	{
		parent->GetComponent<Player>()->SetIsDead(true);
	}
}
void Death::Exit()
{

}
std::string Death::GetNext()
{
	//�ύX�Ȃ�
	return "";
}

//-----< �_�����ʒu�Ƀ��C���[���h���A�����I�ȃ��C���[�ړ� >-----//
AimWire::AimWire()
{
	name = "AimWire";
}
AimWire::AimWire(GameObject* parent)
{
	name = "AimWire";
	this->parent = parent;
}
void AimWire::Enter()
{
	jumpCount = 0;

	parent->GetComponent<ModelRenderer>()->PlayAnimation((int)Animation::Jump, false);

}
void AimWire::Update()
{
	Default::Update();
}
void AimWire::Exit()
{

}
std::string AimWire::GetNext()
{
	//�W�����v�X�e�[�g�֑J�ڂł��邩
	if (JudgeJumpState())
	{
		return "Jump";
	}

	//�A�j���[�V�����Đ����I������痎���X�e�[�g�֑J��
	if (parent->GetComponent<ModelRenderer>()->IsFinishAnimation())
	{
		return "Falling";
	}

	//�ύX�Ȃ�
	return "";
}

//-----< ���C���[�ł̌ʂ��������ړ� >-----//
SwingWire::SwingWire()
{
	name = "SwingWire";
}
SwingWire::SwingWire(GameObject* parent)
{
	name = "SwingWire";
	this->parent = parent;
}
void SwingWire::Enter()
{
	jumpCount = 0;

	//�J�����̑O�����x�N�g�����擾
	DirectX::XMFLOAT3 cameraFront = GetCameraFront();

	//���C���[�̎h���ʒu�i���j
	//���C���[���ΕӂƂ���90��,30��,60���̒��p�O�p�`�Ƃ݂Čv�Z����
	DirectX::XMFLOAT3 pos = parent->GetComponent<Transform>()->pos;
	parameter = pos + (cameraFront * (maxWireLength / sqrtf(2.0f)));//�����̈ʒu���烏�C���[���h��XZ���ʏ�̓_
	parameter.y += maxWireLength / sqrtf(2.0f);//���C���[���h���ʒu

	//������
	oldPosition[0] = pos;
	oldPosition[1] = pos;

	//Debug�`��
	parent->GetComponent<CapsuleCollider>("WireCapsule")->SetEnable(true);
	parent->GetComponent<CapsuleCollider>("WireCapsule")->begin = parameter;
	pos.y += parent->GetComponent<Player>()->GetHeight();
	parent->GetComponent<CapsuleCollider>("WireCapsule")->end = pos;

	//parent->GetComponent<RigidBody>()->SetUseGravity(false);
	parent->GetComponent<ModelRenderer>()->PlayAnimation((int)Animation::StylishFlip, true);
	

	swingTimer = 0.0f;
}
void SwingWire::Update()
{	
	//�o�ߎ���
	swingTimer += SystemManager::Instance().GetElapsedTime();

	//�J�����̑O�����ɐi��(XZ���ʁj
	DirectX::XMFLOAT3 velocity = GetCameraFront() * parent->GetComponent<Player>()->GetWireSpeed();
	SetMoveVelocity(velocity);
	YAxisRotate(velocity);

	DirectX::XMFLOAT3 pos = parent->GetComponent<Transform>()->pos;

	//���C���[�̒����Ɋ�Â��ĕ␳
	DirectX::XMFLOAT3 vec = pos - parameter;
	vec = NormalizeFloat3(vec) * maxWireLength;
	pos = parameter + vec;
	parent->GetComponent<Transform>()->pos = pos;

	//�ʒu�̕ۑ�
	oldPosition[0] = oldPosition[1];
	oldPosition[1] = pos;

	//Debug�`��
	pos.y += parent->GetComponent<Player>()->GetHeight();
	parent->GetComponent<CapsuleCollider>("WireCapsule")->end = pos;

	parent->GetComponent<SphereCollider>("DebugSphere")->center = pos;

	Default::Update();
}
void SwingWire::Exit()
{
	parent->GetComponent<CapsuleCollider>("WireCapsule")->SetEnable(false);

	parent->GetComponent<RigidBody>()->SetUseGravity(true);

	parameter = oldPosition[0];
}
std::string SwingWire::GetNext()
{
	//�W�����v�X�e�[�g�֑J�ڂł��邩
	if (JudgeJumpState())
	{
		return "WireJump";
	}

	DirectX::XMFLOAT3 pos = parent->GetComponent<Transform>()->pos;
	float length = LengthFloat3(oldPosition[1] - oldPosition[0]);
	float l = parent->GetComponent<Player>()->GetWireSpeed() * SystemManager::Instance().GetElapsedTime();
	if (length < l * 0.05f || swingTimer > maxSwingTime)
	{
		return "WireJump";
	}

	//ZR�{�^���𗣂�����Jump�X�e�[�g�ɑJ��
	if (SystemManager::Instance().GetGamePad().GetButtonUp() & GamePad::BTN_RIGHT_SHOULDER)
	{
		return "WireJump";
	}

	//pos.y < 0.0f �Ȃ璅�n�X�e�[�g��
	if (parent->GetComponent<Transform>()->pos.y < 0.0f)
	{
		return "Landing";
	}


	//�ύX�Ȃ�
	return "";
}

//-----< ���C���[�ł̌ʂ��������ړ���̃W�����v >-----//
WireJump::WireJump()
{
	name = "WireJump";
}
WireJump::WireJump(GameObject* parent)
{
	name = "WireJump";
	this->parent = parent;
}
void WireJump::Enter()
{
	//�W�����v����x�N�g�����v�Z
	DirectX::XMFLOAT3 pos = parent->GetComponent<Transform>()->pos;
	DirectX::XMFLOAT3 vec = pos - parameter;
	moveDirection = vec / SystemManager::Instance().GetElapsedTime();
	jumpSpeed = (moveDirection.y < 0.0f ? 0.0f : moveDirection.y) + parent->GetComponent<Player>()->GetJumpSpeed();
	moveDirection.y = 0.0f;

	//������
	oldPosition[0] = pos;
	oldPosition[1] = pos;

	//�W�����v�J�E���g�̍X�V
	jumpCount++;
	//�A�j���[�V�����̐؂�ւ�
	if (jumpCount <= 1)//�W�����v����
		parent->GetComponent<ModelRenderer>()->PlayAnimation((int)Animation::Jump, false);
	else//����ȍ~
		parent->GetComponent<ModelRenderer>()->PlayAnimation((int)Animation::JumpFlip, false);

	//�W�����v
	parent->GetComponent<RigidBody>()->Jump(jumpSpeed);

	falling = false;
}
void WireJump::Update()
{	
	//�ʒu�̕ۑ�
	DirectX::XMFLOAT3 pos = parent->GetComponent<Transform>()->pos;
	oldPosition[0] = oldPosition[1];
	oldPosition[1] = pos;

	DirectX::XMFLOAT3 inputVec = CalcMoveVec();
	DirectX::XMFLOAT3 moveVec{};
	if (LengthFloat3(inputVec) > FLT_EPSILON)
		moveVec = (inputVec * parent->GetComponent<Player>()->GetRunSpeed() * inputRatio) + (moveDirection * (1.0f - inputRatio));
	else
		moveVec = moveDirection;

	//�J�����̑O�����ɐi��(XZ���ʁj
	AddMoveVelocity(moveVec);
	YAxisRotate(inputVec);

	//�A�j���[�V�����Đ����I������痎���X�e�[�g�֑J��
	if (parent->GetComponent<ModelRenderer>()->IsFinishAnimation())
	{
		//�A�j���[�V�����̐؂�ւ�
		parent->GetComponent<ModelRenderer>()->PlayAnimation((int)Animation::Falling, true);

		falling = true;
	}

	
	Default::Update();
}
void WireJump::Exit()
{
}
std::string WireJump::GetNext()
{

	//���C���[�ł̌ʂ��������ړ��X�e�[�g�ɑJ�ڂł��邩
	if (JudgeSwingWireState() && falling)
	{
		return "SwingWire";
	}

	//���C���[�ł̒����ړ��X�e�[�g�ɑJ�ڂł��邩
	if (JudgeAimWireState() && falling)
	{
		return "AimWire";
	}

	//�W�����v�X�e�[�g�֑J�ڂł��邩
	if (JudgeJumpState())
	{
		parameter = oldPosition[0];

		return "WireJump";
	}

	//pos.y < 0.0f �Ȃ璅�n�X�e�[�g��
	if (parent->GetComponent<Transform>()->pos.y < 0.0f)
	{
		return "Landing";
	}

	//�ύX�Ȃ�
	return "";
}

//-----< �󒆍U���i�E�p���`�j >-----//
JumpAttack::JumpAttack()
{
	name = "JumpAttack";
}
JumpAttack::JumpAttack(GameObject* parent)
{
	name = "JumpAttack";
	this->parent = parent;
}
void JumpAttack::Enter()
{
	//�A�j���[�V�����̍Đ�
	parent->GetComponent<ModelRenderer>()->PlayAnimation((int)Animation::HookPunch, false);
	//�A�j���[�V�����̍Đ����x�̕ύX
	parent->GetComponent<ModelRenderer>()->SetAnimationSpeed(3.0f);

	//�U���p�����蔻��̃R���|�[�l���g��L����
	parent->GetComponent<SphereCollider>("RightHandSphere")->SetEnable(true);

	//�U���͂�ݒ�
	attackPower = 1.0f;

	//��ԋ߂��G�̈ʒu��parameter�Ɋi�[
	CalcEnemyDistance();

	//�ړ��x�N�g�����v�Z
	DirectX::XMFLOAT3 vec = parameter - parent->GetComponent<Transform>()->pos;
	float length = LengthFloat3(vec);

	parameter = { 0.0f,0.0f,0.0f };

	if (length < attackRangeMax)
	{
		if (length > attackRangeMin)
		{
			float animSpeed = parent->GetComponent<ModelRenderer>()->GetAnimationSpeed();
			parameter = NormalizeFloat3(vec) * (length / maxMoveTime * animSpeed);
			
			//�d�͂𖳎�����
			parent->GetComponent<RigidBody>()->SetUseGravity(false);
		}
	}

	//2�i�ȍ~�̍U���̓��͂̎擾���J�n
	acceptAttackButton = true;

	//�U��������������
	attackTimer = 0.0f;

	//������U������̂ŏ�����
	attackInterval = 0.0f;

}
void JumpAttack::Update()
{	
	float animSpeed = parent->GetComponent<ModelRenderer>()->GetAnimationSpeed();
	attackTimer += SystemManager::Instance().GetElapsedTime() * animSpeed;

	if (attackTimer < maxMoveTime && LengthFloat3(parameter) > FLT_EPSILON)
	{
		SetMoveVelocity(parameter);
		YAxisRotate(parameter);
	}
	else
	{
		parent->GetComponent<RigidBody>()->SetUseGravity(true);
	}

	//DirectX::XMFLOAT3 vec = parameter - parent->GetComponent<Transform>()->pos;
	//vec.y = 0.0f;
	//float length = LengthFloat3(vec);
	//if (length < attackRangeMax)
	//{
	//	YAxisRotate(vec);
	//	if (length > attackRangeMin)
	//		AddMoveVelocity(NormalizeFloat3(vec) * parent->GetComponent<Player>()->GetWireSpeed());
	//}

	Default::Update();

}
void JumpAttack::Exit()
{
	parent->GetComponent<SphereCollider>("RightHandSphere")->SetEnable(false);
	//�A�j���[�V�����̍Đ����x�̕ύX
	parent->GetComponent<ModelRenderer>()->SetAnimationSpeed(1.0f);


}
std::string JumpAttack::GetNext()
{
	//�A�j���[�V�����Đ����I������痎���X�e�[�g�֑J��
	if (parent->GetComponent<ModelRenderer>()->IsFinishAnimation())
	{
		return "Idle";
	}

	//�ύX�Ȃ�
	return "";
}

