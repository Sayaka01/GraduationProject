
#include "Stdafx.h"
#include "PhysicsFunction.h"

#include "Component/SphereCollider.h"
#include "Component/CapsuleCollider.h"

#include "UserFunction.h"

using namespace DirectX;

bool Physics::IntersectSphereVsSphere(SphereCollider* colliderA, SphereCollider* colliderB, IntersectionResult* result)
{
	XMFLOAT3 centerA = colliderA->GetCenter();
	XMFLOAT3 centerB = colliderB->GetCenter();

	XMFLOAT3 vec = centerA - centerB;
	float lengthSq = LengthFloat3(vec);

	// 距離判定
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

	float t = DotFloat3(capDirection, sphere->GetCenter() - capsule->begin);


	XMFLOAT3 point = {};	// カプセル中心線上の最近点
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


	// 交差判定
	XMFLOAT3 vec = sphere->GetCenter() - point;

	float radiusAdd = sphere->radius + capsule->radius;
	float dist = LengthFloat3(vec);

	if (result)
	{
		result->normal = NormalizeFloat3(vec);
		result->penetration = radiusAdd - dist;
		result->pointA = sphere->GetCenter() + result->normal * (-sphere->radius);
		result->pointB = point + result->normal * capsule->radius;
	}

	return dist < radiusAdd;
}
