
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

	float t = DotFloat3(capDirection, sphere->center - capsule->begin);


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
	// ループ処理するため、xyz成分を配列に代入する
	float start[3] = { ray.start.x, ray.start.y ,ray.start.z };
	float end[3] = { ray.end.x, ray.end.y ,ray.end.z };
	float rayLength = LengthFloat3(ray.end - ray.start);
	DirectX::XMFLOAT3 rayDir = NormalizeFloat3(ray.end - ray.start);
	float dir[3] = { rayDir.x, rayDir.y, rayDir.z };
	float center[3] = { boxCollider->center.x, boxCollider->center.y, boxCollider->center.z };
	float size[3] = { boxCollider->size.x, boxCollider->size.y, boxCollider->size.z };

	// 直線とスラブの２交点までの距離をtminとtmaxと定義
	float tmin = 0.0f;
	float tmax = FLT_MAX;
	int minAxis = 0;

	bool ret = true;

	// スラブとの距離を算出し交差しているかの確認と最近点の算出を行う
	for (int i = 0; i < 3; i++)
	{
		//xyz軸との平行確認
		if (fabsf(dir[i]) < FLT_EPSILON)
		{
			// 平行の場合、位置関係の比較を行い範囲内になければ交差なし
			if (start[i] < center[i] - size[i] || start[i] > center[i] + size[i])
			{
				ret = false;
			}
		}
		else
		{
			// t1が近スラブ、t2が遠スラブとの距離
			float ood = 1.0f / dir[i];
			float t1 = (center[i] - size[i] - start[i]) * ood;
			float t2 = (center[i] + size[i] - start[i]) * ood;

			// 遠近が逆転している場合があるので、その場合入れ替えておく
			if (t1 > t2)
			{
				float tmp = t1;
				t1 = t2;
				t2 = tmp;
			}

			// t1がtminよりも大きい場合、tminをt1で更新する
			if (t1 > tmin)
			{
				tmin = t1;
				minAxis = i;
			}

			// t2がtmaxよりも小さい場合、tmaxをt2で更新する
			if (t2 < tmax)
			{
				tmax = t2;
			}
		}
	}

	if (result)
	{
		result->distance = tmin;
		result->point = ray.start + rayDir * result->distance;
		DirectX::XMFLOAT3 norm = { static_cast<float>(minAxis == 0),   static_cast<float>(minAxis == 1),  static_cast<float>(minAxis == 2) };

		if (dir[minAxis] > 0)
		{
			norm = { -norm.x, -norm.y, -norm.z };
		}

		result->normal = norm;

		//if (resultFar)
		//{
		//	resultFar->distance = tmax;
		//	resultFar->point = DirectX::XMVectorAdd(rayStart, DirectX::XMVectorScale(rayDirection, resultFar->distance));
		//	DirectX::XMVECTOR norm = { static_cast<float>(minAxis == 0),   static_cast<float>(minAxis == 1),  static_cast<float>(minAxis == 2) };
		//	if (dArray[minAxis] > 0)
		//	{
		//		norm = DirectX::XMVectorNegate(norm);
		//	}
		//	resultFar->normal = norm;
		//}
	}

	return ret && (rayLength >= tmin);
}
