
#include "Stdafx.h"

#include "PlayerStates.h"

#include "GameObject/GameObject.h"

#include "Component/Transform.h"
#include "Component/ModelRenderer.h"
#include "Component/Camera.h"
#include "Component/RigidBody.h"
#include "Component/Player.h"

#include "System/SystemManager.h"
#include "System/UserFunction.h"

using namespace PlayerState;

//static�ϐ��̏�����
float Default::attackInterval = 0.0f;
bool Default::acceptAttackButton = false;
bool Default::pushAttackButton = false;
int Default::jumpCount = 0;

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
	//�v���C���[�R���|�[�l���g��MoveVelocity��ݒ�
	parent->GetComponent<RigidBody>()->AddVelocity(velocity);
}
DirectX::XMFLOAT3 Default::CalcMoveVec()
{
	//�J�����̎擾
	Camera* camera = parent->GetParent()->GetChild("CameraController")->GetComponent<Camera>();

	//�J�����̑O�����x�N�g�����擾
	DirectX::XMFLOAT3 cameraFront = camera->GetCameraFront();
	cameraFront.y = 0.0f;
	cameraFront = NormalizeFloat3(cameraFront);
	//�J�����̉E�����x�N�g�����擾
	DirectX::XMFLOAT3 cameraRight = camera->GetCameraRight();
	cameraRight.y = 0.0f;
	cameraRight = NormalizeFloat3(cameraRight);

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

	SetMoveVelocity(moveVelocity);
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
	parent->GetComponent<ModelRenderer>()->PlayAnimation((int)Animation::Jump, false);
	parent->GetComponent<RigidBody>()->Jump(parent->GetComponent<Player>()->GetJumpSpeed());
	//SetMoveVelocity({ 0.0f, parent->GetComponent<Player>()->GetJumpSpeed(), 0.0f });
}
void Jump::Update()
{
	Default::Update();
}
void Jump::Exit()
{

}
std::string Jump::GetNext()
{
	//�A�j���[�V�����Đ����I������痎���X�e�[�g�֑J��
	if (parent->GetComponent<ModelRenderer>()->IsFinishAnimation())
	{
		return "Falling";
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
	Default::Update();
}
void Falling::Exit()
{

}
std::string Falling::GetNext()
{
	//pos.y < 0.0f �Ȃ璅�n�X�e�[�g��
	if (parent->GetComponent<Transform>()->pos.y < 0.0f)
	{
		return "Landing";
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
	parent->GetComponent<ModelRenderer>()->PlayAnimation((int)Animation::Punching, false);
	parent->GetComponent<ModelRenderer>()->SetAnimationSpeed(2.0f);
	
	attackInterval = 0.0f;
	
	acceptAttackButton = true;
}
void PunchRight::Update()
{
	OutputDebugLog("attackInterval", attackInterval);
	Default::Update();
}
void PunchRight::Exit()
{
	parent->GetComponent<ModelRenderer>()->SetAnimationSpeed(1.0f);
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
	parent->GetComponent<ModelRenderer>()->PlayAnimation((int)Animation::Attack, false);

	attackInterval = 0.0f;

	acceptAttackButton = false;
	pushAttackButton = false;
}
void PunchLeft::Update()
{
	Default::Update();
}
void PunchLeft::Exit()
{

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
