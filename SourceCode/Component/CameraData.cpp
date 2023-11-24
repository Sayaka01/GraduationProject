#include "Stdafx.h"
#include "CameraData.h"

#include "System/Common.h"
#include "System/UserFunction.h"

#include "System/SystemManager.h"
#include <SimpleMath.h>

CameraData::CameraData()
{
	constantBuffer.Initialize(SystemManager::Instance().GetDevice(), &cameraConstants);
}

CameraData::~CameraData()
{

}

void CameraData::Initialize()
{
	//�J�����̉�]�l����]�s��ɕϊ�
	//const DirectX::XMMATRIX Transform = DirectX::XMMatrixRotationRollPitchYaw(angle.x, angle.y, angle.z);

	//��]�s�񂩂�O�����x�N�g�������o��
	//const DirectX::XMVECTOR Front = Transform.r[2];
	//DirectX::XMFLOAT3 front{};
	//DirectX::XMStoreFloat3(&front, Front);

	//DirectX::XMFLOAT3 corEye{};
	front = { 0,0,-1 };

	//�����_�itarget�j�����߂�
	corTarget.x = target.x + targetCorrection.x * front.x;
	corTarget.y = target.y + targetCorrection.y;
	corTarget.z = target.z + targetCorrection.z * front.z;

	//DirectX::XMFLOAT3 back = { -front.x,-front.y,-front.z };
	//�J���������Ă���ʒu����J������u���Ă���ʒu�܂ł̃x�N�g�����Z�o
	eyeVector = front + DirectX::XMFLOAT3(0,0.01f,0)/*�X��*/;
	//�J�����̈ʒu
	eye = corTarget - eyeVector * range;
	
	SetLookAt(eye, corTarget, { 0.0f,1,0.0001f });

}

DirectX::XMFLOAT3 CameraData::Update()
{
	//�J�����̉�]�l����]�s��ɕϊ�
	//const DirectX::XMMATRIX Transform = DirectX::XMMatrixRotationRollPitchYaw(angle.x, angle.y, angle.z);

	//��]�s�񂩂�O�����x�N�g�������o��
	//const DirectX::XMVECTOR Front = Transform.r[2];
	//DirectX::XMFLOAT3 front{};
	//DirectX::XMStoreFloat3(&front, Front);

	//DirectX::XMFLOAT3 corEye{};

	//�e���_�̃J�����ʒu�ieye�j�ƒ����_�itarget�j�����߂�
	corTarget.x = target.x + targetCorrection.x * front.x;
	corTarget.y = target.y + targetCorrection.y;
	corTarget.z = target.z + targetCorrection.z * front.z;
	eye = corTarget - front * range;
	
	//�J�����V�F�C�N
	if (shakeTimer > 0.0f)
	{
		shakeTimer -= SystemManager::Instance().GetElapsedTime();
		if (shakeTimer < 0.0f) shakeTimer = 0.0f;
		Shake();
	}
	eye += provisionalTarget * shakeTimer * 0.1f;
	DirectX::XMFLOAT3 t = corTarget + provisionalTarget * shakeTimer * 0.1f;

	//�J�����̎��_�ƒ����_��ݒ�
	SetLookAt(eye, t, { 0.0f,1,0.0001f });

	return eye;
}

//�w�����������
void CameraData::SetLookAt(const DirectX::XMFLOAT3& eye, const DirectX::XMFLOAT3& target, const DirectX::XMFLOAT3& up)
{
	//���_�A�����_�A���������r���[�s��,�r���[�v���W�F�N�V�����s����쐬
	DirectX::XMVECTOR Eye = DirectX::XMLoadFloat3(&eye);
	DirectX::XMVECTOR Target = DirectX::XMLoadFloat3(&target);
	DirectX::XMVECTOR Up = DirectX::XMLoadFloat3(&up);
	DirectX::XMMATRIX P = DirectX::XMLoadFloat4x4(&projection);
	DirectX::XMMATRIX View = DirectX::XMMatrixLookAtLH(Eye, Target, Up);
	DirectX::XMStoreFloat4x4(&view, View);
	DirectX::XMStoreFloat4x4(&viewProjection, View * P);


	//�r���[���t�s�񉻂��A���[���h���W�ɖ߂�
	DirectX::XMMATRIX World = DirectX::XMMatrixInverse(nullptr, View);
	DirectX::XMFLOAT4X4 world;
	DirectX::XMStoreFloat4x4(&world, World);

	//�J�����̕��������o��
	this->right.x = world._11;
	this->right.y = world._12;
	this->right.z = world._13;

	this->up.x = world._21;
	this->up.y = world._22;
	this->up.z = world._23;

	this->front.x = world._31;
	this->front.y = world._32;
	this->front.z = world._33;
	
	this->eye = eye;
	this->target = target;

}

void CameraData::UpdateBuffer(const DirectX::XMFLOAT3 pos)
{
	cameraConstants.viewProjection = viewProjection;
	cameraConstants.position = { pos.x,pos.y,pos.z,0.0f };

	constantBuffer.SetConstantBuffer(SystemManager::Instance().GetDeviceContext(), ConstantBuffer::ShaderType::ALL, ConstantBuffer::UsageType::Camera, &cameraConstants);
}

void CameraData::DebugGui()
{
	ImGui::DragFloat("range", &range, 0.1f);
	ImGui::DragFloat3("target", &target.x, 0.01f);
	ImGui::DragFloat3("targetCorrection", &targetCorrection.x, 0.01f);
	//ImGui::DragFloat3("angle", &angle.x, 0.01f);
	ImGui::DragFloat3("eye", &eye.x, 0.01f);
	ImGui::DragFloat3("eyeVector", &eyeVector.x, 0.01f);
	ImGui::DragFloat3("up", &up.x, 0.01f);
	ImGui::DragFloat3("front", &front.x, 0.01f);
	ImGui::DragFloat3("provisionalTarget", &provisionalTarget.x, 0.01f);

	ImGui::DragFloat3("ShakePower", &shakePower.x, 0.1f);

	if (ImGui::Button("Shake"))
	{
		shakeTimer = maxShakeTime;
	}
}

void CameraData::SetPerspectiveFov(float fovY, float aspect, float nearZ, float farZ)
{
	//�p�[�X�y�N�e�B�u���e�s����쐬
	XMMATRIX Projection = XMMatrixPerspectiveFovLH(fovY, aspect, nearZ, farZ);
	XMStoreFloat4x4(&projection, Projection);
}

void CameraData::Shake()
{
	provisionalTarget = { (float)(rand() % 6) - 3.0f,(float)(rand() % 4) - 2.0f,0.0f };
	provisionalTarget *= shakePower;
}
