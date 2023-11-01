
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

//���N���X
void Default::SetMoveVelocity(DirectX::XMFLOAT3 velocity)
{
	//�v���C���[�R���|�[�l���g��MoveVelocity��ݒ�
	parent->GetComponent<RigidBody>()->AddForce(velocity);
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

//�ҋ@
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
	SetMoveVelocity({ 0.0f,0.0f,0.0f });
}
void Idle::Update()
{

}
void Idle::Exit()
{

}
std::string Idle::Judge()
{
	//L�X�e�B�b�N�̓��͒l���擾
	DirectX::XMFLOAT2 lStickVec = GetLStickVec();

	//L�X�e�B�b�N�̓��͂�����Ȃ瑖��X�e�[�g�֑J��
	if (LengthFloat2(lStickVec) > 0.0f)
	{
		return "Run";
	}
	
	//�ύX�Ȃ�
	return "";
}

//����
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
}
void Run::Exit()
{

}
std::string Run::Judge()
{
	//L�X�e�B�b�N�̓��͒l���擾
	DirectX::XMFLOAT2 lStickVec = GetLStickVec();

	//L�X�e�B�b�N�̓��͂��Ȃ��Ȃ�ҋ@�X�e�[�g�֑J��
	if (LengthFloat2(lStickVec) < FLT_EPSILON)
	{
		return "Idle";
	}

	//�ύX�Ȃ�
	return "";
}
