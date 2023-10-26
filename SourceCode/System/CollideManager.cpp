
#include "Stdafx.h"
#include "CollideManager.h"

#include "Component/SphereMeshRenderer.h"
#include "Component/SphereCollider.h"

#include "System/PhysicsFunction.h"
#include "System/UserFunction.h"

void CollideManager::Initialize()
{
	sphereMesh = new SphereMeshRenderer();

}

void CollideManager::Finalize()
{
	if (sphereMesh)
	{
		delete sphereMesh;
		sphereMesh = nullptr;
	}

	sphereColliders.clear();
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
		if (collider->drawDebugPrimitive)
			sphereMesh->Draw(collider);
	}
}
