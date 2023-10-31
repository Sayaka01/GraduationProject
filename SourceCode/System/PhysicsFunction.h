#pragma once

#include <DirectXMath.h>


class SphereCollider;
class BoxCollider;
class CapsuleCollider;

namespace Physics
{
	//���C
	struct Ray
	{
		DirectX::XMFLOAT3	start = {};		  
		DirectX::XMFLOAT3	end = {};	      
	};

	// �I�u�W�F�N�g�̌������ʂ̍\����
	struct IntersectionResult
	{
		DirectX::XMFLOAT3	pointA = {};			// �I�u�W�F�N�gA���̌�_�B�I�u�W�F�N�gA Vs �I�u�W�F�N�gB�Ŋ֐����\�������
		DirectX::XMFLOAT3	pointB = {};			// �I�u�W�F�N�gB���̌�_�B�I�u�W�F�N�gA Vs �I�u�W�F�N�gB�Ŋ֐����\�������
		DirectX::XMFLOAT3	normal = {};			// ��_�����ԏՓ˂̒P�ʖ@���x�N�g���B�����̓I�u�W�F�N�gB���I�u�W�F�N�gA
		float				penetration = 0.0f; 	// �@���x�N�g�������ɂ����߂荞�ݗʁB�������Ă���ꍇ�Ƀv���X�̒l���ԋp�����
	};

	// �L���X�g�n�̌��ʂ̍\����
	struct CastResult
	{
		DirectX::XMVECTOR	point = {};				// ��_
		DirectX::XMVECTOR	normal = {};			// �Փ˂����|���S���̒P�ʖ@���x�N�g��
		float				distance = 0.0f; 		// ���C�̎n�_�����_�܂ł̋���
	};

	// ��Vs��
	bool IntersectSphereVsSphere(
		SphereCollider* colliderA,
		SphereCollider* colliderB,
		IntersectionResult* result = {});

	//��vs�J�v�Z��
	bool IntersectSphereVsCapsule(
		SphereCollider* sphereCollider,
		CapsuleCollider* capsuleCollider,
		IntersectionResult* result);

	//���CvsAABB((�����s�X���u)
	bool IntersectRayVsAABB(
		Ray& ray,
		BoxCollider* boxCollider,
		CastResult* result);
}