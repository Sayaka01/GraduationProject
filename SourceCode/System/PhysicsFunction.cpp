
#include "Stdafx.h"
#include "PhysicsFunction.h"

#include "Component/SphereCollider.h"

#include "UserFunction.h"

using namespace DirectX;

bool Physics::IntersectSphereVsSphere(SphereCollider* colliderA, SphereCollider* colliderB, IntersectionResult* result)
{
	XMFLOAT3 vec = colliderA->center - colliderB->center;
	float lengthSq = LengthFloat3(vec);

	// ‹——£”»’è
	float range = colliderA->radius + colliderB->radius;

	if (lengthSq > range)
	{
		return false;
	}

	if (result)
	{
		result->normal = NormalizeFloat3(vec);
		result->penetration = sqrtf(range - lengthSq);
		result->pointA = colliderA->center + result->normal * (-colliderA->radius);
		result->pointB = colliderB->center + result->normal * colliderB->radius;
	}

	return true;
}
