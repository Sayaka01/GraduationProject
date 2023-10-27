
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
	//�X�t�B�A���b�V���̐���
	sphereMesh = new SphereMeshRenderer();
	//�J�v�Z�����b�V���̐���
	capsuleMesh = new CapsuleMeshRenderer();
}

void CollideManager::Finalize()
{
	//�X�t�B�A���b�V���̍폜
	SafeDelete(sphereMesh);
	sphereColliders.clear();

	//�J�v�Z�����b�V���̍폜
	SafeDelete(capsuleMesh);
	capsuleColliders.clear();
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
			if (colliderA->GetParentName() == colliderB->GetParentName())continue;
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
		if (collider->GetEnable() && collider->drawDebugPrimitive)
			sphereMesh->Draw(collider);
	}

	//�J�v�Z���`��
	capsuleMesh->DrawPrepare();
	for (CapsuleCollider* collider : capsuleColliders)
	{
		if (collider->GetEnable() && collider->drawDebugPrimitive)
			capsuleMesh->Draw(collider);
	}
}
