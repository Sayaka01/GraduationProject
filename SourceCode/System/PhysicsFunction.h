#pragma once

#include <DirectXMath.h>


class SphereCollider;

namespace Physics
{
	// オブジェクトの交差結果の構造体
	struct IntersectionResult
	{
		DirectX::XMFLOAT3	pointA = {};			// オブジェクトA側の交点。オブジェクトA Vs オブジェクトBで関数が構成される
		DirectX::XMFLOAT3	pointB = {};			// オブジェクトB側の交点。オブジェクトA Vs オブジェクトBで関数が構成される
		DirectX::XMFLOAT3	normal = {};			// 交点を結ぶ衝突の単位法線ベクトル。方向はオブジェクトB→オブジェクトA
		float				penetration = 0.0f; 	// 法線ベクトルを元にしためり込み量。交差している場合にプラスの値が返却される
	};

	// 球Vs球
	bool IntersectSphereVsSphere(
		SphereCollider* colliderA,
		SphereCollider* colliderB,
		IntersectionResult* result = {});

}