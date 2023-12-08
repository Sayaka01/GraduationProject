
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
		if (targetObj != nullptr)
		{
			fetchFront = targetObj->GetComponent<Transform>()->GetForward();
			lockingOn = true;
		}
	}

	if (lockingOn)
	{
		DirectX::XMFLOAT3 velo = fetchFront * cameraData->GetRange();
		lockingOn = LockOnTarget(cameraData->GetTarget() + cameraData->GetTargetCorrection() - velo/*eye*/, rateInCamera);
		//if (rateInCamera < 1)
		//	rateInCamera += SystemManager::Instance().GetElapsedTime();
		//else
		//{
		//	rateInCamera = 0;
		//	lockingOn = false;
		//}

	}



	// �J�����̋����𓮂���
	//ChangeRange();
}

//�`��
void Camera::Draw3D()
{
	UpdateBuffer();
}

//ImGui
void Camera::DebugGui()
{
	if (ImGui::TreeNode(name.c_str()))
	{
		cameraData->DebugGui();
		//if (ImGui::Button("reset"))
		//{
		//	ResetDefault();
		//}

		ImGui::SliderFloat("rate", &rateInCamera,0.0f,1.0f);

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

	//cameraData->SetTarget({0,0,0});
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
bool Camera::LockOnTarget(DirectX::XMFLOAT3 eyeP, float rate)
{
	//��]���eye����target�ւ̃x�N�g��
	DirectX::SimpleMath::Vector3 eyeVector = eyeP - cameraData->GetTarget();
	eyeVector.Normalize();//���K��

	//���݂̃J�����̑O�x�N�g��
	DirectX::SimpleMath::Vector3 currentVector = cameraData->GetEyeVector();
	currentVector.Normalize();//���K��

	DirectX::SimpleMath::Vector3 up = CrossFloat3(eyeVector, currentVector);
	up.Normalize();
	if (up.Length() == 0)return false;
	float dot = DotFloat3(eyeVector, currentVector);
	dot = acosf(dot);// �p�x�Z�o

	if (fabsf(DirectX::XMConvertToDegrees(dot)) <= 1)return false;

	DirectX::XMVECTOR EyeVector = DirectX::XMLoadFloat3(&eyeVector);

	//���E�ɉ�]
	//if (horizonDegree > 0.01f || horizonDegree < -0.01f)
	{
		DirectX::XMVECTOR Axis = DirectX::XMVector3Normalize(DirectX::XMLoadFloat3(&up));
		DirectX::XMVECTOR Quaternion = DirectX::XMQuaternionRotationAxis(DirectX::XMLoadFloat3(&up),0);
		EyeVector = DirectX::XMVector3Rotate(EyeVector, Quaternion);
	}

#ifdef _DEBUG
	ImGui::Begin("camera reset");
	ImGui::SliderFloat3("up",&up.x, -1.0f, 1.0f);
	ImGui::End();

#endif

	EyeVector = DirectX::XMVector3Normalize(EyeVector);
	DirectX::XMStoreFloat3(&eyeVector, EyeVector);
	cameraData->SetEyeVector(eyeVector);

	//cameraData->SetTarget(cameraData->GetEye() + eyeVector * cameraData->GetRange());

	//float cy = currentVector.y;
	//float ey = eyeVector.y;

	//currentVector.y = 0;
	//eyeVector.y = 0;



	//���`��Ԃ���
	//currentVector = DirectX::XMVectorLerp(currentVector, -eyeVector, rate);
	//cameraData->SetEyeVector(currentVector);
	//currentVector.y = cy;


	//�J�����̈ʒu���Z�o
	//DirectX::SimpleMath::Vector3 target = cameraData->GetTarget();
	//DirectX::SimpleMath::Vector3 targetCorrection = cameraData->GetTargetCorrection();
	//DirectX::SimpleMath::Vector3 eye = target + targetCorrection + (currentVector * cameraData->GetRange());

	//cameraData->SetEye(eye);

	//�J�����̏���ݒ�
	//cameraData->SetLookAt(eye, target, { 0.0f,1,0.0001f });

	return true;
}