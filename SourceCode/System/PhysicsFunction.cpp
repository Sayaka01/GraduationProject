
#include "Stdafx.h"
#include "PhysicsFunction.h"

#include "Component/SphereCollider.h"
#include "Component/BoxCollider.h"
#include "Component/CapsuleCollider.h"

#include "UserFunction.h"

using namespace DirectX;

bool Physics::IntersectSphereVsSphere(SphereCollider* colliderA, SphereCollider* colliderB, IntersectionResult* result)
{
	XMFLOAT3 centerA = colliderA->center;
	XMFLOAT3 centerB = colliderB->center;

	XMFLOAT3 vec = centerA - centerB;
	float lengthSq = LengthFloat3(vec);

	// ��������
	float range = colliderA->radius + colliderB->radius;

	if (lengthSq > range)
	{
		return false;
	}

	if (result)
	{
		result->normal = NormalizeFloat3(vec);
		result->penetration = sqrtf(range - lengthSq);
		result->pointA = centerA + result->normal * (-colliderA->radius);
		result->pointB = centerB + result->normal * colliderB->radius;
	}

	return true;
}

bool Physics::IntersectSphereVsCapsule(SphereCollider* sphere, CapsuleCollider* capsule, IntersectionResult* result)
{
	XMFLOAT3 capDirection = capsule->end - capsule->begin;

	float capHeight = LengthFloat3(capDirection);

	capDirection = NormalizeFloat3(capDirection);

	float t = DotFloat3(capDirection, sphere->center - capsule->begin);


	XMFLOAT3 point = {};	// �J�v�Z�����S����̍ŋߓ_
	if (t < 0)
	{
		point = capsule->begin;
	}
	else if (t > capHeight)
	{
		point = capsule->end;
	}
	else
	{
		point = capsule->begin + capDirection * t;
	}


	// ��������
	XMFLOAT3 vec = sphere->center - point;

	float radiusAdd = sphere->radius + capsule->radius;
	float dist = LengthFloat3(vec);

	if (result)
	{
		result->normal = NormalizeFloat3(vec);
		result->penetration = radiusAdd - dist;
		result->pointA = sphere->center + result->normal * (-sphere->radius);
		result->pointB = point + result->normal * capsule->radius;
	}

	return dist < radiusAdd;
}

bool Physics::IntersectRayVsAABB(Ray& ray, BoxCollider* boxCollider, CastResult* result)
{
	// ���[�v�������邽�߁Axyz������z��ɑ������
	float start[3] = { ray.start.x, ray.start.y ,ray.start.z };
	float end[3] = { ray.end.x, ray.end.y ,ray.end.z };
	//float center[3] = {boxCollider->center.x,



	//float startPosArray[3] = { DirectX::XMVectorGetX(rayStart), DirectX::XMVectorGetY(rayStart), DirectX::XMVectorGetZ(rayStart) };
	//float slubCenterArray[3] = { DirectX::XMVectorGetX(slubCenter), DirectX::XMVectorGetY(slubCenter), DirectX::XMVectorGetZ(slubCenter) };
	//float slubRadiiArray[3] = { DirectX::XMVectorGetX(slubRadii), DirectX::XMVectorGetY(slubRadii), DirectX::XMVectorGetZ(slubRadii) };
	//float dArray[3] = { DirectX::XMVectorGetX(rayDirection), DirectX::XMVectorGetY(rayDirection), DirectX::XMVectorGetZ(rayDirection) };

	//// �����ƃX���u�̂Q��_�܂ł̋�����tmin��tmax�ƒ�`
	//float tmin = 0.0f;
	//float tmax = FLT_MAX;
	//int minAxis = 0;

	//bool ret = true;

	//// �X���u�Ƃ̋������Z�o���������Ă��邩�̊m�F�ƍŋߓ_�̎Z�o���s��
	//for (int i = 0; i < 3; i++)
	//{
	//	//xyz���Ƃ̕��s�m�F
	//	if (fabsf(dArray[i]) < FLT_EPSILON)
	//	{
	//		// ���s�̏ꍇ�A�ʒu�֌W�̔�r���s���͈͓��ɂȂ���Ό����Ȃ�
	//		if (startPosArray[i] < slubCenterArray[i] - slubRadiiArray[i] || startPosArray[i] > slubCenterArray[i] + slubRadiiArray[i])
	//		{
	//			ret = false;
	//		}
	//	}
	//	else
	//	{
	//		// t1���߃X���u�At2�����X���u�Ƃ̋���
	//		float ood = 1.0f / dArray[i];
	//		float t1 = (slubCenterArray[i] - slubRadiiArray[i] - startPosArray[i]) * ood;
	//		float t2 = (slubCenterArray[i] + slubRadiiArray[i] - startPosArray[i]) * ood;

	//		// ���߂��t�]���Ă���ꍇ������̂ŁA���̏ꍇ����ւ��Ă���
	//		if (t1 > t2)
	//		{
	//			float tmp = t1;
	//			t1 = t2;
	//			t2 = tmp;
	//		}

	//		// t1��tmin�����傫���ꍇ�Atmin��t1�ōX�V����
	//		if (t1 > tmin)
	//		{
	//			tmin = t1;
	//			minAxis = i;
	//		}

	//		// t2��tmax�����������ꍇ�Atmax��t2�ōX�V����
	//		if (t2 < tmax)
	//		{
	//			tmax = t2;
	//		}
	//	}
	//}

	//if (resultNear)
	//{
	//	resultNear->distance = tmin;
	//	resultNear->point = DirectX::XMVectorAdd(rayStart, DirectX::XMVectorScale(rayDirection, resultNear->distance));
	//	DirectX::XMVECTOR norm = { static_cast<float>(minAxis == 0),   static_cast<float>(minAxis == 1),  static_cast<float>(minAxis == 2) };
	//	if (dArray[minAxis] > 0)
	//	{
	//		norm = DirectX::XMVectorNegate(norm);
	//	}
	//	resultNear->normal = norm;

	//	if (resultFar)
	//	{
	//		resultFar->distance = tmax;
	//		resultFar->point = DirectX::XMVectorAdd(rayStart, DirectX::XMVectorScale(rayDirection, resultFar->distance));
	//		DirectX::XMVECTOR norm = { static_cast<float>(minAxis == 0),   static_cast<float>(minAxis == 1),  static_cast<float>(minAxis == 2) };
	//		if (dArray[minAxis] > 0)
	//		{
	//			norm = DirectX::XMVectorNegate(norm);
	//		}
	//		resultFar->normal = norm;
	//	}
	//}

	//return ret && (rayDist >= tmin);

	return false;
}
