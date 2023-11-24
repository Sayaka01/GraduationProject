
#include "Stdafx.h"

#include  "Camera.h"

#include "GameObject/GameObject.h"
#include "System/Common.h"

#include "Component/Transform.h"
#include "System/SystemManager.h"
#include "System/UserFunction.h"

#include "Player.h"
#include "Component/RigidBody.h"

//������
void Camera::Initialize()
{
	name = "Camera";

	cameraData = std::make_unique<CameraData>();
	constexpr float aspect = static_cast<float>(SCREEN_WIDTH) / static_cast<float>(SCREEN_HEIGHT);
	cameraData->SetPerspectiveFov(FovRadY, aspect, nearZ, farZ);

	//cameraData->SetAngle({ DirectX::XMConvertToRadians(10),0.0f,0.0f });

	//cameraData->SetTarget(parent->GetParent()->GetChild("player")->GetComponent<Transform>()->pos);

	cameraData->Initialize();

	cameraData->SetTargetCorrection(targetCorrection);
}

//�X�V
void Camera::Update()
{
	MoveCameraAngle();

	SetEyePoint();

	Tracking();

	parent->GetComponent<Transform>()->pos = cameraData->Update();

	//�Q�[���p�b�h�̎擾
	GamePad gamePad = SystemManager::Instance().GetGamePad();
	if (gamePad.GetButtonUp() & GamePad::BTN_RIGHT_THUMB)// R�X�e�B�b�N��������
	{
		//�v���C���[�̑O�����փJ���������Z�b�g

	}

	// �J�����̋����𓮂���
	ChangeRange();
}

//�`��
void Camera::Draw()
{
	UpdateBuffer();
}

//ImGui
void Camera::DebugGui()
{
	if (ImGui::TreeNode(name.c_str()))
	{
		cameraData->DebugGui();
		if (ImGui::Button("reset"))
		{
			ResetDefault();
		}
#if 0
		ImGui::DragFloat("fov", &FovRadY, 0.1f);
		constexpr float aspect = static_cast<float>(SCREEN_WIDTH) / static_cast<float>(SCREEN_HEIGHT);
		cameraData->SetPerspectiveFov(FovRadY, aspect, nearZ, farZ);
#endif
		ImGui::TreePop();
	}

}

void Camera::UpdateBuffer() const
{
	cameraData->UpdateBuffer(parent->GetComponent<Transform>()->pos);
}

void Camera::MoveCameraAngle()
{
	const GamePad& gamepad = SystemManager::Instance().GetGamePad();

	const float rx = gamepad.GetAxisRX();
	const float ry = gamepad.GetAxisRY();

	bool inputX = SetAngleToX();
	bool inputY = SetAngleToY();

	//���͒l���Ȃ���΍X�V���Ȃ�
	if (!inputX && !inputY)return;

	DirectX::XMFLOAT3 eyeV = cameraData->GetEyeVector();

	DirectX::XMVECTOR EyeVector = DirectX::XMLoadFloat3(&eyeV);
	DirectX::XMFLOAT3 up = { 0,1,0 };//�ˋ�̏�x�N�g����ݒ�
	DirectX::XMVECTOR Up = DirectX::XMLoadFloat3(&up);

	//�㉺�̉�]
	//�v���C���[�̐^��Ɛ^���ɋ߂��Ƃ��͉�]���Ȃ�
	if (verticalDegree > 0.01f || verticalDegree < -0.01f)
	{
		DirectX::XMVECTOR Dot = DirectX::XMVector3Dot(DirectX::XMVector3Normalize(Up), DirectX::XMVector3Normalize(EyeVector));
		float dot{};
		DirectX::XMStoreFloat(&dot, Dot);
		//�^��ɋ߂���
		if (dot > 0.75f)
		{
			if (verticalDegree < -0.1f)//���͒l���}�C�i�X�̏ꍇ
			{
				DirectX::XMVECTOR Right = DirectX::XMVector3Cross(EyeVector, Up);
				DirectX::XMVECTOR Axis = DirectX::XMVector3Normalize(Right);
				DirectX::XMVECTOR Quaternion = DirectX::XMQuaternionRotationAxis(Axis, DirectX::XMConvertToRadians(verticalDegree));
				EyeVector = DirectX::XMVector3Rotate(EyeVector, Quaternion);
			}
			verticalDegree = 0;
		}
		//�^���ɋ߂���
		else if (dot < -0.22f)
		{
			if (verticalDegree > 0.1f)//���͒l���v���X�̏ꍇ
			{
				DirectX::XMVECTOR Right = DirectX::XMVector3Cross(EyeVector, Up);
				DirectX::XMVECTOR Axis = DirectX::XMVector3Normalize(Right);
				DirectX::XMVECTOR Quaternion = DirectX::XMQuaternionRotationAxis(Axis, DirectX::XMConvertToRadians(verticalDegree));
				EyeVector = DirectX::XMVector3Rotate(EyeVector, Quaternion);
			}
			verticalDegree = 0;
		}
		//�ʏ펞
		else
		{
			DirectX::XMFLOAT3 cameraRight = cameraData->GetRight();
			DirectX::XMVECTOR CameraRight = DirectX::XMLoadFloat3(&cameraRight);
			DirectX::XMVECTOR Axis = DirectX::XMVector3Normalize(CameraRight);
			DirectX::XMVECTOR Quaternion = DirectX::XMQuaternionRotationAxis(Axis, DirectX::XMConvertToRadians(verticalDegree));
			EyeVector = DirectX::XMVector3Rotate(EyeVector, Quaternion);
			verticalDegree = 0;
		}
	}
	//���E�ɉ�]
	if (horizonDegree > 0.01f || horizonDegree < -0.01f)
	{
		DirectX::XMVECTOR Axis = DirectX::XMVector3Normalize(Up);
		DirectX::XMVECTOR Quaternion = DirectX::XMQuaternionRotationAxis(Axis, DirectX::XMConvertToRadians(horizonDegree));
		EyeVector = DirectX::XMVector3Rotate(EyeVector, Quaternion);
		horizonDegree = 0;
	}

	EyeVector = DirectX::XMVector3Normalize(EyeVector);
	DirectX::XMStoreFloat3(&eyeV, EyeVector);
	cameraData->SetEyeVector(eyeV);


	//DirectX::XMFLOAT3 angle = cameraData->GetAngle();

	//const float elapsedTime = SystemManager::Instance().GetElapsedTime();

	//angle.x -= ry * rotationSpeed * elapsedTime;
	//angle.y += rx * rotationSpeed * elapsedTime;


	//angle.x = Clamp(angle.x, DirectX::XMConvertToRadians(minDegree), DirectX::XMConvertToRadians(maxDegree));
	//angle.y = CirculateRadian(angle.y);

	//cameraData->SetAngle(angle);
}

void Camera::Tracking() const
{
	if (!tracking)return;

	cameraData->SetTarget(targetObj->GetComponent<Transform>()->pos);
}

//---- �R���g���[���[��R�X�e�B�b�N��X���̈ړ��l���擾 ----
//�i�߂�l�F���͂�����Ƃ�true�j
bool Camera::SetAngleToX()
{
	//�R���g���[���[�ő��삷�鏈��

	//�Q�[���p�b�h�̎擾
	GamePad gamePad = SystemManager::Instance().GetGamePad();
	//�R���g���[���[��R�X�e�B�b�N�̈ړ��x�N�g�����擾
	float vec = (float)(gamePad.GetAxisRX());

	if (vec > 0.1f || vec < -0.1f)
	{
		horizonDegree = rotationSpeed * vec * SystemManager::Instance().GetElapsedTime();
		return true;
	}
	return false;
}

//---- �R���g���[���[��R�X�e�B�b�N��Y���̈ړ��l���擾 ----
//�i�߂�l�F���͂�����Ƃ�true�j
bool Camera::SetAngleToY()
{
	//�R���g���[���[�ő��삷�鏈��
	//�Q�[���p�b�h�̎擾
	GamePad gamePad = SystemManager::Instance().GetGamePad();
	//�R���g���[���[��R�X�e�B�b�N�̈ړ��x�N�g�����擾
	float vec = (float)(gamePad.GetAxisRY());

	if (vec > 0.1f || vec < -0.1f)
	{
		verticalDegree = rotationSpeed * -vec * SystemManager::Instance().GetElapsedTime();
		return true;
	}
	return false;
}

//---- �J�����̈ʒu�ƒ����_���Z�o���ݒ� ----
//�i�߂�l�F���͂�����Ƃ�true�j
void Camera::SetEyePoint()
{
	// �J�����̈ʒu
	DirectX::XMFLOAT3 eye = cameraData->GetTarget() + cameraData->GetEyeVector() * cameraData->GetRange();

	//�l��ۑ�
	cameraData->SetLookAt(eye, cameraData->GetTarget(), { 0.0f,1,0.0001f });
}

// �v���C���[�̃X�e�[�g�ɂ���ăJ�����̋����𓮂���
void Camera::ChangeRange()
{
	float range = cameraData->GetRange();

	//���C���[���̓J������player���班������
	if (targetObj->GetComponent<Player>()->GetCurrentState()->GetName() == "SwingWire"
		|| targetObj->GetComponent<Player>()->GetCurrentState()->GetName() == "WireJump")
	{
		if (range < 40)
			cameraData->SetRange(range + SystemManager::Instance().GetElapsedTime() * 30);
	}
	else
	{
		if (range > 30)
			cameraData->SetRange(range - SystemManager::Instance().GetElapsedTime() * 60);
	}
}

//---- �J���������Ă�����ړI�n�ɐݒ� ----
//watcherPosition:���_�̈ʒu�AtargetPosition:�����_�̈ʒu
void Camera::LockOnTarget(DirectX::XMFLOAT3 targetPosition, float rate)
{
	//DirectX::SimpleMath::Vector3 angle = cameraData->GetAngle();

	////�J�����̉�]�l����]�s��ɕϊ�
	//const DirectX::XMMATRIX Transform = DirectX::XMMatrixRotationRollPitchYaw(angle.x, angle.y, angle.z);
	////��]�s�񂩂�O�����x�N�g�������o��
	//const DirectX::XMVECTOR Front = Transform.r[2];
	//Front = DirectX::XMVector3Normalize(Front);
	//DirectX::XMFLOAT3 front{};
	//DirectX::XMStoreFloat3(&front, Front);

	//DirectX::XMFLOAT3 corEye{};
	//DirectX::XMFLOAT3 corTarget{};

	////�e���_�̃J�����ʒu�ieye�j�ƒ����_�itarget�j�����߂�
	//corTarget.x = cameraData->GetTarget().x + targetCorrection.x * front.x;
	//corTarget.y = cameraData->GetTarget().y + targetCorrection.y;
	//corTarget.z = cameraData->GetTarget().z + targetCorrection.z * front.z;
	//corEye = corTarget - front * cameraData->GetRange();


	////DirectX::XMVECTOR WatcherPos = DirectX::XMLoadFloat3(&focus);
	////DirectX::XMVECTOR TargetPos = DirectX::XMLoadFloat3(&targetPosition);
	////�ڂ��璍���_�ւ̃x�N�g�����Z�o���ۑ�
	////DirectX::XMVECTOR EyeVector = (WatcherPos - TargetPos);
	////EyeVector = DirectX::XMVector3Normalize(EyeVector);//���K��
	////DirectX::XMVECTOR CurrentFront = DirectX::XMLoadFloat3(&eyeVectorN);
	////CurrentVector = DirectX::XMVector3Normalize(CurrentVector);
	////���`��Ԃ���
	//CurrentVector = DirectX::XMVectorLerp(CurrentVector, EyeVector, rate);
	//DirectX::XMStoreFloat3(&eyeVectorN, CurrentVector);
	//EyeVector = CurrentVector;

	////�J�����̈ʒu���Z�o
	//DirectX::XMVECTOR EyePoint = WatcherPos;
	//DirectX::XMVECTOR Eye = EyePoint + EyeVector * range;

	//DirectX::XMStoreFloat3(&eye, Eye);
	//DirectX::XMStoreFloat3(&focus, EyePoint);

	////�J�����̏���ݒ�
	//cameraData->SetLookAt(eye, focus, up);
}

void Camera::ResetDefault()
{
	//DirectX::SimpleMath::Vector3 angle = cameraData->GetAngle();

	// player����J�����̎��_�@�Ɓ@default�̎��_����ς��Ċp�x���Z�o�����
		
	//�J�����̉�]�l����]�s��ɕϊ�
	//const DirectX::XMMATRIX transform = DirectX::XMMatrixRotationRollPitchYaw(angle.x, angle.y, angle.z);
	//��]�s�񂩂�O�����x�N�g�������o��
	//DirectX::XMVECTOR Front = transform.r[2];
	//Front = DirectX::XMVector3Normalize(Front);
	//DirectX::XMFLOAT3 front{};
	//DirectX::XMStoreFloat3(&front, Front);

	////�J�����̌��݂̌����Ă������
	//DirectX::SimpleMath::Vector3 eyeVector;
	//DirectX::XMStoreFloat3(&eyeVector, Front);

	//GameObject* pl = parent->GetParent()->GetChild("player");
	//if (pl == nullptr)return;
	//DirectX::SimpleMath::Vector3 playerPos = pl->GetComponent<Transform>()->pos;

	////���Z�b�g�����Ƃ��ɃJ����������ʒu
	//DirectX::SimpleMath::Vector3 targetPos;
	//targetPos.x = playerPos.x + targetCorrection.x;
	//targetPos.y = playerPos.y + targetCorrection.y;
	//targetPos.z = playerPos.z + targetCorrection.z;

	//// player��front
	//DirectX::SimpleMath::Vector3 plFront = pl->GetComponent<Transform>()->GetForward();
	//plFront.Normalize();

	//DirectX::SimpleMath::Vector3 eye = targetPos + -plFront * cameraData->GetRange();

	//cameraData->SetAngle({ 0, 0, 0 });

	////�J�����̏���ݒ�
	//cameraData->SetLookAt(eye, targetPos, { 0.0f,1,0.0001f });

}
