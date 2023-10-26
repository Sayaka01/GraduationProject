
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
	//�����蔻����s��

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
					//colliderA�������o��
					colliderA->center += result.normal * result.penetration;					
				}
				else if(colliderA->priority < colliderB->priority)
				{
					//colliderB�������o��
					colliderB->center -= result.normal * result.penetration;
				}
				else
				{
					//�����������o��
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
	//���`��
	sphereMesh->DrawPrepare();
	for (SphereCollider* collider : sphereColliders)
	{
		if (collider->drawDebugPrimitive)
			sphereMesh->Draw(collider);
	}
}
