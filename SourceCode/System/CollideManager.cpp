
#include "Stdafx.h"
#include "CollideManager.h"

#include "Component/SphereMeshRenderer.h"
#include "Component/SphereCollider.h"
#include "Component/CapsuleMeshRenderer.h"
#include "Component/CapsuleCollider.h"

#include "System/PhysicsFunction.h"
#include "System/UserFunction.h"

void CollideManager::Initialize()
{
	//スフィアメッシュの生成
	sphereMesh = new SphereMeshRenderer();
	//カプセルメッシュの生成
	capsuleMesh = new CapsuleMeshRenderer();
}

void CollideManager::Finalize()
{
	//スフィアメッシュの削除
	SafeDelete(sphereMesh);
	sphereColliders.clear();

	//カプセルメッシュの削除
	SafeDelete(capsuleMesh);
	capsuleColliders.clear();
}

void CollideManager::Collide()
{
	//当たり判定を行う

	Physics::IntersectionResult result;

	//Sphere
	for (SphereCollider* colliderA : sphereColliders)
	{
		for (SphereCollider* colliderB : sphereColliders)
		{
			if (!colliderA->GetEnable())continue;
			if (!colliderB->GetEnable())continue;
			if (colliderA->GetParentName() == colliderB->GetParentName())continue;
			if (colliderA == colliderB)continue;

			if (Physics::IntersectSphereVsSphere(colliderA, colliderB, &result))
			{
				if (colliderA->priority > colliderB->priority)
				{
					//colliderAを押し出す
					colliderA->center += result.normal * result.penetration;					
				}
				else if(colliderA->priority < colliderB->priority)
				{
					//colliderBを押し出す
					colliderB->center -= result.normal * result.penetration;
				}
				else
				{
					//半分ずつ押し出す
					colliderA->center += result.normal * result.penetration * 0.5f;					
					colliderB->center -= result.normal * result.penetration * 0.5f;
				}

				colliderA->OnCollisionEnter(colliderB);
				colliderB->OnCollisionEnter(colliderA);

			}
		}
	}
}

void CollideManager::Draw()
{
	//球描画
	sphereMesh->DrawPrepare();
	for (SphereCollider* collider : sphereColliders)
	{
		if (collider->GetEnable() && collider->drawDebugPrimitive)
			sphereMesh->Draw(collider);
	}

	//カプセル描画
	capsuleMesh->DrawPrepare();
	for (CapsuleCollider* collider : capsuleColliders)
	{
		if (collider->GetEnable() && collider->drawDebugPrimitive)
			capsuleMesh->Draw(collider);
	}
}
