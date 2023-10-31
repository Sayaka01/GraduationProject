#pragma once

#include <DirectXMath.h>


class SphereCollider;
class BoxCollider;
class CapsuleCollider;

namespace Physics
{
	//レイ
	struct Ray
	{
		DirectX::XMFLOAT3	start = {};		  
		DirectX::XMFLOAT3	end = {};	      
	};

	// オブジェクトの交差結果の構造体
	struct IntersectionResult
	{
		DirectX::XMFLOAT3	pointA = {};			// オブジェクトA側の交点。オブジェクトA Vs オブジェクトBで関数が構成される
		DirectX::XMFLOAT3	pointB = {};			// オブジェクトB側の交点。オブジェクトA Vs オブジェクトBで関数が構成される
		DirectX::XMFLOAT3	normal = {};			// 交点を結ぶ衝突の単位法線ベクトル。方向はオブジェクトB→オブジェクトA
		float				penetration = 0.0f; 	// 法線ベクトルを元にしためり込み量。交差している場合にプラスの値が返却される
	};

	// キャスト系の結果の構造体
	struct CastResult
	{
		DirectX::XMVECTOR	point = {};				// 交点
		DirectX::XMVECTOR	normal = {};			// 衝突したポリゴンの単位法線ベクトル
		float				distance = 0.0f; 		// レイの始点から交点までの距離
	};

	// 球Vs球
	bool IntersectSphereVsSphere(
		SphereCollider* colliderA,
		SphereCollider* colliderB,
		IntersectionResult* result = {});

	//球vsカプセル
	bool IntersectSphereVsCapsule(
		SphereCollider* sphereCollider,
		CapsuleCollider* capsuleCollider,
		IntersectionResult* result);

	//レイvsAABB((軸平行スラブ)
	bool IntersectRayVsAABB(
		Ray& ray,
		BoxCollider* boxCollider,
		CastResult* result);
}