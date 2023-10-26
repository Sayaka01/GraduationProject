#pragma once

#include <DirectXMath.h>


class SphereCollider;

namespace Physics
{
	// �I�u�W�F�N�g�̌������ʂ̍\����
	struct IntersectionResult
	{
		DirectX::XMFLOAT3	pointA = {};			// �I�u�W�F�N�gA���̌�_�B�I�u�W�F�N�gA Vs �I�u�W�F�N�gB�Ŋ֐����\�������
		DirectX::XMFLOAT3	pointB = {};			// �I�u�W�F�N�gB���̌�_�B�I�u�W�F�N�gA Vs �I�u�W�F�N�gB�Ŋ֐����\�������
		DirectX::XMFLOAT3	normal = {};			// ��_�����ԏՓ˂̒P�ʖ@���x�N�g���B�����̓I�u�W�F�N�gB���I�u�W�F�N�gA
		float				penetration = 0.0f; 	// �@���x�N�g�������ɂ����߂荞�ݗʁB�������Ă���ꍇ�Ƀv���X�̒l���ԋp�����
	};

	// ��Vs��
	bool IntersectSphereVsSphere(
		SphereCollider* colliderA,
		SphereCollider* colliderB,
		IntersectionResult* result = {});

}